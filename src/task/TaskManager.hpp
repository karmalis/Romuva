//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_TASKMANAGER_HPP
#define ROMUVA_TASKMANAGER_HPP

#include <atomic>
#include <thread>
#include <list>

#include "Task.hpp"
#include "ConcurrentPriorityQueue.hpp"

namespace Romuva {
    namespace Tasks {
        class TaskManager {
        protected:

            using TaskQueue = ConcurrentPriorityQueue<WrappedTask, CompareWrappedTasks>;

        public:

            TaskManager(size_t numWorkerrs = 0);

            void addWork(Task t, bool repeating = false, bool background = false);

            void addRepeatingWork(Task t, bool background = false);
            void addBackgroundWork(Task t, bool repeating = false);
            void addRepeatingBackgroundWork(Task t);

            void start();
            void stop();

        protected:

            void addWork(WrappedTask t);
            void execute(WrappedTask t);

            TaskQueue _mainTasks;
            TaskQueue _backgroundTasks;

            std::atomic<bool> _isRunning;
            std::atomic<bool> _stopping;

            size_t _numWorkers;
            std::list<std::thread*> _threads;
        };
    }
}


#endif //ROMUVA_TASKMANAGER_HPP
