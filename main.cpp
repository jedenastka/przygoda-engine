#include <iostream>
#include <vector>

class Room {
    public:
    private:
};

class Location {
    public:
    private:
        std::vector<Room> rooms;
};

class Map {
    public:
    private:
        std::vector<Location> locations;
};

class Player {
    public:
    private:
        int room;
};

int main() {
    Player player;
}