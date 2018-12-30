#include <iostream>
#include <string>
#include <vector>

class Game {
    public:
        Game();
        void start();
    private:
        struct Location;
        struct Player {
            std::string name;
            Location *location;
        };
        struct Object;
        struct Exit;
        enum class DIRECTION;
        std::vector<Location> locations;
        Player player;
};

Game::Game() {
    player.name = "player";
    Location location;
    location.name = "room";
    location.description = "It seems small and smells bad.";
    Object object;
    object.name = "computer";
    object.description = "It looks like it's using Debian GNU/Linux.";
    location.objects.push_back(object);
}

void Game::start() {
    // ...
}

struct Game::Location {
    std::string name;
    std::string description;
    std::vector<Object> objects;
    std::vector<Exit> exits;
};

// Here was Player

struct Game::Object {
    std::string name;
    std::string description;
};

struct Game::Exit {
    DIRECTION direction;
    Location *location;
};

enum class Game::DIRECTION {N, E, S, W};

int main() {
    Game game;
    game.start();
}