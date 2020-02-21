//
// Created by voltairepunk on 2019-12-31.
//

#include "LogLevel.hpp"

#include <atomic>

namespace Romuva {

    namespace Core {

        struct LogLevelState {
            std::atomic_bool states[5] = {};

            LogLevelState() {
                for (auto& st : states) {
                    st.store(true);
                }
            }
        };

        LogLevelState logLevelState;

        int selectLogState(eLogLevel level)
        {
            switch(level)
            {
                case eLogLevel::E_DEBUG: return 0;
                case eLogLevel::E_MESSAGE: return 1;
                case eLogLevel::E_WARNING: return 2;
                case eLogLevel::E_ERROR: return 3;
                case eLogLevel::E_FATAL: return 4;
                default: throw std::runtime_error("Unsupported Log Level");
            }
        }

        void setLogLevel(eLogLevel level, bool enabled) {
            int lvl = selectLogState(level);
            logLevelState.states[lvl].store(enabled);
        }

        bool logLevel(eLogLevel level) {
            int lvl = selectLogState(level);
            return logLevelState.states[lvl].load();
        }

    }

}

std::ostream& operator << (std::ostream& os, const Romuva::Core::eLogLevel& level) {
    using Romuva::Core::eLogLevel;

    switch (level) {
        case eLogLevel::E_DEBUG: os << " [ DEBUG ] "; break;
        case eLogLevel::E_MESSAGE: os << "           "; break;
        case eLogLevel::E_WARNING: os << " [ WARNING ] "; break;
        case eLogLevel::E_ERROR: os << " < ERROR > "; break;
        case eLogLevel::E_FATAL: os << " < FATAL > "; break;
        default: os << " ";
    }

    return os;
}

