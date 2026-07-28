#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "JsonLoader.h"
#include "Geometry.h"
#include "PlacementManager.h"

int main()
{
    // 1. 加载JSON房间数据
    RoomData room = JsonLoader::load("example1.json");
    std::cout << "加载完成，待摆放物品数量：" << room.items.size() << std::endl;

    // 2. 内开门生成占用障碍物
    PlacementManager manager;
    if (room.isOpenInward)
    {
        std::cout << "内开门，生成门占用区域..." << std::endl;
        Point p0 = room.door[0];
        Point p1 = room.door[1];

        double dx = p1.x - p0.x;
        double dy = p1.y - p0.y;
        double doorLen = std::sqrt(dx * dx + dy * dy);

        // 计算两个法向量
        double n1x = -dy / doorLen;
        double n1y = dx / doorLen;
        double n2x = dy / doorLen;
        double n2y = -dx / doorLen;

        // 判断内侧法向量（指向房间内部）
        double midX = (p0.x + p1.x) / 2.0;
        double midY = (p0.y + p1.y) / 2.0;
        Point testPoint = { midX + n1x * 1.0, midY + n1y * 1.0 };

        double nx, ny;
        if (Geometry::pointInPolygon(testPoint, room.boundary))
        {
            nx = n1x;
            ny = n1y;
        }
        else
        {
            nx = n2x;
            ny = n2y;
        }

        // 生成门占用矩形（深度1000mm，对应1x1空间）
        const double DOOR_DEPTH = 1000.0;
        std::vector<Point> doorPoly;
        doorPoly.push_back(p0);
        doorPoly.push_back(p1);
        doorPoly.push_back({ p1.x + nx * DOOR_DEPTH, p1.y + ny * DOOR_DEPTH });
        doorPoly.push_back({ p0.x + nx * DOOR_DEPTH, p0.y + ny * DOOR_DEPTH });

        manager.setDoorObstacle(doorPoly);
    }
    else
    {
        std::cout << "外开门，不占用室内空间" << std::endl;
    }

    // 3. 按面积从大到小排序（保留原索引，输出保持输入顺序）
    std::vector<int> indices(room.items.size());
    for (size_t i = 0; i < indices.size(); ++i)
        indices[i] = static_cast<int>(i);

    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        double areaA = room.items[a].length * room.items[a].width;
        double areaB = room.items[b].length * room.items[b].width;
        return areaA > areaB;
        });

    // 4. 依次摆放物品
    bool allSuccess = true;
    for (int idx : indices)
    {
        Item& item = room.items[idx];
        bool ok = manager.tryPlace(item, room.boundary);
        if (!ok)
        {
            std::cout << "摆放失败：" << item.name << std::endl;
            allSuccess = false;
            break;
        }
        std::cout << "已摆放：" << item.name << std::endl;
    }

    // 5. 输出结果
    std::cout << "\n========== 摆放结果 ==========" << std::endl;
    std::cout << "是否可行：" << (allSuccess ? "可行" : "不可行") << std::endl;
    std::cout << "\n物品详情（按输入顺序）：" << std::endl;

    for (const auto& item : room.items)
    {
        std::cout << "【" << item.name << "】" << std::endl;
        std::cout << "  中心点：(" << item.centerX << ", " << item.centerY << ")" << std::endl;
        std::cout << "  旋转角度：" << item.angle << "°" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}