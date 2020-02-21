//
// Created by voltairepunk on 2020-01-02.
//

#include <chrono>
#include <random>
#include <algorithm>
#include <vector>


#include "Random.hpp"

namespace Romuva {

    namespace Util {

        namespace Random {

            std::vector<char> possible_random_chars{
                    '0', '1', '2', '3', '4',
                    '5', '6', '7', '8', '9',
                    'A', 'B', 'C', 'D', 'E', 'F',
                    'G', 'H', 'I', 'J', 'K',
                    'L', 'M', 'N', 'O', 'P',
                    'Q', 'R', 'S', 'T', 'U',
                    'V', 'W', 'X', 'Y', 'Z',
                    'a', 'b', 'c', 'd', 'e', 'f',
                    'g', 'h', 'i', 'j', 'k',
                    'l', 'm', 'n', 'o', 'p',
                    'q', 'r', 's', 't', 'u',
                    'v', 'w', 'x', 'y', 'z'
            };

            template<typename T>
            T generate(std::size_t length) {
                throw std::exception{};
            }

            template<>
            std::string generate<std::string>(std::size_t length) {
                std::string str(length, 0);
                std::default_random_engine rng(std::random_device{}());
                std::mt19937 mt(rng());
                std::uniform_int_distribution<> dist(0, possible_random_chars.size() - 1);


                std::generate_n(str.begin(), length, [&dist, &mt]{
                    return possible_random_chars[dist(mt)];
                });

                return str;
            }

            template<typename T>
            T generate(T min, T max) {
                std::default_random_engine rd(std::random_device{}());
                std::mt19937 mt(rd());

                if (std::is_integral<T>::value) {
                    std::uniform_int_distribution<T> dist{min, max};
                    return dist(mt);
                } else if (std::is_floating_point<T>::value) {
                    std::uniform_real_distribution<T> dist{min, max};
                    return dist(mt);
                }

                throw std::exception{};
            }



        }

    }

}
