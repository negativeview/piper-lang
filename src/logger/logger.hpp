#pragma once

#include <string>

namespace piper {
    namespace logger {
        enum LoggerSeverity {
            INFO,
            WARNING,
            ERROR
        };

        class Logger {
            public:
                Logger();
                ~Logger() = default;

                void addError(
                    const std::string &filename,
                    int line,
                    int column,
                    const std::string &category,
                    LoggerSeverity severity,
                    const std::string &message
                );

                bool getHadError();
            protected:
                bool hadError;
        };
    }
}