#pragma once

#include <string>


enum class ItemType
{
    Fridge,
    Shelf,
    OverShelf,
    IceMaker
};


struct Item
{
    std::string name;


    double length;
    double width;


    ItemType type;


    // 输出结果
    double centerX = 0;
    double centerY = 0;

    double angle = 0;  // 0或者90
};