//
// Created by voltairepunk on 2019-12-31.
//

#include "Active.hpp"

namespace Romuva {

    namespace Core {

        void ActiveThreadExecutor::operator()(Active *active) {
            while (!active->_isDone) {
                Callback fn;
                if (active->tryPopMessageQueue(fn)) {
                    fn();
                }
            }
        }

        bool Active::tryPopMessageQueue(Callback &fn) {
            const std::lock_guard<std::mutex> lock(this->_messageQueueMutex);
            if (this->_messageQueue.empty()) return false;

            fn = this->_messageQueue.front();
            this->_messageQueue.pop();
            return true;
        }

        Active::Active() : _isDone(false), _executor(), _thread(nullptr) {

        }

        Active::~Active() {
            send([this]{ _isDone = true; });
            this->_thread->join();
            delete this->_thread;
        }

        std::unique_ptr<Active> Active::create() {
            std::unique_ptr<Active> result(new Active);
            result->_thread = new std::thread(result->_executor, result.get());
            return result;
        }

        void Active::send(Callback message) {
            const std::lock_guard<std::mutex> lock(this->_messageQueueMutex);
            this->_messageQueue.push(std::move(message));
        }


    }

}
