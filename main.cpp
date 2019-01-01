#include <iostream>
#include <string>
#include <vector>

class Game {
    public:
        Game();
        void start();
    private:
        enum class DIRECTION {N, E, S, W};

        struct Object {
            std::string name;
            std::string description;
        };

        struct Exit {
            DIRECTION direction;
            int locationID;
            std::string str();
        };

        struct Player {
            std::string name;
            int locationID;
        };

        struct Location {
            int ID;
            std::string name;
            std::string description;
            std::vector<Object> objects;
            std::vector<Exit> exits;
            void describe();
        };

        Location findLocation(int ID);
        std::vector<Location> locations;
        Player player;
};

std::string Game::Exit::str() {
    switch (direction) {
        case N:
            return "north";
        case E:
            return "east";
        case S:
            return "south";
        case W:
            return "west";
    }
}

void Game::Location::describe() {
    std::cout << "You are in " << name << ".\n" << description << "\n" << "You can see ";
    for (auto i: objects) {
        std::cout << i.name << " ";
    }
    std::cout << ".\n" << "You can go ";
    for (auto i: exits) {
        std::cout << i.str() << " ";
    }
    std::cout << ".\n";
}

Game::Game() {
    player.name = "player";
    Location location;
    location.ID = 0;
    location.name = "room";
    location.description = "It seems small and smells bad.";
    Object object;
    object.name = "computer";
    object.description = "It looks like it's using Debian GNU/Linux.";
    location.objects.push_back(object);
    locations.push_back(location);
    player.locationID = 0;
}

Game::Location Game::findLocation(int ID) {
    for (auto i: locations) {
        if (i.ID == ID) {
            return i;
        }
    }
    throw "Location not found.";
}

void Game::start() {
    while (1) {
        findLocation(player.locationID).describe();
        //takeInput();
    }
}

int main() {
    Game game;
    game.start();
}