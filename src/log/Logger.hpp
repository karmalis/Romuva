//
// Created by voltairepunk on 2019-12-31.
//

#ifndef ROMUVA_LOGGER_HPP
#define ROMUVA_LOGGER_HPP

#include "LogLevel.hpp"
#include "LogMessage.hpp"
#include "../Active.hpp"

namespace Romuva {

    namespace Core {

        class LogSink;
        class LogMessage;

        class Logger {
        public:

            Logger(const std::string& filename);

            LogMessage operator() (
                    eLogLevel level,
                    const std::string& filename,
                    int line
            );

            void add(const LogSink& sink);
            void remove(const LogSink& sink);

            void flush(const LogMessage& message) const;

        protected:

            std::vector<LogSink> _sinks;
            std::unique_ptr<Active> _active;
        };

        extern Logger gLogger;

    }

}

#define gLogLevel(level) ::Romuva::Core::gLogger(\
		::Romuva::Core::eLogLevel::level, \
		 __FILE__, \
		 __LINE__ \
)

#define gLog			gLogLevel(E_MESSAGE)
#define gLogDebug		gLogLevel(E_DEBUG)
#define gLogMessage		gLogLevel(E_MESSAGE)
#define gLogError		gLogLevel(E_ERROR)
#define gLogWarning		gLogLevel(E_WARNING)
#define gLogFatal		gLogLevel(E_FATAL)


#endif //ROMUVA_LOGGER_HPP
