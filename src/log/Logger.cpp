//
// Created by voltairepunk on 2019-12-31.
//

#include "Logger.hpp"
#include "LogSink.hpp"

#include <algorithm>

namespace Romuva {

    namespace Core {

        Logger::Logger(const std::string &filename) {
            this->_active = Active::create();

            this->add(makeConsoleSink());
            this->add(makeFileSink(filename));
        }

        LogMessage Logger::operator()(eLogLevel level, const std::string &filename, int line) {
            return LogMessage(
                    level,
                    filename,
                    line,
                    this
            );
        }

        void Logger::add(const LogSink &sink) {
            this->_sinks.push_back(sink);
        }

        void Logger::remove(const LogSink &sink) {
            //auto it = std::find(this->_sinks.begin(), this->_sinks.end(), sink);
            auto it = std::find(std::begin(this->_sinks), std::end(this->_sinks), sink);
            if (it == this->_sinks.end()) {
                throw std::runtime_error("Tried to remove a sink that has not been added yet");
            }

            this->_sinks.erase(it);
        }

        void Logger::flush(const LogMessage &message) const {
            auto&& sinks = this->_sinks;
            auto&& meta = message._meta;
            auto msg = message._buffer.str();

            this->_active->send([=]{
                for (auto&& sink : sinks) {
                    sink.forward(meta, msg);
                }
            });
        }

        Logger gLogger("log.txt");

    }

}