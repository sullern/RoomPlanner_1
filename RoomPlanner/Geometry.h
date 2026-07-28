#pragma once

#include <vector>

#include "Point.h"
#include "Rect.h"
#include "BoundingBox.h"

class Geometry
{

public:


    static bool pointInPolygon(
        const Point& p,
        const std::vector<Point>& polygon
    );



    static std::vector<Point> getCorners(
        const Rect& rect
    );



    static bool rectInsidePolygon(
        const Rect& rect,
        const std::vector<Point>& polygon
    );

    static bool rectOverlap(
        const Rect& a,
        const Rect& b
    );

    static BoundingBox getBoundingBox(
        const std::vector<Point>& polygon
    );

    static bool lineIntersect(
        const Point& a1,
        const Point& a2,
        const Point& b1,
        const Point& b2
    );

    static bool rectOverlapPolygon(
        const Rect& rect,
        const std::vector<Point>& poly
    );
};