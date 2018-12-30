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
        std::vector<Location> locations;
        Player player;
};

class Game::Location {

};

class Game::Player {

};