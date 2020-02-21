//
// Created by voltairepunk on 2019-12-31.
//

#include <iostream>
#include <memory>
#include <string>
#include <fstream>

#include <chrono>
#include <iomanip>
#include <ctime>


#include "LogSink.hpp"

namespace Romuva {

    namespace Core {

        LogSink::LogSink(const LogSink &sink) :
                _wrapper(sink._wrapper->clone())
        {

        }

        LogSink& LogSink::operator=(LogSink sink) {
            this->_wrapper = std::move(sink._wrapper);
            return *this;
        }

        bool LogSink::operator==(const LogSink &sink) const {
            return (this->_wrapper.get() == sink._wrapper.get());
        }

        void LogSink::forward(const LogMessage::Meta &meta, const std::string &message) const {
            this->_wrapper->forward(meta, message);
        }

        LogSink makeConsoleSink() {
            return [] (
                    const LogMessage::Meta& meta,
                    const std::string& message
            ) {
                std::cout << meta.level << message << std::endl;
            };
        }

        struct FileSink {
            FileSink(const std::string& filename):
                    file(std::make_shared<std::ofstream>(filename))
            {
                if ( ! this->file->good()) {
                    std::string message = "Failed to open log file sink: ";
                    message.append(filename);
                    throw std::runtime_error(message);
                }
            }

            void operator()(
                    const LogMessage::Meta& meta,
                    const std::string& message
            ) const {
                using namespace std::chrono;

                auto now = system_clock::now();
                auto time_t = system_clock::to_time_t(now);
                auto local_time = std::localtime(&time_t);

                (*this->file)
                        << std::put_time(local_time, " [%H:%M:%S] ")
                        << meta.level
                        << message
                        << " ( "
                        << meta.file
                        << " : "
                        << meta.line
                        << " )"
                        << std::endl;
            }

            std::shared_ptr<std::ofstream> file;
        };

        LogSink makeFileSink(const std::string& filename) {
            return FileSink(filename);
        }


    }

}
