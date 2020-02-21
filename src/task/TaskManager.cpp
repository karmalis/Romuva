//
// Created by voltairepunk on 2019-12-30.
//

#include "TaskManager.hpp"
#include "../log/Logger.hpp"

namespace Romuva {

    namespace Tasks {

        TaskManager::TaskManager(size_t numWorkerrs) :
                _isRunning(false),
                _stopping(false) {
            if (numWorkerrs <= 0) {
                _numWorkers = std::thread::hardware_concurrency() + 1;
            } else {
                _numWorkers = numWorkerrs;
            }
        }

        void TaskManager::addWork(Task t, bool repeating, bool background) {
            this->addWork(make_wrapped(std::move(t), repeating, background));
        }

        void TaskManager::addRepeatingWork(Task t, bool background) {
            this->addWork(make_wrapped(std::move(t), true, background));
        }

        void TaskManager::addBackgroundWork(Task t, bool repeating) {
            this->addWork(make_wrapped(std::move(t), repeating, true));
        }

        void TaskManager::addRepeatingBackgroundWork(Task t) {
            this->addWork(make_wrapped(std::move(t), true, true));
        }

        void TaskManager::start() {
            if (this->_isRunning.load()) return;

            this->_isRunning = true;

            for (size_t i = 0; i < this->_numWorkers; i++) {
                this->_threads.push_back(
                        new std::thread([this] {
                            while (_isRunning.load()) {
                                WrappedTask t;
                                if (_backgroundTasks.tryPop(t)) {
                                    execute(t);
                                }
                            }
                        }));
            }

            while (this->_isRunning.load()) {
                TaskQueue localQueue;
                this->_mainTasks.swap(localQueue);
                while (!localQueue.empty()) {
                    this->execute(localQueue.pop());
                }
            }

            while (this->_stopping.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        void TaskManager::stop() {
            this->_stopping.store(true);
            this->_isRunning.store(false);

            for (const auto& it : this->_threads) {
                it->join();
            }

            this->_stopping.store(false);
        }

        void TaskManager::addWork(WrappedTask t) {
            if (t.isBackground()) {
                this->_backgroundTasks.push(std::move(t));
            } else {
                this->_mainTasks.push(std::move(t));
            }
        }

        void TaskManager::execute(WrappedTask t) {
            t();

            if (t.isRepeating()) {
                this->addWork(std::move(t));
            }
        }


    }

}
