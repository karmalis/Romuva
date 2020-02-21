//
// Created by voltairepunk on 2019-12-31.
//

#include "LogMessage.hpp"
#include "Logger.hpp"

namespace Romuva {

    namespace Core {

        LogMessage::LogMessage(eLogLevel level, const std::string &file, int line, Logger *owner) : _owner(owner) {
            this->_meta = {
                    level, file, line
            };
        }

        LogMessage::~LogMessage() {
            if (this->_owner && logLevel(this->_meta.level)) {
                this->_owner->flush(*this);
            }
        }

        LogMessage::LogMessage(LogMessage &&message) :
                _meta(std::move(message._meta)),
                _owner(std::move(message._owner)),
                _buffer(std::move(message._buffer)) {
            message._owner = nullptr;
        }

        LogMessage &LogMessage::operator<<(std::ostream &(*fn)(std::ostream &)) {
            fn(this->_buffer);
            return * this;
        }


    }

}
