//
// Created by voltairepunk on 2019-12-31.
//

#ifndef ROMUVA_ACTIVE_HPP
#define ROMUVA_ACTIVE_HPP

#include <memory>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>

namespace Romuva {

    namespace Core {

        typedef std::function<void()> Callback;

        class Active;

        struct ActiveThreadExecutor {
            void operator()(Active* active);
        };



        class Active {
        private:
            Active();

            std::mutex _messageQueueMutex;

        public:

            typedef std::queue<Callback> MessageQueue;

            Active(const Active&) = delete;
            Active& operator= (const Active&) = delete;

            ~Active();

            static std::unique_ptr<Active> create();

            void send(Callback message);

            bool tryPopMessageQueue(Callback& fn);

        protected:

            bool _isDone;

            MessageQueue _messageQueue;
            std::thread* _thread;

            ActiveThreadExecutor _executor;
            friend struct ActiveThreadExecutor;
        };

    }

}


#endif //ROMUVA_ACTIVE_HPP
