#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "entities/Boss.hpp"
#include "input/InputManager.hpp"

namespace levels {

struct Tile {
    SDL_FRect rect;
    bool solid{false};
};

struct Coin {
    SDL_FRect rect;
    bool collected{false};
};

struct Powerup {
    SDL_FRect rect;
    std::string type;
    bool collected{false};
};

class Level {
public:
    void setRenderer(SDL_Renderer* renderer) { renderer_ = renderer; }
    void update(float dt, const input::InputManager& input);
    void render(SDL_Renderer* renderer) const;
    void clear();

    void addTile(const Tile& t) { tiles_.push_back(t); }
    void addCoin(const Coin& c) { coins_.push_back(c); }
    void addPowerup(const Powerup& p) { powerups_.push_back(p); }
    void addEnemy(const entities::Enemy& e) { enemies_.push_back(e); }
    void setBoss(const entities::Boss& b) { boss_ = b; hasBoss_ = true; }
    std::vector<entities::Player>& players() { return players_; }
    const std::vector<entities::Player>& players() const { return players_; }

private:
    void resolvePlayerCollisions(entities::Player& player);
    void handleCoins();
    void handlePowerups();
    void handleEnemies(float dt);
    void handleBoss(float dt);

    SDL_Renderer* renderer_{};
    std::vector<Tile> tiles_;
    std::vector<Coin> coins_;
    std::vector<Powerup> powerups_;
    std::vector<entities::Enemy> enemies_;
    entities::Boss boss_{0,0};
    bool hasBoss_{false};
    std::vector<entities::Player> players_;
};

} // namespace levels
