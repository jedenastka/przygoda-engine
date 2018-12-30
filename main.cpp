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
    // ...
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