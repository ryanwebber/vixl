#pragma once

#include <concepts>

#include <uvw/async.h>
#include <uvw/loop.h>
#include <uvw/timer.h>

#include <Common/Copyable.h>
#include <Common/Noncopyable.h>

#include <Core/Assert.h>
#include <Core/Closable.h>

// A quick and dirty A+ promise-like system for wrapping async
// operations on the event loop. Is currently not completely memory safe and
// not optimal in performance.

namespace Core::Async {
    namespace {

        class Executor;

        class Timer;

        template<typename T>
        class Publisher;

        template<typename T>
        class Subscriber;

        template<typename T>
        class Socket;

        template<typename T>
        class EventEmitter;

        using Millis = std::chrono::duration<uint64_t, std::milli>;

        class EventLoop final {
        private:
            std::shared_ptr<Executor> m_Executor;
            std::shared_ptr<uvw::Loop> m_Loop;

            explicit EventLoop(std::shared_ptr<uvw::Loop> internal);

        public:
            EventLoop()
                    : EventLoop(uvw::Loop::create()) {}

            ~EventLoop() = default;

            void Run() { m_Loop->run(); }

            void Close() {
                m_Loop->walk(uvw::Overloaded{
                        [](uvw::TimerHandle &h) { h.close(); }, // Kill timers
                        [](uvw::AsyncHandle &h) { h.close(); }, // Kill event listeners
                        [](auto &&) {
                            VX_ASSERT(false,
                                      "Event loop not able to close all resources. Program will probably hand...");
                        },
                });

                m_Loop->close();
            }

            std::shared_ptr<Executor> GetExecutor() { return m_Executor; }

            Executor &GetExecutorRef() { return *m_Executor; }
        };

        class Executor final : public std::enable_shared_from_this<Executor> {
            VX_DEFAULT_MOVABLE(Executor);
            VX_DEFAULT_COPYABLE(Executor);

        private:
            std::shared_ptr<uvw::Loop> m_EventLoop;

            explicit Executor(std::shared_ptr<uvw::Loop> event_loop)
                    : m_EventLoop(std::move(event_loop)) {}

        public:
            ~Executor() = default;

            uvw::Loop &GetRawLoop() { return *m_EventLoop; }

            template<typename T>
            Socket<T> CreateSocket();

            template<typename T>
            Socket<T> CreateSocket(std::shared_ptr<uvw::AsyncHandle>);

            template<typename T>
            EventEmitter<T> CreateEventEmitter();

            Timer CreateTimer(Millis timeout, Millis repeat, bool autostart);

            friend class EventLoop;
        };

        template<typename T>
        class Subscriber final {
            VX_DEFAULT_MOVABLE(Subscriber);
            VX_DEFAULT_COPYABLE(Subscriber);

        private:
            std::shared_ptr<uvw::AsyncHandle> m_Handle;

        public:
            explicit Subscriber(std::shared_ptr<uvw::AsyncHandle> handle)
                    : m_Handle(std::move(handle)) {}

            ~Subscriber() = default;

            Closable On(std::function<void(T)> callback) {
                auto connection = m_Handle->on<uvw::AsyncEvent>([cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                    cb(*h.data<T>());
                });

                Closable c([=]() { m_Handle->erase(connection); });
                return std::move(c);
            }

            Closable Once(std::function<void(T)> callback) {
                auto connection = m_Handle->once(
                        [cb = std::move(callback)](const uvw::AsyncEvent &e, const uvw::AsyncHandle &h) {
                            cb(*h.data<T>());
                        });

                Closable c([=]() { m_Handle->erase(connection); });
                return std::move(c);
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
                    : m_Handle(std::move(handle)) {}

            ~Publisher() = default;

            void Publish(T message) {
                auto message_data = std::make_shared<T>(message);
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
                    : publisher(std::move(publisher)), subscriber(std::move(subscriber)) {}

            Socket(const Publisher<T> &publisher, const Subscriber<T> &subscriber)
                    : publisher(publisher), subscriber(subscriber) {}

            explicit Socket(Executor &e);
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
                    : m_Handle(std::move(executor.GetRawLoop().resource<uvw::AsyncHandle>())),
                      m_Socket(std::move(executor.template CreateSocket<T>(m_Handle))) {
            }

            Publisher<T> &GetPublisher() { return m_Socket.publisher; }

            Subscriber<T> &GetSubscriber() { return m_Socket.subscriber; }

            void Reset() { m_Handle->clear(); }

            ~EventEmitter() = default;
        };

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
                    : m_Emitter(executor->CreateEventEmitter<EventType>()), m_Timeout(timeout), m_Repeat(repeat),
                      m_Running(autostart), m_Executor(std::move(executor)) {
                if (m_Running)
                    Start();
            };

            void Start() {
                if (!m_Running || m_TimerHandle == nullptr) {
                    m_TimerHandle = m_Executor->GetRawLoop().resource<uvw::TimerHandle>();

                    // Can take a reference capture to 'this' because if we get deleted we'll
                    // end up cancelling the timer
                    m_TimerHandle->on<uvw::TimerEvent>([&](const uvw::TimerEvent &, const uvw::TimerHandle &) {
                        m_Running = false;
                        m_Emitter.GetPublisher().Publish({});
                    });

                    m_TimerHandle->start(m_Timeout, m_Repeat);
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

        template<typename T>
        using PromiseResolver = std::function<void(std::function<void(T)>)>;

        template<typename T>
        class Promise final {
            VX_DEFAULT_MOVABLE(Promise);
            VX_MAKE_NONCOPYABLE(Promise);

        private:
            std::shared_ptr<Executor> m_Executor;
            std::vector<Closable> m_Connections;

            // Socket.Publisher is called to resolve this promise
            // Socket.Subscriber will be called when this promise is resolved
            Socket<T> m_Socket;

        public:
            Promise(std::shared_ptr<Executor> executor, Socket<T> &&socket)
                    : m_Executor(std::move(executor)), m_Socket(std::move(socket)) {}

            Promise(std::shared_ptr<Executor> executor, PromiseResolver<T> resolver)
                    : m_Executor(std::move(executor)), m_Socket(*executor) {
                auto cb = [publisher_clone = m_Socket.publisher](T value) {
                    publisher_clone.Publish(value);
                };

                resolver(std::move(cb));
            }

            ~Promise() = default;

            template<typename TNew>
            Promise<TNew> Then(std::function<TNew(T)> map_fn) {
                auto new_socket = m_Executor->CreateSocket<TNew>();
                auto closable = m_Socket.subscriber.Once(
                        [publisher = new_socket.publisher, mapper = std::move(map_fn)](T value) {
                            auto mapped_value = mapper(value);
                            publisher.Publish(mapped_value);
                        });

                m_Connections.push_back(std::move(closable));

                return Promise<TNew>(m_Executor, std::move(new_socket));
            }

            friend class Executor;
        };

        // Impl functions now that everything is declared

        EventLoop::EventLoop(std::shared_ptr<uvw::Loop> internal)
                : m_Executor(nullptr), m_Loop(std::move(internal)) {
            auto ptr = new Executor(m_Loop);
            m_Executor = std::shared_ptr<Executor>(ptr);
        }

        template<typename T>
        Socket<T>::Socket(Executor &e)
                : Socket<T>(std::move(e.template CreateSocket<T>())) {
        }

        template<typename T>
        Socket<T> Executor::CreateSocket(std::shared_ptr<uvw::AsyncHandle> handle) {
            Publisher<T> p(handle);
            Subscriber<T> s(handle);
            return std::move(Socket<T>(p, s));
        }

        template<typename T>
        Socket<T> Executor::CreateSocket() {
            return std::move(CreateSocket<T>(std::move(m_EventLoop->template resource<uvw::AsyncHandle>())));
        }

        template<typename T>
        EventEmitter<T> Executor::CreateEventEmitter() {
            return std::move(EventEmitter<T>(*this));
        }

        Timer Executor::CreateTimer(Millis timeout, Millis repeat, bool autostart) {
            auto self = shared_from_this();
            return std::move(Timer(std::move(self), timeout, repeat, autostart));
        }
    }
}
