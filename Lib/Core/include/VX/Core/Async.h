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

// A quick and dirty A+ promise-like system for wrapping async
// operations on the event loop. Is currently not completely memory safe and
// not optimal in performance.

namespace VX::Core {
    namespace {

        class Executor;
        class EventLoop;
        template<typename T> class EventEmitter;
        template<typename T> class Promise;
        template<typename T> class Publisher;
        template<typename T> class Socket;
        template<typename T> class Subscriber;

        using Millis = std::chrono::duration<uint64_t, std::milli>;

        class Executor final {
            VX_DEFAULT_MOVABLE(Executor);
            VX_DEFAULT_COPYABLE(Executor);

        private:
            std::shared_ptr<uvw::Loop> m_EventLoop;

            explicit Executor(std::shared_ptr<uvw::Loop> event_loop)
                    : m_EventLoop(std::move(event_loop)) {}

        public:
            ~Executor() = default;

            friend class EventLoop;

            template <typename T>
            friend std::shared_ptr<T> MakeHandle(Executor&);
        };

        template<typename T>
        std::shared_ptr<T> MakeHandle(Executor &e) {
            VX_ASSERT(e.m_EventLoop != nullptr, "The underlying event loop is gone");
            return e.m_EventLoop->resource<T>();
        }

        template<typename T>
        class Subscriber final {
            VX_DEFAULT_MOVABLE(Subscriber);
            VX_DEFAULT_COPYABLE(Subscriber);

        private:
            std::shared_ptr<uvw::AsyncHandle> m_Handle;

        public:
            explicit Subscriber(std::shared_ptr<uvw::AsyncHandle> handle)
                    : m_Handle(std::move(handle))
            {
            }

            ~Subscriber() = default;

            Closable On(std::function<void(T&)> callback) {
                auto connection = m_Handle->on<uvw::AsyncEvent>([cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                    cb(*h.data<T>());
                });

                Closable c([=]() {
                    if (m_Handle && m_Handle->active())
                        m_Handle->erase(connection);
                });

                return c;
            }

            Closable Once(std::function<void(T&)> callback) {
                auto connection = m_Handle->once<uvw::AsyncEvent>([cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                    cb(*h.data<T>());
                });

                Closable c([=]() {
                    if (m_Handle && m_Handle->active())
                        m_Handle->erase(connection);
                });

                return c;
            }

            friend class Executor;
        };

        template<typename T>
        class Publisher final {
            VX_DEFAULT_MOVABLE(Publisher);
            VX_DEFAULT_COPYABLE(Publisher);

        private:
            std::shared_ptr<uvw::AsyncHandle> m_Handle;

        public:
            explicit Publisher(std::shared_ptr<uvw::AsyncHandle> handle)
                : m_Handle(std::move(handle))
            {}

            ~Publisher() = default;

            void Publish(T message) const {
                auto message_data = std::make_shared<T>(std::move(message));
                m_Handle->data(std::move(message_data));
                m_Handle->send();
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
            {}

            Socket(const Publisher<T> &publisher, const Subscriber<T> &subscriber)
                    : publisher(publisher)
                    , subscriber(subscriber)
            {}

            explicit Socket(Executor &e);
            explicit Socket(std::shared_ptr<uvw::AsyncHandle>);
        };

        template<typename T>
        class EventEmitter final {
            VX_DEFAULT_MOVABLE(EventEmitter);
            VX_DEFAULT_COPYABLE(EventEmitter);
        private:
            std::shared_ptr<uvw::AsyncHandle> m_Handle;
            Socket<T> m_Socket;

        public:
            explicit EventEmitter(Executor &executor)
                    : m_Handle(std::move(MakeHandle<uvw::AsyncHandle>(executor)))
                    , m_Socket(m_Handle) {
            }

            Publisher<T> &GetPublisher() { return m_Socket.publisher; }
            Subscriber<T> &GetSubscriber() { return m_Socket.subscriber; }

            void Reset() { m_Handle->clear(); }

            ~EventEmitter() = default;
        };

        template<typename T>
        using PromiseResolver = std::function<void(std::function<void(T)>)>;

        template<typename T>
        class Promise final {
            VX_DEFAULT_MOVABLE(Promise);
            VX_MAKE_NONCOPYABLE(Promise);

        private:
            std::shared_ptr<uvw::AsyncHandle> m_Handle;
            Socket<T> m_Socket;

        public:
            Promise(Executor &executor, PromiseResolver<T> resolver)
                    : m_Handle(std::move(MakeHandle<uvw::AsyncHandle>(executor)))
                    , m_Socket(m_Handle)
            {
                auto cb = [publisher_clone = m_Socket.publisher](T value) {
                    publisher_clone.Publish(std::move(value));
                };

                resolver(std::move(cb));
            }

            ~Promise() = default;

            Closable Finally(std::function<void(T&)> fn) {
                auto closable = m_Socket.subscriber.Once([=](T& value) {
                    fn(value);
                    m_Handle->close();
                });

                return std::move(closable);
            }

            friend class Executor;
        };

        template <typename T>
        class Promise2 final {
        private:
            std::shared_ptr<Executor> m_Executor;
        };

        class EventLoop final {
        private:
            std::shared_ptr<uvw::Loop> m_Loop;
            std::shared_ptr<Executor> m_Executor;

            explicit EventLoop(std::shared_ptr<uvw::Loop> internal)
                    : m_Loop(std::move(internal))
                    , m_Executor(new Executor(m_Loop))
            {
            }

        public:
            EventLoop()
                    : EventLoop(uvw::Loop::create())
            {}

            ~EventLoop() = default;

            void Run() {
                m_Loop->run();
            }

            void Close() {
                m_Loop->walk(uvw::Overloaded{
                        [](uvw::TimerHandle &h) { h.close(); }, // Kill timers
                        [](uvw::AsyncHandle &h) { h.close(); }, // Kill event listeners
                        [](auto &&) {
                            // TODO: Is this assert correct? We should probably let file system handles complete
                            VX_ASSERT(false, "Event loop not able to close all resources. Program will probably hand...");
                        },
                });

                m_Loop->close();
            }

            static void RunScoped(const std::function<std::vector<Closable>(std::shared_ptr<Executor>)>& block) {
                EventLoop e;
                auto promises = block(e.GetExecutor());
                e.Run();
            }

            std::shared_ptr<Executor> GetExecutor() { return m_Executor; }
        };

        // Impl functions now that everything is declared

        template<typename T>
        Socket<T>::Socket(Executor &e)
            : Socket<T>(std::move(MakeHandle<uvw::AsyncHandle>(e)))
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
                bool m_Running;
                Millis m_Timeout;
                Millis m_Repeat;
                EventEmitter<EventType> m_Emitter;
                std::shared_ptr<Executor> m_Executor;
                std::shared_ptr<uvw::TimerHandle> m_TimerHandle;

            public:

                Timer(std::shared_ptr<Executor> executor, Millis timeout, Millis repeat, bool autostart)
                        : m_Emitter(*executor)
                        , m_Timeout(timeout)
                        , m_Repeat(repeat)
                        , m_Running(autostart)
                        , m_Executor(std::move(executor))
                {
                    if (m_Running)
                        Start();
                };

                void Start() {
                    if (!m_Running || m_TimerHandle == nullptr) {
                        m_TimerHandle = std::move(MakeHandle<uvw::TimerHandle>(*m_Executor));

                        // Can take a reference capture to 'this' because if we get deleted we'll
                        // end up cancelling the timer
                        m_TimerHandle->on<uvw::TimerEvent>([&](const uvw::TimerEvent &, const uvw::TimerHandle &) {
                            m_Running = false;
                            m_Emitter.GetPublisher().Publish({});
                        });

                        m_TimerHandle->start(m_Timeout, m_Repeat);
                    }
                }

                void Stop() {
                    if (m_TimerHandle != nullptr) {
                        m_TimerHandle->stop();
                    }
                }

                void Reset() {
                    if (m_Running && m_TimerHandle != nullptr) {
                        m_TimerHandle->clear();
                        m_TimerHandle->close();
                        m_TimerHandle.reset();
                        m_Running = false;
                    }
                }

                Subscriber<EventType> &GetReceiver() { return m_Emitter.GetSubscriber(); }

                [[nodiscard]] bool IsRunning() const { return m_Running; }

                ~Timer() = default;
            };

            Timer StartTimeout(std::shared_ptr<Executor> executor, Millis timeout);
            Timer StartInterval(std::shared_ptr<Executor> executor, Millis interval);
        }

        namespace FileSystem {
            using ReadResult = VX::Expected<VX::ByteBuffer>;
            Promise<ReadResult> ReadFile(Executor &executor, const std::filesystem::path &path, size_t offset, size_t length) {
                auto read_handle = MakeHandle<uvw::FileReq>(executor);
                PromiseResolver<ReadResult> resolver = [=](auto cb) {
                    read_handle->template once<uvw::FsEvent<uvw::FileReq::Type::READ>>([=](auto &read, auto&) {
                        auto raw_buffer = std::make_unique<std::byte[]>(read.size);
                        memcpy(&raw_buffer[0], read.data.get(), read.size);
                        VX::ByteBuffer bb(std::move(raw_buffer), read.size);
                        cb(std::move(bb));
                        read_handle->close();
                    });

                    read_handle->template once<uvw::ErrorEvent>([=](const auto &error, const auto&) {
                        auto result = VX::MakeUnexpected<VX::ByteBuffer>(error.what());
                        cb(std::move(result));
                        read_handle->close();
                    });
                };

                // Trigger the read after a successful open
                read_handle->once<uvw::FsEvent<uvw::FileReq::Type::OPEN>>([=](const auto&, auto& handle_ref){
                    handle_ref.read(offset, length);
                });

                read_handle->open(path, 0, 0);

                return { executor, std::move(resolver) };
            }
        }
    }
}
