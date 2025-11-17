#pragma once
#include <string>
#include "Level.hpp"

namespace levels {

class LevelLoader {
public:
    bool loadFromFile(const std::string& path, Level& level);
};

} // namespace levels
