#pragma once

struct vec2
{
    double x;
    double y;

    vec2 operator*(double a)
    {
        vec2 R = {this->x * a, this->y * a};
        return R;
    }

    vec2 operator+(vec2 V)
    {
        vec2 R = {this->x + V.x, this->y + V.y};
        return R;
    }

    friend vec2 operator*(double a, vec2 V)
    {
        vec2 R = {a * V.x, a * V.y};
        return R;
    }
};
