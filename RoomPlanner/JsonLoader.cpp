#include "JsonLoader.h"

#include <fstream>
#include <nlohmann/json.hpp>


using json = nlohmann::json;



RoomData JsonLoader::load(
    const std::string& path
)
{

    RoomData room;


    std::ifstream file(path);


    json data;

    file >> data;



    for (auto& p : data["boundary"])
    {
        room.boundary.push_back(
            {
                p[0],
                p[1]
            }
        );
    }



    for (auto& p : data["door"])
    {
        room.door.push_back(
            {
                p[0],
                p[1]
            }
        );
    }


    room.isOpenInward =
        data["isOpenInward"];



    for (auto& item : data["algoToPlace"].items())
    {

        Item obj;


        obj.name =
            item.key();


        obj.length =
            item.value()[0];


        obj.width =
            item.value()[1];

        if (item.key().find("fridge") != std::string::npos)
            obj.type = ItemType::Fridge;
        else if (item.key().find("iceMaker") != std::string::npos)
            obj.type = ItemType::IceMaker;
        else if (item.key().find("overShelf") != std::string::npos)
            obj.type = ItemType::OverShelf;
        else if (item.key().find("shelf") != std::string::npos)
            obj.type = ItemType::Shelf;


        room.items.push_back(obj);

    }


    return room;

}