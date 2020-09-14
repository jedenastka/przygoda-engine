#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include <nlohmann/json.hpp>



class Room;

enum class Direction;

std::string directionToString(Direction);



// TODO: Should be a class. See below.
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


class Item {
    
    public:
        enum Property {
            Name,
            Description
        };
        
        Item(std::string);

        bool operator==(Item);

        std::string getProperty(Property);
        std::string getId();

        static void addItem(std::string, std::map<Property, std::string>);
    
    private:
        std::string id;

        static std::map<std::string, std::map<Property, std::string>> itemProperties;
    
};

std::map<std::string, std::map<Item::Property, std::string>> Item::itemProperties;

Item::Item(std::string id) {
    this->id = id;
}

bool Item::operator==(Item r) {
    return this->getId() == r.getId();
}

std::string Item::getProperty(Property property) {
    return itemProperties[id][property];
}

std::string Item::getId() {
    return id;
}

void Item::addItem(std::string id, std::map<Property, std::string> properties) {
    itemProperties[id] = properties;
}


class Room {

    public:
        Room() {}
        Room(std::string, std::map<Direction, Exit>, std::vector<Item>);

        bool getExit(Direction, Exit&);
        std::vector<Item> getItems();

        bool removeItem(Item);
        void addItem(Item);
        void describe();
    
    private:
        std::string description;
        std::map<Direction, Exit> exits;
        std::vector<Item> items;

};

Room::Room(std::string description, std::map<Direction, Exit> exits, std::vector<Item> items) {
    this->description = description;
    this->exits = exits;
    this->items = items;
}

bool Room::getExit(Direction direction, Exit &exit) {
    if (exits.find(direction) != exits.end()) {
        exit = exits[direction];
        return 1;
    } else {
        return 0;
    }
}

std::vector<Item> Room::getItems() {
    return items;
}

bool Room::removeItem(Item item) {
    auto foundItem = std::find(items.begin(), items.end(), item);
    if (foundItem != items.end()) {
        items.erase(foundItem);
        return 1;
    } else {
        return 0;
    }
}

void Room::addItem(Item item) {
    items.push_back(item);
}

void Room::describe() {
    std::cout << description << "\n";

    // TODO: Doesn't take 'hidden' property into account.
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

    // TODO: There is no 'hidden' property for items (yet), but it won't take it into account anyways.
    if (items.size() > 0) {
        std::cout << "\nYou can see ";
        // TODO: Copy-pasted code. Move that into a function... if possible.
        for (auto itr = items.begin(); itr != items.end(); ++itr) {
            if (itr != items.begin()) {
                if (std::next(itr) == items.end()) {
                    std::cout << " and ";
                } else {
                    std::cout << ", ";
                }
            }
            std::cout << itr->getProperty(Item::Name);
        }
        std::cout << ".\n";
    }
}


class Player {

    public:
        Player() {}

        Room *getRoom();
        std::vector<Item> getItems();

        void moveTo(Room*, bool);
        bool moveDirection(Direction);
        bool removeItem(Item);
        void addItem(Item);
    
    private:
        Room *room;
        std::vector<Item> items;
    
};

Room *Player::getRoom() {
    return room;
}

std::vector<Item> Player::getItems() {
    return items;
}

void Player::moveTo(Room *room, bool silent = 0) {
    this->room = room;
    if (!silent) {
        this->room->describe();
    }
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

bool Player::removeItem(Item item) {
    auto foundItem = std::find(items.begin(), items.end(), item);
    if (foundItem != items.end()) {
        items.erase(foundItem);
        return 1;
    } else {
        return 0;
    }
}

void Player::addItem(Item item) {
    items.push_back(item);
}



// TODO: Make a Direction class and move this there.
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

// TODO: This too. As a static member.
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

    for (auto item: game["items"].items()) {
        std::map<Item::Property, std::string> properties;
        properties[Item::Name] = item.value()["name"];
        properties[Item::Description] = item.value()["description"];
        Item::addItem(item.key(), properties);
    }
    
    std::map<int, Room> rooms;
    for (nlohmann::json room: game["rooms"]) {
        std::map<Direction, Exit> exits;
        for (auto exit: room["exits"].items()) {
            // There could be a problem with 'target' being a pointer, if the room it points to isn't yet in 'exits' map.
            exits[stringToDirection(exit.key())] = Exit(&rooms[exit.value()["target"]], exit.value()["hidden"]);
        }

        std::vector<Item> items;
        for (nlohmann::json item: room["items"]) {
            items.push_back(Item(item));
        }

        rooms[room["id"]] = Room(room["description"], exits, items);
    }

    Player player;
    player.moveTo(&rooms[game["player"]["room"]]);

    bool end = 0;
    while (!end) {
        std::cout << "? ";
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
        } else if (command == "describe" && args.size() == 0) {
            player.getRoom()->describe();
        } else if (command == "examine" && args.size() == 1) {
            bool found = 0;
            for (Item item: player.getRoom()->getItems()) {
                if (item.getProperty(Item::Name) == args[0]) {
                    std::cout << item.getProperty(Item::Description) << '\n';
                    found = 1;
                    break;
                }
            }

            if (!found) {
                std::cout << "There is no " << args[0] << " there.\n";
            }
        } else if (command == "take" && args.size() == 1) {
            // TODO: This is kinda copypasted.
            for (Item item: player.getRoom()->getItems()) {
                if (item.getProperty(Item::Name) == args[0]) {
                    player.getRoom()->removeItem(item);
                    player.addItem(item);
                    break;
                }
            }
        } else if (command == "put" && args.size() == 1) {
            // TODO: This too.
            for (Item item: player.getItems()) {
                if (item.getProperty(Item::Name) == args[0]) {
                    player.removeItem(item);
                    player.getRoom()->addItem(item);
                    break;
                }
            }
        } else {
            std::cout << "Invalid command.\n";
        }
    }
}