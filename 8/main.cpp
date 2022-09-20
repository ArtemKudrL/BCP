#include <utility>
#include <vector>
#include "gnuplot-iostream.h"

#include "myvec.h"

vec2 euler(mat2 A, vec2 X, double h);
vec2 i_euler(mat2 A, vec2 X, double h);

int main(int argc, char *argv[])
{
    Gnuplot gp;

    mat2 M = {998, 1998,
            - 999, - 1999};

    std::vector<std::pair<double, double>> xy1, xy2;
    double step = 1e-4;
    vec2 X1, X2;
    X1 = X2 = {1, 1};
    for (double t = 0; t < 1; t += step)
    {
        xy1.push_back(std::make_pair(X1.x, X1.y));
        xy2.push_back(std::make_pair(X2.x, X2.y));

        X1 = euler(M, X1, step);
        X2 = i_euler(M, X2, step);
    }

    gp << "set xrange [-2:2]\n";
    gp << "set yrange [-2:2]\n";
    gp << "plot '-' with lines title 'simple', '-' with lines title 'implicit'\n";
    gp.send1d(xy1).send1d(xy2);

    return 0;
}

vec2 euler(mat2 A, vec2 X, double h)
{
    return X + A * X;
}

vec2 i_euler(mat2 A, vec2 X, double h)
{
    static mat2 E = {1, 0,
                             0, 1};
    return (E - A * h).inv() * X;
}
