#include <vector>
#include "gnuplot-iostream.h"

#include "myvec.h"

vec2 foo(vec2 X)
{
    vec2 V = { 10 * X.x - 2 * X.x * X.y,
              2 * X.x * X.y - 10 * X.y };

    return V;
}

vec2 rk2(vec2 foo(vec2), vec2 X, double h);

int main(int argc, char *argv[])
{
    Gnuplot gp;

    std::vector<std::pair<double, double>> xy;
    double step = 0.01;
    vec2 X = {1, 1};
    for (double t = 0; t < 1; t += step)
    {
        xy.push_back(std::make_pair(X.x, X.y));

        X = rk2(foo, X, step);
    }

    gp << "set xrange [0:21]\n";
    gp << "set yrange [0:21]\n";
    gp << "plot '-' with lines title 'trajectory'\n";
    gp.send1d(xy);
    gp.flush();

    return 0;
}

vec2 rk2(vec2 foo(vec2), vec2 X, double h)
{
    static vec2 K;
    static vec2 R;

    K = foo(X);
    R = X + 0.5 * (K + foo(X + K * h)) * h;

    return R;
}
