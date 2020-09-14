#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>

#include <nlohmann/json.hpp>



class Room;
template<typename Iterator, typename Function> std::string enumerateItems(Iterator, Iterator, Function);



class Direction {

    public:
        enum Value {
            None,
            North,
            South,
            East,
            West,
            Up,
            Down
        };
        
        Direction(Value);
        Direction(std::string);

        bool operator==(const Direction) const;
        bool operator!=(const Direction) const;
        bool operator<(const Direction) const;

        std::string str() const;
    
    private:
        Value value;
    
};

Direction::Direction(Value value) {
    this->value = value;
}

Direction::Direction(std::string string) {
    if (string == "north" || string == "n") {
        value = North;
    } else if (string == "south" || string == "s") {
        value = South;
    } else if (string == "east" || string == "e") {
        value = East;
    } else if (string == "west" || string == "w") {
        value = West;
    } else if (string == "up" || string == "u") {
        value = Up;
    } else if (string == "down" || string == "d") {
        value = Down;
    } else {
        value = None;
    }
}

bool Direction::operator==(const Direction r) const {
    return value == r.value;
}

bool Direction::operator!=(const Direction r) const {
    return value != r.value;
}

bool Direction::operator<(const Direction r) const {
    return std::tie(value) < std::tie(r.value);
}

std::string Direction::str() const {
    switch (this->value) {
        
        case North:
            return "north";
        
        case South:
            return "south";
        
        case East:
            return "east";
        
        case West:
            return "west";
        
        case Up:
            return "up";
        
        case Down:
            return "down";
        
        default:
            return "";
        
    }
}


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
        std::cout << "\nYou can go " << enumerateItems(exits.begin(), exits.end(), [](auto itr) { return itr->first.str(); }) << ".\n";
    }

    // TODO: There is no 'hidden' property for items (yet), but it won't take it into account anyways.
    if (items.size() > 0) {
        std::cout << "\nYou can see " << enumerateItems(items.begin(), items.end(), [](auto itr) { return itr->getProperty(Item::Name); }) << ".\n";
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

template<typename Iterator, typename Function>
std::string enumerateItems(Iterator begin, Iterator end, Function getString) {
    std::stringstream ss;
    for (Iterator itr = begin; itr != end; ++itr) {
        if (itr != begin) {
            if (std::next(itr) == end) {
                ss << " and ";
            } else {
                ss << ", ";
            }
        }
        ss << getString(itr);
    }
    return ss.str();
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
            exits[exit.key()] = Exit(&rooms[exit.value()["target"]], exit.value()["hidden"]);
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
            Direction direction = args[0];
            if (direction != Direction::None) {
                if (!player.moveDirection(direction)) {
                    std::cout << "Can't go " << direction.str() << ".\n";
                }
            } else {
                std::cout << "There is no direction '" << args[0] << "'.\n";
            }
        } else if (command == "exit" && args.size() == 0) {
            end = 1;
        } else if (command == "describe" && args.size() == 0) {
            player.getRoom()->describe();
        } else if (command == "examine" && args.size() == 1) {
            std::vector<Item> items = player.getRoom()->getItems();
            auto foundItem = std::find_if(items.begin(), items.end(), [args](Item item) { return item.getProperty(Item::Name) == args[0]; });
            
            if (foundItem != items.end()) {
                std::cout << foundItem->getProperty(Item::Description) << '\n';
            } else {
                std::cout << "There is no " << args[0] << " there.\n";
            }
        } else if (command == "take" && args.size() == 1) {
            std::vector<Item> items = player.getRoom()->getItems();
            auto foundItem = std::find_if(items.begin(), items.end(), [args](Item item) { return item.getProperty(Item::Name) == args[0]; });
            
            if (foundItem != items.end()) {
                player.getRoom()->removeItem(*foundItem);
                player.addItem(*foundItem);
            } else {
                std::cout << "There is no " << args[0] << " there.\n";
            }
        } else if (command == "put" && args.size() == 1) {
            std::vector<Item> items = player.getItems();
            auto foundItem = std::find_if(items.begin(), items.end(), [args](Item item) { return item.getProperty(Item::Name) == args[0]; });
            
            if (foundItem != items.end()) {
                player.removeItem(*foundItem);
                player.getRoom()->addItem(*foundItem);
            } else {
                std::cout << "There is no " << args[0] << " in your inventory.\n";
            }
        } else {
            std::cout << "Invalid command.\n";
        }
    }
}