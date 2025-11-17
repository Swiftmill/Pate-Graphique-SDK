#include "Boss.hpp"
#include <cstdlib>
#include <algorithm>

namespace entities {

Boss::Boss(float x, float y) : x_(x), y_(y) {}

void Boss::update(float dt) {
    phaseTimer_ += dt;
    if (phaseTimer_ > 2.0f) {
        spawnAttack();
        phaseTimer_ = 0.0f;
    }

    for (auto& atk : attacks_) {
        atk.lifetime -= dt;
    }
    attacks_.erase(std::remove_if(attacks_.begin(), attacks_.end(), [](const BossAttack& a){ return a.lifetime <= 0; }), attacks_.end());
}

void Boss::render(SDL_Renderer* renderer) const {
    SDL_FRect rect{x_, y_, 120, 120};
    SDL_SetRenderDrawColor(renderer, 255, 120, 0, 255);
    SDL_RenderFillRectF(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 40, 0, 160);
    for (const auto& atk : attacks_) {
        SDL_RenderFillRectF(renderer, &atk.zone);
    }
}

void Boss::hit(int dmg) {
    health_ -= dmg;
}

void Boss::spawnAttack() {
    BossAttack atk;
    atk.zone = SDL_FRect{x_ - 40 + static_cast<float>(std::rand() % 80), y_ + 140, 80, 40};
    atk.lifetime = 3.0f;
    attacks_.push_back(atk);
}

} // namespace entities
