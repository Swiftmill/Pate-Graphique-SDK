#include "Level.hpp"
#include <algorithm>

namespace levels {

void Level::update(float dt, const input::InputManager& input) {
    for (auto& player : players_) {
        player.applyInput(input, dt);
        resolvePlayerCollisions(player);
    }
    handleCoins();
    handlePowerups();
    handleEnemies(dt);
    handleBoss(dt);
}

void Level::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 40, 40, 50, 255);
    for (const auto& tile : tiles_) {
        if (tile.solid) {
            SDL_RenderFillRectF(renderer, &tile.rect);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 240, 140, 255);
    for (const auto& coin : coins_) {
        if (!coin.collected) SDL_RenderFillRectF(renderer, &coin.rect);
    }

    SDL_SetRenderDrawColor(renderer, 120, 200, 255, 255);
    for (const auto& p : powerups_) {
        if (!p.collected) SDL_RenderFillRectF(renderer, &p.rect);
    }

    for (const auto& enemy : enemies_) {
        if (!enemy.isDefeated()) enemy.render(renderer);
    }

    for (const auto& player : players_) {
        player.render(renderer);
    }

    if (hasBoss_) {
        boss_.render(renderer);
    }
}

void Level::clear() {
    tiles_.clear();
    coins_.clear();
    powerups_.clear();
    enemies_.clear();
    players_.clear();
    hasBoss_ = false;
}

void Level::resolvePlayerCollisions(entities::Player& player) {
    auto rect = player.bounds();
    for (const auto& tile : tiles_) {
        if (!tile.solid) continue;
        SDL_FRect intersection;
        if (SDL_IntersectFRect(&rect, &tile.rect, &intersection)) {
            if (intersection.w < intersection.h) {
                float dx = (rect.x < tile.rect.x) ? -intersection.w : intersection.w;
                player.moveBy(dx, 0);
                rect.x += dx;
            } else {
                float dy = (rect.y < tile.rect.y) ? -intersection.h : intersection.h;
                player.moveBy(0, dy);
                rect.y += dy;
                if (dy < 0) {
                    player.setGrounded(true);
                    player.setVelocityY(0.0f);
                }
            }
        }
    }
}

void Level::handleCoins() {
    for (auto& player : players_) {
        auto pRect = player.bounds();
        for (auto& coin : coins_) {
            if (!coin.collected && SDL_HasIntersectionF(&pRect, &coin.rect)) {
                coin.collected = true;
                player.addCoin();
            }
        }
    }
}

void Level::handlePowerups() {
    for (auto& player : players_) {
        auto pRect = player.bounds();
        for (auto& power : powerups_) {
            if (!power.collected && SDL_HasIntersectionF(&pRect, &power.rect)) {
                power.collected = true;
                player.grantPowerup(power.type);
            }
        }
    }
}

void Level::handleEnemies(float dt) {
    for (auto& enemy : enemies_) {
        enemy.update(dt);
    }
}

void Level::handleBoss(float dt) {
    if (!hasBoss_) return;
    boss_.update(dt);
}

} // namespace levels
