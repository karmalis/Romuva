//
// Created by voltairepunk on 2020-01-24.
//

#ifndef ROMUVA_EXCEPTIONS_HPP
#define ROMUVA_EXCEPTIONS_HPP

#include <bits/exception.h>

#include <utility>

namespace Romuva {

    class Exception : public std::exception {
    protected:

        std::string _message;

    public:

        Exception(std::string message) : _message(std::move(message)) {}

        const char *what() const noexcept override {
            return _message.c_str();
        }
    };

    class NotImplementedException : public std::exception {
    public:
        const char *what() const noexcept override {
            return "Not implemented";
        }
    };

    class BadArgumentException : public std::exception {
    public :
        const char *what() const noexcept override {
            return "Bad argument";
        }
    };
}

#endif //ROMUVA_EXCEPTIONS_HPP
