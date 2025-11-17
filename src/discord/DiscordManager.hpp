#pragma once
#include <string>
#include <vector>
#include <functional>

namespace discord {

struct RemoteUserInfo {
    std::string username;
    std::string avatarUrl;
    int64_t userId{0};
};

struct LobbyInfo {
    int64_t id{0};
    std::vector<RemoteUserInfo> members;
};

class DiscordManager {
public:
    bool initialize();
    void shutdown();
    void setPresence(const std::string& details);
    void pumpCallbacks();

    LobbyInfo createOrGetLobby();
    void joinLobby(int64_t id);
    const LobbyInfo& currentLobby() const { return lobby_; }

    const RemoteUserInfo& localUser() const { return localUser_; }

private:
    LobbyInfo lobby_;
    RemoteUserInfo localUser_;
    bool initialized_{false};
};

} // namespace discord
