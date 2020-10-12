#pragma once

#include <string>

namespace piper {
    namespace tokenizer {
        struct Location {
            int line;
            int column;
            std::string filename;
        };
    }
}