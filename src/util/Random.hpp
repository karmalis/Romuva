//
// Created by voltairepunk on 2020-01-02.
//

#ifndef ROMUVA_RANDOM_HPP
#define ROMUVA_RANDOM_HPP

#include <string>

namespace Romuva {

    namespace Util {

        namespace Random {

            template <typename T>
            T generate(std::size_t length);

            template <typename T>
            T generate(T min, T max);

        }

    }

}


#endif //ROMUVA_RANDOM_HPP
