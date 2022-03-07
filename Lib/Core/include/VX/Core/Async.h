#pragma once

#include <concepts>
#include <filesystem>

#include <uvw/async.h>
#include <uvw/loop.h>
#include <uvw/timer.h>
#include <uvw/fs.h>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>
#include <VX/ByteBuffer.h>
#include <VX/Expected.h>

#include <VX/Core/Assert.h>
#include <VX/Core/Closable.h>
#include <VX/Core/Types.h>

// A quick and dirty promise-like system for wrapping async
// operations on the event loop. Is currently not completely memory safe and
// not optimal in performance.

namespace VX::Core {
    class Executor;

    class EventLoop;

    template<typename T>
    class EventEmitter;

    template<typename T>
    class Promise;

    template<typename T>
    class Publisher;

    template<typename T>
    class Socket;

    template<typename T>
    class Subscriber;

    using Millis = std::chrono::duration<uint64_t, std::milli>;

    class Executor final {
        VX_DEFAULT_MOVABLE(Executor);
        VX_DEFAULT_COPYABLE(Executor);

    private:
        std::shared_ptr<uvw::Loop> m_event_loop;

        explicit Executor(std::shared_ptr<uvw::Loop> event_loop)
                : m_event_loop(std::move(event_loop))
        {
        }

    public:
        ~Executor() = default;

        friend class EventLoop;

        template<typename T>
        friend std::shared_ptr<T> make_handle(Executor &);
    };

    template<typename T>
    inline std::shared_ptr<T> make_handle(Executor &e)
    {
        VX_ASSERT(e.m_event_loop != nullptr, "The underlying event loop is gone");
        return e.m_event_loop->resource<T>();
    }

    template<typename T>
    class Subscriber final {
    VX_DEFAULT_MOVABLE(Subscriber);

    VX_DEFAULT_COPYABLE(Subscriber);

    private:
        std::shared_ptr<uvw::AsyncHandle> m_handle;

    public:
        explicit Subscriber(std::shared_ptr<uvw::AsyncHandle> handle)
                : m_handle(std::move(handle))
        {
        }

        ~Subscriber() = default;

        Closable on(std::function<void(T &)> callback)
        {
            auto connection = m_handle->on<uvw::AsyncEvent>(
                    [cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                        cb(*h.data<T>());
                    });

            Closable c([handle = m_handle, conn = std::move(connection)]() {
                if (handle && handle->active())
                    handle->erase(conn);
            });

            return c;
        }

        void on_detached(std::function<void(T &)> callback)
        {
            m_handle->on<uvw::AsyncEvent>(
                    [cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                        cb(*h.data<T>());
                    });
        }

        Closable once(std::function<void(T &)> callback)
        {
            auto connection = m_handle->once<uvw::AsyncEvent>(
                    [cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                        cb(*h.data<T>());
                    });

            Closable c([handle = m_handle, conn = std::move(connection)]() {
                if (handle && handle->active())
                    handle->erase(conn);
            });

            return c;
        }

        void once_detached(std::function<void(T &)> callback)
        {
            m_handle->once<uvw::AsyncEvent>(
                    [cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                        cb(*h.data<T>());
                    });
        }

        friend class Executor;
    };

    template<typename T>
    class Publisher final {
        VX_DEFAULT_MOVABLE(Publisher);
        VX_DEFAULT_COPYABLE(Publisher);

    private:
        std::shared_ptr<uvw::AsyncHandle> m_handle;

    public:
        explicit Publisher(std::shared_ptr<uvw::AsyncHandle> handle)
                : m_handle(std::move(handle))
        {
        }

        ~Publisher() = default;

        void publish(T message) const
        {
            auto message_data = std::make_shared<T>(std::move(message));
            m_handle->data(std::move(message_data));
            m_handle->send();
        }
    };

    template<typename T>
    struct Socket {
        VX_DEFAULT_COPYABLE(Socket);
        VX_DEFAULT_MOVABLE(Socket);

        Publisher<T> publisher;
        Subscriber<T> subscriber;

        Socket(Publisher<T> &publisher, Subscriber<T> &subscriber)
                : publisher(std::move(publisher))
                , subscriber(std::move(subscriber))
        {
        }

        Socket(const Publisher<T> &publisher, const Subscriber<T> &subscriber)
                : publisher(publisher)
                , subscriber(subscriber)
        {
        }

        explicit Socket(Executor &e);

        explicit Socket(std::shared_ptr<uvw::AsyncHandle>);
    };

    template<typename T>
    class EventEmitter final {
        VX_DEFAULT_MOVABLE(EventEmitter);
        VX_DEFAULT_COPYABLE(EventEmitter);

    private:
        std::shared_ptr<uvw::AsyncHandle> m_handle;
        Socket<T> m_socket;

    public:
        explicit EventEmitter(Executor &executor)
                : m_handle(std::move(make_handle<uvw::AsyncHandle>(executor)))
                , m_socket(m_handle)
        {
        }

        Publisher<T> &publisher() { return m_socket.publisher; }

        Subscriber<T> &subscriber() { return m_socket.subscriber; }

        void reset() { m_handle->clear(); }

        ~EventEmitter() = default;
    };

    template<typename T>
    using PromiseResolver = std::function<void(std::function<void(T)>)>;

    template<typename T>
    class Promise final {
        VX_DEFAULT_MOVABLE(Promise);
        VX_MAKE_NONCOPYABLE(Promise);

    private:
        std::shared_ptr<Executor> m_executor;
        std::shared_ptr<uvw::AsyncHandle> m_handle;
        Socket<T> m_socket;

    public:
        Promise(std::shared_ptr<Executor> executor, std::shared_ptr<uvw::AsyncHandle> handle, const Socket<T> &socket)
                : m_executor(std::move(executor))
                , m_handle(std::move(handle))
                , m_socket(socket)
        {
        }

        Promise(std::shared_ptr<Executor> executor, PromiseResolver<T> resolver)
                : m_executor(std::move(executor))
                , m_handle(std::move(make_handle<uvw::AsyncHandle>(*m_executor)))
                , m_socket(m_handle)
        {
            auto cb = [publisher_clone = m_socket.publisher](T value) {
                publisher_clone.publish(std::move(value));
            };

            resolver(std::move(cb));
        }

        ~Promise() = default;

        template<typename TNew>
        Promise<TNew> map(std::function<TNew(T &)> fn)
        {
            auto handle = make_handle<uvw::AsyncHandle>(*m_executor);
            auto socket = Socket<TNew>(handle);
            auto closable = finally([=, handle_clone = m_handle](T &value) {
                auto mapped_value = fn(value);
                socket.publisher.publish(std::move(mapped_value));

                // Hold onto the handle from this promise so it doesn't get
                // closed if we get destructed (which would happen if you
                // chained .map calls together_
                (void) handle_clone;
            });

            // TODO: this probably isn't the right way to do this
            closable.erase();

            return Promise<TNew>(m_executor, handle, socket);
        }

        Closable finally(std::function<void(T &)> fn)
        {
            auto closable = m_socket.subscriber.once([=, handle = m_handle](T &value) {
                fn(value);
                if (handle && handle->active())
                    handle->close();
            });

            return std::move(closable);
        }

        friend class Executor;
    };

    class EventLoop final {
    private:
        std::shared_ptr<uvw::Loop> m_loop;
        std::shared_ptr<Executor> m_executor;

        explicit EventLoop(std::shared_ptr<uvw::Loop> internal)
                : m_loop(std::move(internal))
                , m_executor(new Executor(m_loop))
        {
        }

    public:
        EventLoop()
                : EventLoop(uvw::Loop::create())
        {
        }

        ~EventLoop() = default;

        void run() { m_loop->run(); }

        void close()
        {
            m_loop->walk(uvw::Overloaded{
                    [](uvw::TimerHandle &h) { h.close(); }, // Kill timers
                    [](uvw::AsyncHandle &h) { h.close(); }, // Kill event listeners
                    [](auto &&) {
                        // TODO: Is this assert correct? We should probably let file system handles complete
                        VX_ASSERT(false, "Event loop not able to close all resources. Program will probably hang...");
                    },
            });

            m_loop->close();
        }

        static void run_scoped(const std::function<std::vector<Closable>(std::shared_ptr<Executor>)> &block)
        {
            EventLoop e;
            auto promises = block(e.executor());
            e.run();
        }

        static void run_scoped(const std::function<Closable(std::shared_ptr<Executor>)> &block)
        {
            EventLoop e;
            auto promise = block(e.executor());
            e.run();
        }

        std::shared_ptr<Executor> executor() { return m_executor; }
    };

    // Impl functions now that everything is declared

    template<typename T>
    Socket<T>::Socket(Executor &e)
            : Socket<T>(std::move(make_handle<uvw::AsyncHandle>(e)))
    {
    }

    template<typename T>
    Socket<T>::Socket(std::shared_ptr<uvw::AsyncHandle> handle)
            : Socket<T>(std::move(Socket<T>(Publisher<T>(handle), Subscriber<T>(handle))))
    {
    }

    namespace Time {
        class Timer final {
            VX_DEFAULT_MOVABLE(Timer);
            VX_MAKE_NONCOPYABLE(Timer);

        public:
            using EventType = bool;

        private:
            bool m_running;
            Millis m_timeout;
            Millis m_repeat;
            EventEmitter<EventType> m_emitter;
            std::shared_ptr<Executor> m_executor;
            std::shared_ptr<uvw::TimerHandle> m_timer_handle;

        public:

            Timer(std::shared_ptr<Executor> executor, Millis timeout, Millis repeat, bool autostart)
                    : m_emitter(*executor)
                    , m_timeout(timeout)
                    , m_repeat(repeat)
                    , m_running(autostart)
                    , m_executor(std::move(executor))
            {
                if (m_running)
                    start();
            };

            void start()
            {
                if (!m_running || m_timer_handle == nullptr)
                {
                    m_timer_handle = std::move(make_handle<uvw::TimerHandle>(*m_executor));

                    // Can take a reference capture to 'this' because if we get deleted we'll
                    // end up cancelling the timer
                    m_timer_handle->on<uvw::TimerEvent>([&](const uvw::TimerEvent &, const uvw::TimerHandle &) {
                        m_running = false;
                        m_emitter.publisher().publish({});
                    });

                    m_timer_handle->start(m_timeout, m_repeat);
                }
            }

            void stop()
            {
                if (m_timer_handle != nullptr) {
                    m_timer_handle->stop();
                }
            }

            void reset()
            {
                if (m_running && m_timer_handle != nullptr) {
                    m_timer_handle->clear();
                    m_timer_handle->close();
                    m_timer_handle.reset();
                    m_running = false;
                }
            }

            Subscriber<EventType> &subscriber() { return m_emitter.subscriber(); }

            [[nodiscard]] bool running() const { return m_running; }

            ~Timer()
            {
                if (m_timer_handle->active())
                    m_timer_handle->stop();
            }
        };

        inline Timer start_timeout(std::shared_ptr<Executor> executor, Millis timeout);
        inline Timer start_interval(std::shared_ptr<Executor> executor, Millis interval);
    }

    namespace FileSystem {
        using ReadResult = VX::Expected<VX::ByteBuffer>;

        inline Promise<ReadResult> read_file(std::shared_ptr<Executor> executor, const std::filesystem::path &path, size_t offset, size_t length)
        {
            auto read_handle = make_handle<uvw::FileReq>(*executor);
            PromiseResolver<ReadResult> resolver = [=](auto cb) {
                read_handle->template once<uvw::FsEvent<uvw::FileReq::Type::READ>>([=](auto &read, auto &) {
                    auto raw_buffer = std::make_unique<std::byte[]>(read.size);
                    memcpy(&raw_buffer[0], read.data.get(), read.size);
                    VX::ByteBuffer bb(std::move(raw_buffer), read.size);
                    cb(std::move(bb));
                    read_handle->close();
                });

                read_handle->template once<uvw::ErrorEvent>([=](const auto &error, const auto &) {
                    auto result = VX::make_unexpected<VX::ByteBuffer>(error.what());
                    cb(std::move(result));
                    read_handle->close();
                });
            };

            // Trigger the read after a successful open
            read_handle->once<uvw::FsEvent<uvw::FileReq::Type::OPEN>>([=](const auto &, auto &handle_ref) {
                handle_ref.read(offset, length);
            });

            Logger::Core->debug("Attempting to read file: {} (offset={} size={})", path.string(), offset, length);
            read_handle->open(path, 0, 0);

            return {std::move(executor), std::move(resolver)};
        }
    }
}
