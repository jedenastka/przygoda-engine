#include <iostream>
#include <string>
#include <vector>

class Game {
    public:
        Game();
        void start();
    private:
        
        class Location;
        class Player;
        class Object;
        class Exit;
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
    locations.push_back(location);
    player.location = *locations[0];
}

void Game::start() {
    while (1) {
        player.location->describe();
        //takeInput();
    }
}

class Game::Location {
    public:
        std::string name;
        std::string description;
        std::vector<Object> objects;
        std::vector<Exit> exits;
        void describe();
};

void Game::Location::describe() {
    std::cout << "You are in " << name << ".\n" << description << "\n" << "You can see ";
    for (auto i : objects) {
        std::cout << i.name << " ";
    }
    std::cout << ".\n" << "You can go ";
    for (auto i : objects) {
        std::cout << i.direction << " ";
    }
    std::cout << ".\n";
}

class Game::Player {
    public:
        std::string name;
        Location *location;
};

class Game::Object {
    public:
        std::string name;
        std::string description;
};

class Game::Exit {
    public:
        DIRECTION direction;
        Location *location;
};

enum class Game::DIRECTION {N, E, S, W};

int main() {
    Game game;
    game.start();
}