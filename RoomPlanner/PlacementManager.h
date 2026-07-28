#pragma once

#include <vector>

#include "Item.h"
#include "Rect.h"
#include "Point.h"


class PlacementManager
{

private:

    std::vector<Rect> placed;// 已放置物品
    std::vector<Point> doorObstacle;// 门占用障碍物
    bool hasDoorObstacle = false;
    // 冰箱开门边约束检查
    bool checkFridgeConstraint(const Rect& rect) const;

public:
    // 设置门占用障碍物
    void setDoorObstacle(const std::vector<Point>& poly);

    bool canPlace(
        const Rect& rect,
        const std::vector<Point>& room
    );


    void add(
        const Rect& rect
    );


    const std::vector<Rect>& getPlaced()
    {
        return placed;
    }

    bool tryPlace(
       Item& item,
        const std::vector<Point>& room
    );
};