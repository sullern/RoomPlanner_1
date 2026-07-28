#include "PlacementManager.h"

#include "Geometry.h"
#include <cmath>


bool PlacementManager::canPlace(
    const Rect& rect,
    const std::vector<Point>& room
)
{

    // 1. 是否在房间内

    if (!Geometry::rectInsidePolygon(
        rect,
        room))
    {
        return false;
    }



    // 2. 是否碰撞已有物体

    for (auto& old : placed)
    {

        if (Geometry::rectOverlap(
            rect,
            old))
        {
            return false;
        }

    }


    return true;

}



void PlacementManager::add(
    const Rect& rect
)
{
    placed.push_back(rect);
}

// 辅助：根据物品、中心点、角度生成轴对齐矩形
static Rect makeRect(const Item& item, double cx, double cy, double angle)
{
    Rect r;
    r.centerX = cx;
    r.centerY = cy;
    r.angle = angle;
    r.itemName = item.name;

    if (angle == 0.0)
    {
        r.width = item.length;
        r.height = item.width;
    }
    else // 90度旋转：交换宽高，保持轴对齐
    {
        r.width = item.width;
        r.height = item.length;
    }
    return r;
}

bool PlacementManager::tryPlace(Item& item, const std::vector<Point>& room)
{
    BoundingBox box = Geometry::getBoundingBox(room);
    const double SEARCH_STEP = 50.0; // 搜索步长，越小精度越高、速度越慢

    // 尝试两个旋转角度
    std::vector<double> angles = { 0.0, 90.0 };

    for (double angle : angles)
    {
        double w, h;
        if (angle == 0.0)
        {
            w = item.length;
            h = item.width;
        }
        else
        {
            w = item.width;
            h = item.length;
        }

        // 中心点遍历范围
        double startX = box.minX + w / 2.0;
        double endX = box.maxX - w / 2.0;
        double startY = box.minY + h / 2.0;
        double endY = box.maxY - h / 2.0;

        // 从左下角开始贪心搜索第一个可行位置
        for (double x = startX; x <= endX; x += SEARCH_STEP)
        {
            for (double y = startY; y <= endY; y += SEARCH_STEP)
            {
                Rect rect = makeRect(item, x, y, angle);
                if (canPlace(rect, room))
                {
                    add(rect);
                    item.centerX = x;
                    item.centerY = y;
                    item.angle = angle;
                    return true;
                }
            }
        }
    }
    return false;
}



void PlacementManager::setDoorObstacle(const std::vector<Point>& poly)
{
    doorObstacle = poly;
    hasDoorObstacle = true;
}

// 冰箱开门边约束：开门侧不能有物品贴合
bool PlacementManager::checkFridgeConstraint(const Rect& rect) const
{
    const double MIN_GAP = 1.0; // 开门边最小间隙，单位mm

    double rectMinX = rect.centerX - rect.width / 2.0;
    double rectMaxX = rect.centerX + rect.width / 2.0;
    double rectMinY = rect.centerY - rect.height / 2.0;
    double rectMaxY = rect.centerY + rect.height / 2.0;

    for (const auto& old : placed)
    {
        // 只检查冰箱类型
        if (old.itemName.find("fridge") == std::string::npos)
            continue;

        double oldMinX = old.centerX - old.width / 2.0;
        double oldMaxX = old.centerX + old.width / 2.0;
        double oldMinY = old.centerY - old.height / 2.0;
        double oldMaxY = old.centerY + old.height / 2.0;

        // 假设：0°时开门边在顶部(y+)，90°时在右侧(x+)
        if (old.angle == 0.0)
        {
            // 新物品在冰箱开门侧且间隙不足
            if (rectMinY < oldMaxY + MIN_GAP && rectMaxY > oldMaxY)
            {
                if (rectMaxX > oldMinX && rectMinX < oldMaxX)
                    return false;
            }
        }
        else
        {
            if (rectMinX < oldMaxX + MIN_GAP && rectMaxX > oldMaxX)
            {
                if (rectMaxY > oldMinY && rectMinY < oldMaxY)
                    return false;
            }
        }
    }
    return true;
}