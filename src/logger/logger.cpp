#include "logger.hpp"

#include <iostream>

namespace piper {
    namespace logger {
        Logger::Logger() {
            this->hadError = false;
        }

        void Logger::addError(
            const std::string &filename,
            int line,
            int column,
            const std::string &category,
            LoggerSeverity severity,
            const std::string &message
        ) {
            std::cout << filename << ":" << line << ":" << column << " [" << category << "][";
            switch (severity) {
                case LoggerSeverity::ERROR:
                    std::cout << "ERROR";
                    break;
                case LoggerSeverity::INFO:
                    std::cout << "INFO";
                    break;
                case LoggerSeverity::WARNING:
                    std::cout << "WARNING";
                    break;
            }
            std::cout << "]" << message << std::endl;
            this->hadError = true;
        }

        bool Logger::getHadError() {
            return this->hadError;
        }
    }
}