//
// Created by voltairepunk on 2019-12-30.
//

#include "Task.hpp"
#include "../log/Logger.hpp"

namespace Romuva {
    namespace Tasks {

        WrappedTask::WrappedTask() {}

        WrappedTask::WrappedTask(Task t, bool repeating, bool background, unsigned short priority) :
        _unwrappedTask(std::move(t)),
        _isRepeating(repeating),
        _isBackground(background),
        _priority(priority) {}

        void WrappedTask::operator()() const {
            try {
                this->_unwrappedTask();
            } catch (const std::exception& ex) {
                gLogError << "Exception with wrapped task execution: " << ex.what();
            } catch (...) {
                gLogError << "An unexpected error has occured while executing task";
            }
        }

        bool WrappedTask::isRepeating() const {
            return this->_isRepeating;
        }

        bool WrappedTask::isBackground() const {
            return this->_isBackground;
        }

        void WrappedTask::setPriority(unsigned short priority) {
            this->_priority = priority;
        }

        unsigned short WrappedTask::getPriority() const {
            return this->_priority;
        }

        void WrappedTask::setRepeating(bool enabled) {
            this->_isRepeating = enabled;
        }

        void WrappedTask::setBackground(bool enabled) {
            this->_isBackground = enabled;
        }

        bool CompareWrappedTasks::operator()(const WrappedTask &t1, const WrappedTask &t2) const {
            return t1._priority > t2._priority;
        }

        WrappedTask make_wrapped(Task task, bool repeating, bool background, unsigned short priority) {
            return WrappedTask {std::move(task), repeating, background, priority};
        }
    }
}