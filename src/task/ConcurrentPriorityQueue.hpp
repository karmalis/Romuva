//
// Created by voltairepunk on 2020-01-27.
//

#ifndef ROMUVA_CONCURRENTPRIORITYQUEUE_HPP
#define ROMUVA_CONCURRENTPRIORITYQUEUE_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Romuva {

    namespace Tasks {

        template <typename T, class Comparator>
        class ConcurrentPriorityQueue {
        public:

            ConcurrentPriorityQueue() = default;
            ConcurrentPriorityQueue(const ConcurrentPriorityQueue&) = delete;
            ConcurrentPriorityQueue& operator=(const ConcurrentPriorityQueue&) = delete;

            T pop();

            bool tryPop(T& item);

            void push(const T& item);
            void push(T&& item);

            bool empty();

            void swap(ConcurrentPriorityQueue& other);

        protected:

            std::priority_queue<T, std::vector<T>, Comparator> _queue;
            std::mutex _mutex;
            std::condition_variable _cond;
        };

        template<typename T, class Comparator>
        T ConcurrentPriorityQueue<T, Comparator>::pop() {
            std::unique_lock<std::mutex> mlock(_mutex);
            while (_queue.empty()) {
                _cond.wait(mlock);
            }
            auto val = _queue.top();
            _queue.pop();
            return val;
        }

        template<typename T, class Comparator>
        void ConcurrentPriorityQueue<T, Comparator>::push(const T &item) {
            std::unique_lock<std::mutex> mlock(_mutex);
            _queue.push(item);
            mlock.unlock();
            _cond.notify_one();
        }

        template<typename T, class Comparator>
        void ConcurrentPriorityQueue<T, Comparator>::push(T&& item) {
            std::unique_lock<std::mutex> mlock(_mutex);
            _queue.push(std::move(item));
            mlock.unlock();
            _cond.notify_one();
        }

        template<typename T, class Comparator>
        bool ConcurrentPriorityQueue<T, Comparator>::tryPop(T &item) {
            {
                std::unique_lock<std::mutex> mlock(_mutex);
                if (_queue.empty()) return false;
            }

            item = pop();
            return true;
        }

        template<typename T, class Comparator>
        bool ConcurrentPriorityQueue<T, Comparator>::empty() {
            std::unique_lock<std::mutex> mlock(_mutex);
            return _queue.empty();
        }

        template<typename T, class Comparator>
        void ConcurrentPriorityQueue<T, Comparator>::swap(ConcurrentPriorityQueue &other) {
            std::unique_lock<std::mutex> mlock(_mutex);
            _queue.swap(other._queue);
            mlock.unlock();
            _cond.notify_one();
        }

    }

}


#endif //ROMUVA_CONCURRENTPRIORITYQUEUE_HPP
