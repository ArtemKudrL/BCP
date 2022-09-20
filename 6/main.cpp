#include <map>
#include "gnuplot-iostream.h"

double foo(double t, double x)
{
    return -x;
}

double euler1(double foo(double, double), double x, double t, double h);
double rk2(double foo(double, double), double x, double t, double h);
double rk4(double foo(double, double), double x, double t, double h);

int main(int argc, char *argv[])
{
    Gnuplot gp;

    std::map<double, double> xy_1, xy_2, xy_4;

    double step = 0.5;
    double x1, x2, x4;
    x1 = x2 = x4 = 1;
    for (double t = 0; t <= 3; t += step)
    {
        xy_1[t] = x1;
        xy_2[t] = x2;
        xy_4[t] = x4;

        x1 = euler1(foo, x1, t, step);
        x2 = rk2(foo, x2, t, step);
        x4 = rk4(foo, x4, t, step);
    }

    gp << "set xrange [0:3]\n";
    gp << "set yrange [0:1]\n";
    gp << "plot '-' with lines title 'euler', '-' with lines title 'r-k 2', '-' with lines title 'r-k 4', exp(-x) title 'solution'\n";
    gp.send(xy_1).send(xy_2).send(xy_4);

    return 0;
}

double euler1(double foo(double, double), double x, double t, double h)
{
    return x + foo(t, x) * h;
}

double rk2(double foo(double, double), double x, double t, double h)
{
    static double k1;
    k1 = foo(t, x);
    return x + 0.5 * (foo(t, x) + foo(t + h, x + k1 * h)) * h;
}

double rk4(double foo(double, double), double x, double t, double h)
{
    static double k1;
    static double k2;
    static double k3;
    static double k4;

    k1 = foo(t, x);
    k2 = foo(t + 0.5 * h, x + 0.5 * k1 * h);
    k3 = foo(t + 0.5 * h, x + 0.5 * k2 * h);
    k4 = foo(t + h, x + k3 * h);

    return x + (k1 + 2 * k2 + 2 * k3 + k4) * h / 6;
}
