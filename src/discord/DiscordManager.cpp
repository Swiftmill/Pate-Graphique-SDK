#include "DiscordManager.hpp"
#include <iostream>
#include <random>

namespace discord {

bool DiscordManager::initialize() {
    initialized_ = true;
    localUser_.username = "GPU Hero";
    localUser_.avatarUrl = "";
    localUser_.userId = 1;
    std::cout << "Discord SDK initialized (stub)" << std::endl;
    return true;
}

void DiscordManager::shutdown() {
    initialized_ = false;
}

void DiscordManager::setPresence(const std::string& details) {
    if (!initialized_) return;
    std::cout << "Setting Discord presence: " << details << std::endl;
}

void DiscordManager::pumpCallbacks() {
    // Real implementation would call discord::Core::RunCallbacks();
}

LobbyInfo DiscordManager::createOrGetLobby() {
    if (lobby_.id == 0) {
        std::random_device rd;
        lobby_.id = static_cast<int64_t>(rd());
        lobby_.members.push_back(localUser_);
        std::cout << "Created lobby " << lobby_.id << std::endl;
    }
    return lobby_;
}

void DiscordManager::joinLobby(int64_t id) {
    lobby_.id = id;
    lobby_.members.push_back(localUser_);
    std::cout << "Joined lobby " << id << std::endl;
}

} // namespace discord
