#pragma once

#include <cassert>


namespace glgame2d {


struct Rect {
    bool collides(const Rect& other) const
    {
        // Sign convention: Taking upwards and rightwards as positive

        // Sanity check dimensions
        assert(w > 0 && h > 0);
        assert(other.w > 0 && other.h > 0);

        bool overlapX = x < other.x + other.w   // this left bound < other right bound
                 && x + w > other.x;            // this right bound > other left bound

        bool overlapY = y > other.y - other.h   // this top bound > other bottom bound (above)
                 && y - h < other.y;            // this bottom bound < other top bound (below)

        return overlapX && overlapY;
    }

    float x, y;
    float w, h;
};


} // namespace glgame2d
