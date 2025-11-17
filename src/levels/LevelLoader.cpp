#include "LevelLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace levels {

bool LevelLoader::loadFromFile(const std::string& path, Level& level) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open level file: " << path << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto json = nlohmann::json::parse(buffer.str());

    level.clear();
    if (json.contains("players")) {
        for (const auto& player : json["players"]) {
            entities::Player p;
            p.setName(player.value<std::string>("name", "GPU"));
            p.setPosition(player.value<double>("x", 64), player.value<double>("y", 64));
            level.players().push_back(p);
        }
    }

    if (json.contains("tiles")) {
        for (const auto& tile : json["tiles"]) {
            Tile t;
            t.rect = SDL_FRect{static_cast<float>(tile.value<double>("x",0)), static_cast<float>(tile.value<double>("y",0)),
                               static_cast<float>(tile.value<double>("w",64)), static_cast<float>(tile.value<double>("h",64))};
            t.solid = tile.value<bool>("solid", true);
            level.addTile(t);
        }
    }

    if (json.contains("coins")) {
        for (const auto& coin : json["coins"]) {
            Coin c;
            c.rect = SDL_FRect{static_cast<float>(coin.value<double>("x",0)), static_cast<float>(coin.value<double>("y",0)), 24, 24};
            level.addCoin(c);
        }
    }

    if (json.contains("powerups")) {
        for (const auto& pow : json["powerups"]) {
            Powerup p;
            p.rect = SDL_FRect{static_cast<float>(pow.value<double>("x",0)), static_cast<float>(pow.value<double>("y",0)), 28, 28};
            p.type = pow.value<std::string>("type", "shield");
            level.addPowerup(p);
        }
    }

    if (json.contains("enemies")) {
        for (const auto& enemy : json["enemies"]) {
            entities::Enemy e(static_cast<float>(enemy.value<double>("x",0)), static_cast<float>(enemy.value<double>("y",0)));
            level.addEnemy(e);
        }
    }

    if (json.contains("boss")) {
        const auto& boss = json["boss"];
        entities::Boss b(static_cast<float>(boss.value<double>("x",0)), static_cast<float>(boss.value<double>("y",0)));
        level.setBoss(b);
    }

    return true;
}

} // namespace levels
