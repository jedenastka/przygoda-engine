#include <iostream>
#include <string>
#include <vector>

class Game {
    public:
        Game();
        void start();
    private:
        struct Location;
        struct Player;
        struct Object;
        struct Exit;
        enum DIRECTION;
        std::vector<Location> locations;
        Player player;
};

Game::Game() {
    //FNI
}

void Game::start {
    ///FNI
}

struct Game::Location {
    std::string name;
    std::string description;
    std::vector<Object> objects;
    std::vector<Exit> exits;
};

struct Game::Player {
    std::string name;
    Location *location;
};

struct Game::Object {
    std::string name;
    std::string description;
};

struct Game::Exit {
    DIRECTION direction;
    Location *location;
};

enum Game::DIRECTION {N, E, S, W};

int main() {
    Game game;
    game.start();
}