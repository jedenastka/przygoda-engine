#include <iostream>
#include <vector>
#include <map>
#include <string>

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    UP,
    DOWN
};

class Room {
    public:
        Room(std::map<Direction, Room*> exitsTmp) {
            exits[NORTH] = NULL;
            exits[EAST] = NULL;
            exits[SOUTH] = NULL;
            exits[WEST] = NULL;
            exits[UP] = NULL;
            exits[DOWN] = NULL;
        }
    private:
        std::map<Direction, Room*> exits;
};

class Location {
    public:
        Location* getRoom(std::string id);
    private:
        std::map<std::string, Room> rooms;
};

class Map {
    public:
        Location* getLocation(std::string id);
    private:
        std::map<std::string, Location*> locations;
};

class Player {
    public:
        void go(Direction);
    private:
        Room* currentRoom;
};

Player player;
Map map;

int main() {
    
}