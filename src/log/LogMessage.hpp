//
// Created by voltairepunk on 2019-12-31.
//

#ifndef ROMUVA_LOGMESSAGE_HPP
#define ROMUVA_LOGMESSAGE_HPP

#include <string>
#include <sstream>
#include <ostream>

#include "LogLevel.hpp"

namespace Romuva {

    namespace Core {

        class Logger;
        class LogSink;

        class LogMessage {
        private:

            friend class Logger;
            friend class LogSink;

            LogMessage(
                    eLogLevel level,
                    const std::string& file,
                    int line,
                    Logger* owner
                    );

        public:

            ~LogMessage();

            LogMessage(LogMessage&& message);

            template <typename T>
            LogMessage& operator<<(const T& message);

            LogMessage& operator<<(std::ostream& (*fn)(std::ostream& os));

            struct Meta {
                eLogLevel level;
                std::string file;
                int line;
            };

        protected:

            Meta _meta;
            Logger* _owner;
            std::ostringstream _buffer;
        };

        template <typename T>
        LogMessage& LogMessage::operator<<(const T& message) {
            this->_buffer << message;
            return * this;
        }

    }

}


#endif //ROMUVA_LOGMESSAGE_HPP
