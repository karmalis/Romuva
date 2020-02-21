//
// Created by voltairepunk on 2019-12-31.
//

#ifndef ROMUVA_LOGLEVEL_HPP
#define ROMUVA_LOGLEVEL_HPP

#include <iostream>

namespace Romuva {

    namespace Core {

        enum eLogLevel {
            E_DEBUG,
            E_MESSAGE,
            E_WARNING,
            E_ERROR,
            E_FATAL
        };

        void setLogLevel(eLogLevel level, bool enabled);
        bool logLevel(eLogLevel level);

    }

}

std::ostream& operator << (std::ostream& os, const Romuva::Core::eLogLevel& level);


#endif //ROMUVA_LOGLEVEL_HPP
