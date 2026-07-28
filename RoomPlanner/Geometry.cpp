#include "Geometry.h"
#include <limits>

bool Geometry::pointInPolygon(
    const Point& p,
    const std::vector<Point>& poly
)
{

    bool inside = false;


    int n = poly.size();


    for (int i = 0, j = n - 1; i < n; j = i++)
    {

        bool intersect =
            ((poly[i].y > p.y) != (poly[j].y > p.y))
            &&
            (
                p.x <
                (poly[j].x - poly[i].x)
                *
                (p.y - poly[i].y)
                /
                (poly[j].y - poly[i].y)
                +
                poly[i].x
                );


        if (intersect)
            inside = !inside;

    }


    return inside;

}

std::vector<Point> Geometry::getCorners(
    const Rect& rect
)
{

    double halfW =
        rect.width / 2.0;


    double halfH =
        rect.height / 2.0;



    return
    {

        {
            rect.centerX - halfW,
            rect.centerY - halfH
        },


        {
            rect.centerX + halfW,
            rect.centerY - halfH
        },


        {
            rect.centerX + halfW,
            rect.centerY + halfH
        },


        {
            rect.centerX - halfW,
            rect.centerY + halfH
        }

    };

}

bool Geometry::rectInsidePolygon(
    const Rect& rect,
    const std::vector<Point>& polygon
)
{

    auto corners =
        getCorners(rect);


    for (auto& p : corners)
    {

        if (!pointInPolygon(
            p,
            polygon))
        {
            return false;
        }

    }


    return true;

}

bool Geometry::rectOverlap(
    const Rect& a,
    const Rect& b
)
{

    double aLeft =
        a.centerX - a.width / 2;


    double aRight =
        a.centerX + a.width / 2;


    double aBottom =
        a.centerY - a.height / 2;


    double aTop =
        a.centerY + a.height / 2;



    double bLeft =
        b.centerX - b.width / 2;


    double bRight =
        b.centerX + b.width / 2;


    double bBottom =
        b.centerY - b.height / 2;


    double bTop =
        b.centerY + b.height / 2;



    if (aRight <= bLeft ||
        bRight <= aLeft ||
        aTop <= bBottom ||
        bTop <= aBottom)
    {
        return false;
    }


    return true;

}

BoundingBox Geometry::getBoundingBox(
    const std::vector<Point>& polygon
)
{
    BoundingBox box;


    box.minX = std::numeric_limits<double>::max();
    box.maxX = std::numeric_limits<double>::lowest();

    box.minY = std::numeric_limits<double>::max();
    box.maxY = std::numeric_limits<double>::lowest();



    for (const auto& p : polygon)
    {
        if (p.x < box.minX)
            box.minX = p.x;


        if (p.x > box.maxX)
            box.maxX = p.x;


        if (p.y < box.minY)
            box.minY = p.y;


        if (p.y > box.maxY)
            box.maxY = p.y;
    }


    return box;
}

bool Geometry::lineIntersect(
    const Point& a1,
    const Point& a2,
    const Point& b1,
    const Point& b2
)
{
    auto cross = [](const Point& o, const Point& a, const Point& b) {
        return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
        };

    double c1 = cross(a1, a2, b1);
    double c2 = cross(a1, a2, b2);
    double c3 = cross(b1, b2, a1);
    double c4 = cross(b1, b2, a2);

    return (c1 * c2 < 0) && (c3 * c4 < 0);
}

bool Geometry::rectOverlapPolygon(
    const Rect& rect,
    const std::vector<Point>& poly
)
{
    auto corners = getCorners(rect);
    double rectMinX = rect.centerX - rect.width / 2.0;
    double rectMaxX = rect.centerX + rect.width / 2.0;
    double rectMinY = rect.centerY - rect.height / 2.0;
    double rectMaxY = rect.centerY + rect.height / 2.0;

    // 1. 矩形顶点在多边形内
    for (const auto& p : corners)
    {
        if (pointInPolygon(p, poly)) return true;
    }

    // 2. 多边形顶点在矩形内
    for (const auto& p : poly)
    {
        if (p.x >= rectMinX && p.x <= rectMaxX &&
            p.y >= rectMinY && p.y <= rectMaxY)
            return true;
    }

    // 3. 边与边相交
    int polySize = poly.size();
    for (int i = 0; i < 4; ++i)
    {
        Point r1 = corners[i];
        Point r2 = corners[(i + 1) % 4];
        for (int j = 0; j < polySize; ++j)
        {
            Point p1 = poly[j];
            Point p2 = poly[(j + 1) % polySize];
            if (lineIntersect(r1, r2, p1, p2)) return true;
        }
    }
    return false;
}