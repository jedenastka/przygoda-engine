#include <iostream>
#include <string>
#include <vector>

enum DIRECTION {N, E, S, W};

class Game {
    public:
        Game();
        void start();
    private:
        struct Object {
            std::string name;
            std::string description;
        };

        struct Exit {
            DIRECTION direction;
            int locationID;
            std::string str();
        };

        struct Location {
            int ID;
            std::string name;
            std::string description;
            std::vector<Object> objects;
            std::vector<Exit> exits;
            void describe();
        };

        struct Player {
            std::string name;
            int locationID;
            void go(std::string directionStr);
        };

        void takeInput();
        static Location findLocation(int ID);
        static std::vector<Location> locations;
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
    for (std::vector<Object>::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
        std::cout << itr->name;
        if (itr+1 != objects.end()) {
            std::cout << ", ";
        }
    }
    std::cout << ".\n" << "You can go ";
    for (std::vector<Exit>::iterator itr = exits.begin(); itr != exits.end(); ++itr) {
        std::cout << itr->str();
        if (itr+1 != exits.end()) {
            std::cout << ", ";
        }
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
    Exit exit;
    exit.direction = W;
    exit.locationID = 1;
    location.exits.push_back(exit);
    locations.push_back(location);
    Location location2;
    location2.ID = 1;
    location2.name = "libray";
    location2.description = "You can see books everywhere. It's quite abbadoned.";
    exit.direction = E;
    exit.locationID = 0;
    location2.exits.push_back(exit);
    locations.push_back(location2);
    player.locationID = 0;
}

void Game::takeInput() {
    std::string input;
    std::cout << "> ";
    getline(std::cin, input);
    std::vector<std::string> data;
    std::string actualString;
    for (int i = 0; i <= input.size(); i++) {
        if (input[i] == ' ' || i == input.size()) {
            data.push_back(actualString);
            actualString = "";
            continue;
        }
        actualString += input[i];
    }
    if (data[0] == "go") {
        player.go(data[1]);
    } else {
        std::cout << "Hmm?\n";
    }
}

std::vector<Game::Location> Game::locations;

void Game::Player::go(std::string directionStr) {
    DIRECTION direction;
    if (directionStr == "north") {
        direction = N;
    } else if (directionStr == "east") {
        direction = E;
    } else if (directionStr == "south") {
        direction = S;
    } else if (directionStr == "west") {
        direction = W;
    }
    for (auto i: findLocation(locationID).exits) {
        if (i.direction == direction) {
            locationID = i.locationID;
            return;
        }
    }
    std::cout << "You can't go there!\n";
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
        takeInput();
    }
}

int main() {
    Game game;
    game.start();
}