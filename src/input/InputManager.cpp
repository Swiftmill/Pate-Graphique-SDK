#include "InputManager.hpp"

namespace input {

void InputManager::initializeDefaults() {
    bindings_[Action::MoveLeft] = {SDL_SCANCODE_A};
    bindings_[Action::MoveRight] = {SDL_SCANCODE_D};
    bindings_[Action::Jump] = {SDL_SCANCODE_SPACE};
    bindings_[Action::Attack] = {SDL_SCANCODE_J};
    bindings_[Action::Pause] = {SDL_SCANCODE_ESCAPE};
}

void InputManager::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && !e.key.repeat) {
        keyState_[e.key.keysym.scancode] = true;
    } else if (e.type == SDL_KEYUP) {
        keyState_[e.key.keysym.scancode] = false;
    }
}

bool InputManager::isDown(Action action) const {
    auto it = bindings_.find(action);
    if (it == bindings_.end()) return false;
    auto ks = keyState_.find(it->second.key);
    return ks != keyState_.end() && ks->second;
}

void InputManager::rebind(Action action, SDL_Scancode key) {
    bindings_[action] = {key};
}

} // namespace input
