//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_TASK_HPP
#define ROMUVA_TASK_HPP

#include <functional>

namespace Romuva {
    namespace Tasks {

        typedef std::function<void()> Task;

        class WrappedTask {
        public:

            explicit WrappedTask();

            explicit WrappedTask(Task t, bool repeating, bool background, unsigned short priority);

            void operator()() const;

            bool isRepeating() const;

            bool isBackground() const;

            void setPriority(unsigned short priority);

            unsigned short getPriority() const;

            friend class TaskProcessor;

            friend class CompareWrappedTasks;

        protected:

            void setRepeating(bool enabled);

            void setBackground(bool enabled);

            Task _unwrappedTask;
            bool _isRepeating;
            bool _isBackground;

            unsigned short _priority;
        };

        class CompareWrappedTasks {
        public:

            bool operator() (const WrappedTask& t1, const WrappedTask& t2) const;
        };

        WrappedTask make_wrapped(
                Task task,
                bool repeating = false,
                bool background = false,
                unsigned short priority = 10
                );

    }
}


#endif //ROMUVA_TASK_HPP
