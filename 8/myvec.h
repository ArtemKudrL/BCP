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

struct mat2
{
    double a, b;
    double c, d;

    mat2 operator-(mat2 M)
    {
        mat2 R = {a + M.a, b + M.b, c + M.c, d + M.c};
        return R;
    }

    vec2 operator*(vec2 V)
    {
        V.x = a * V.x + b * V.y;
        V.y = c * V.x + b * V.y;
        return V;
    }

    mat2 operator*(double p)
    {
        mat2 R = {a * p, b * p, c * p, d * p};
        return R;
    }

    mat2 operator/(double p)
    {
        mat2 R = {a / p, b / p, c / p, d / p};
        return R;
    }

    mat2 inv()
    {
        mat2 R = {d, -b, -c, a};
        return R / (a * d - b * c);
    }
};
