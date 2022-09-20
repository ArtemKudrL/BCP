#include <iostream>
#include <vector>
#include <string>
#include <cmath>

double foo_a(double x)
{
    return 1 / (1 + x * x);
}

double foo_b(double x)
{
    return pow(x, 1. / 3.) * exp(sin(x));
}

double trapezoidal(double foo(double), double xmin, double xmax, double intervals);
double simpson(double foo(double), double xmin, double xmax, double intervals);

int main(int argc, char *argv[])
{
    std::vector<double(*)(double)> foos = {foo_a, foo_b};
    std::vector<std::pair<double, double>> limits = {{- 1, 1}, {0, 1}};
    std::vector<std::string> names = {"A", "B"};
    std::vector<int> intervals = {4, 6, 8};

    for (int i = 0; i < foos.size(); ++i)
    {
        std::cout << "Integral " << names[i] << ":" << std::endl;
        for (auto N : intervals)
        {
            std::cout << "\t" << N << " bins:" << std::endl;
            std::cout << "\tTrap: " << trapezoidal(foos[i], limits[i].first, limits[i].second, N) << std::endl;
            std::cout << "\tSimp: " << simpson(foos[i], limits[i].first, limits[i].second, N) << std::endl;
        }
    }

    return 0;
}

double trapezoidal(double foo(double), double xmin, double xmax, double N)
{
    double sum = 0;
    double bin = (xmax - xmin) / N;

    double x = xmin;
    for (int n = 1; n < N; ++n)
    {
        x += bin;
        sum += foo(x);
    }

    sum += 0.5 * (foo(xmin) + foo(xmax));
    sum *= bin;

    return sum;
}

double simpson(double foo(double), double xmin, double xmax, double N)
{
    double sum = 0;
    double bin = 0.5 * (xmax - xmin) / N;

    double x = xmin;
    for (int n = 1; n < N; ++n)
    {
        x += bin;
        sum += 2 * foo(x);

        x += bin;
        sum += foo(x);
    }

    sum = foo(xmin) + 2 * sum + 4 * foo(xmax - bin) + foo(xmax);
    sum *= bin / 3;

    return sum;
}
