//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_CHANNELQUEUE_HPP
#define ROMUVA_CHANNELQUEUE_HPP

#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>


namespace Romuva {

    namespace Messaging {

        class Channel;

        template <typename tMessage>
        class ChannelQueue
        {
        public:

            using Handler = std::function<void(const tMessage&)>;
            using ScopedLock = std::lock_guard<std::mutex>;

            static ChannelQueue& instance();

            template <typename tHandler> void add(tHandler* handler);
            template <typename tHandler> void remove(tHandler* handler);

            void broadcast(const tMessage& message);

        protected:

            ChannelQueue();

            template <typename tHandler> Handler createHandler(tHandler * handler);

            std::mutex _mutex;
            std::vector<Handler> _handlers;
            std::vector<void*> _originalPtrs;
        };

        template<typename tMessage>
        ChannelQueue<tMessage>& ChannelQueue<tMessage>::instance() {
            static ChannelQueue instance;
            return instance;
        }

        template <typename tMessage>
        template <typename tHandler>
        void ChannelQueue<tMessage>::add(tHandler* handler) {
            ScopedLock lock(this->_mutex);
            this->_handlers.push_back(this->createHandler(handler));
            this->_originalPtrs.push_back(handler);
        }

        template <typename tMessage>
        template <typename tHandler>
        void ChannelQueue<tMessage>::remove(tHandler* handler)
        {
            ScopedLock lock(this->_mutex);

            auto it = std::find(this->_originalPtrs.begin(), this->_originalPtrs.end(), handler);

            if (it == this->_originalPtrs.end())
                throw std::runtime_error("Tried to remove a handler that is not in the list");

            auto idx = (it - this->_originalPtrs.begin());

            this->_handlers.erase(this->_handlers.begin() + idx);
            this->_originalPtrs.erase(it);
        }

        template <typename tMessage>
        void ChannelQueue<tMessage>::broadcast(const tMessage& message)
        {
            std::vector<Handler> localVector(this->_handlers.size());

            {
                ScopedLock lock(this->_mutex);
                localVector = this->_handlers;
            }

            for (const auto& handler : localVector)
            {
                handler(message);
            }
        }

        template<typename tMessage>
        ChannelQueue<tMessage>::ChannelQueue() = default;

        template <typename tMessage>
        template <typename tHandler>
        typename ChannelQueue<tMessage>::Handler ChannelQueue<tMessage>::createHandler(tHandler* handler)
        {
            return [handler](const tMessage& message) { (*handler)(message); };
        }

    }

}


#endif //ROMUVA_CHANNELQUEUE_HPP
