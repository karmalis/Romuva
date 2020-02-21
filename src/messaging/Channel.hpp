//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_CHANNEL_HPP
#define ROMUVA_CHANNEL_HPP

#include "ChannelQueue.hpp"

namespace Romuva {

    namespace Messaging {

        class Channel {
        public:

            template <typename tEvent, class tHandler>
            static void add(tHandler* handler);

            template <typename tEvent, class tHandler>
            static void remove(tHandler* handler);

            template <typename tEvent>
            static void broadcast(const tEvent& message);
        };

        template <typename tMessage>
        class MessageHandler
        {
        public:

            MessageHandler();
            virtual ~MessageHandler();

            virtual void operator()(const tMessage&) = 0;
        };

        template <typename tEvent, class tHandler>
        void Channel::add(tHandler* handler)
        {
            ChannelQueue<tEvent>::instance().add(handler);
        }

        template <typename tEvent, class tHandler>
        void Channel::remove(tHandler* handler)
        {
            ChannelQueue<tEvent>::instance().remove(handler);
        }

        template <typename tEvent>
        void Channel::broadcast(const tEvent& message)
        {
            ChannelQueue<tEvent>::instance().broadcast(message);
        }

        template <typename T>
        MessageHandler<T>::MessageHandler() {
            Channel::add<T>(this);
        }

        template <typename T>
        MessageHandler<T>::~MessageHandler() {
            Channel::remove<T>(this);
        }

    }

}



#endif //ROMUVA_CHANNEL_HPP
