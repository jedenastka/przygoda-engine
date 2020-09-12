#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include <nlohmann/json.hpp>



class Room;

enum class Direction;

std::string directionToString(Direction);



enum class Direction {
    None,
    North,
    South,
    East,
    West,
    Up,
    Down
};


class Exit {
    
    public:
        Exit() {}
        Exit(Room*, bool);

        Room *getRoom();
        bool isHidden();
    
    private:
        Room *room;
        bool hidden;
    
};

Exit::Exit(Room *room, bool hidden) {
    this->room = room;
    this->hidden = hidden;
}

Room *Exit::getRoom() {
    return room;
}

bool Exit::isHidden() {
    return hidden;
}


class Room {

    public:
        Room() {}
        Room(std::string, std::map<Direction, Exit>);

        void describe();
        bool getExit(Direction, Exit&);
    
    private:
        std::string description;
        std::map<Direction, Exit> exits;

};

Room::Room(std::string description, std::map<Direction, Exit> exits) {
    this->description = description;
    this->exits = exits;
}

void Room::describe() {
    std::cout << description << "\n";
    if (exits.size() > 0) {
        std::cout << "\nYou can go ";
        for (auto itr = exits.begin(); itr != exits.end(); ++itr) {
            if (itr != exits.begin()) {
                if (std::next(itr) == exits.end()) {
                    std::cout << " and ";
                } else {
                    std::cout << ", ";
                }
            }
            std::cout << directionToString(itr->first);
        }
        std::cout << ".\n";
    }
}

bool Room::getExit(Direction direction, Exit &exit) {
    if (exits.find(direction) != exits.end()) {
        exit = exits[direction];
        return 1;
    } else {
        return 0;
    }
}


class Player {

    public:
        Player(Room*);

        Room *getRoom();
        void moveTo(Room*);
        bool moveDirection(Direction);
    
    private:
        Room *room;
    
};

Player::Player(Room *room) {
    this->room = room;
}

Room *Player::getRoom() {
    return room;
}

void Player::moveTo(Room *room) {
    this->room = room;
}

bool Player::moveDirection(Direction direction) {
    Exit exit;
    if (room->getExit(direction, exit)) {
        moveTo(exit.getRoom());
        return 1;
    } else {
        return 0;
    }
}



std::string directionToString(Direction direction) {
    switch (direction) {
        
        case Direction::North:
            return "north";
        
        case Direction::South:
            return "south";
        
        case Direction::East:
            return "east";
        
        case Direction::West:
            return "west";
        
        case Direction::Up:
            return "up";
        
        case Direction::Down:
            return "down";
        
        default:
            throw;
        
    }
}

Direction stringToDirection(std::string string) {
    if (string == "north" || string == "n") {
        return Direction::North;
    } else if (string == "south" || string == "s") {
        return Direction::South;
    } else if (string == "east" || string == "e") {
        return Direction::East;
    } else if (string == "west" || string == "w") {
        return Direction::West;
    } else if (string == "up" || string == "u") {
        return Direction::Up;
    } else if (string == "down" || string == "d") {
        return Direction::Down;
    } else {
        return Direction::None;
    }
}

std::vector<std::string> splitString(std::string string, char delimeter = ' ') {
    std::vector<std::string> ret;
    std::string temp;
    for (char ch: string) {
        if (ch == delimeter) {
            ret.push_back(temp);
            temp = "";
        } else {
            temp += ch;
        }
    }
    ret.push_back(temp);
    return ret;
}

int main() {
    nlohmann::json game;
    std::ifstream gameFile("game.json");
    gameFile >> game;
    
    std::map<int, Room> rooms;
    for (nlohmann::json room: game["rooms"]) {
        std::map<Direction, Exit> exits;
        for (auto exit: room["exits"].items()) {
            // There could be a problem with 'target' being a pointer, if the room it points to isn't yet in 'exits' map.
            exits[stringToDirection(exit.key())] = Exit(&rooms[exit.value()["target"]], exit.value()["hidden"]);
        }
        rooms[room["id"]] = Room(room["description"], exits);
    }

    Player player(&rooms[game["player"]["room"]]);

    bool end = 0;
    while (!end) {
        player.getRoom()->describe();

        std::cout << "\n? ";
        std::string commandString;
        getline(std::cin, commandString);

        std::vector<std::string> args = splitString(commandString);
        std::string command = args[0];
        args.erase(args.begin());

        if (command == "go" && args.size() == 1) {
            Direction direction = stringToDirection(args[0]);
            if (direction != Direction::None) {
                if (!player.moveDirection(direction)) {
                    std::cout << "Can't go " << directionToString(direction) << ".\n";
                }
            } else {
                std::cout << "There is no direction '" << args[0] << "'.\n";
            }
        } else if (command == "exit" && args.size() == 0) {
            end = 1;
        } else {
            std::cout << "Invalid command.\n";
        }
    }
}