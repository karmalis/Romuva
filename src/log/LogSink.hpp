//
// Created by voltairepunk on 2019-12-31.
//

#ifndef ROMUVA_LOGSINK_HPP
#define ROMUVA_LOGSINK_HPP

#include <memory>

#include "LogMessage.hpp"

namespace Romuva {

    namespace Core {

        class LogSink {
        public:

            template <typename T>
            LogSink(T impl);

            LogSink(const LogSink& sink);
            LogSink& operator= (LogSink sink);

            bool operator == (const LogSink& sink) const;

            void forward(
                    const LogMessage::Meta& meta,
                    const std::string& message
                    ) const;

        protected:

            struct Concept {
                virtual ~Concept() = default;
                virtual Concept* clone() const = 0;

                virtual void forward(
                        const LogMessage::Meta& meta,
                        const std::string& message
                ) const = 0;

            };

            template <typename T>
            struct Model: Concept {

                Model(T impl);

                virtual Concept* clone() const override;

                virtual void forward(
                        const LogMessage::Meta& meta,
                        const std::string& message
                ) const override;

                T _impl;
            };

            std::unique_ptr<Concept> _wrapper;

        };

        LogSink makeConsoleSink();
        LogSink makeFileSink(const std::string& filename);

        template <typename T>
        LogSink::LogSink(T impl) :
                _wrapper(new Model<T>(std::forward<T>(impl)))
        {

        }

        template <typename T>
        LogSink::Model<T>::Model(T impl) :
                _impl(std::forward<T>(impl))
        {

        }

        template <typename T>
        LogSink::Concept* LogSink::Model<T>::clone() const {
            return new Model<T>(_impl);
        }

        template <typename T>
        void LogSink::Model<T>::forward(
                const LogMessage::Meta& meta,
                const std::string& message
        ) const {
            this->_impl(meta, message);
        }

    }

}


#endif //ROMUVA_LOGSINK_HPP
