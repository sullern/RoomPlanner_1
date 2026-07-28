#pragma once

#include <string>
#include <vector>

#include "Point.h"
#include "Item.h"



struct RoomData
{
    std::vector<Point> boundary;

    std::vector<Point> door;


    bool isOpenInward;


    std::vector<Item> items;

};



class JsonLoader
{

public:

    static RoomData load(
        const std::string& path
    );

};