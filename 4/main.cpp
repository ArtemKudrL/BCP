#include <iostream>
#include <cmath>

double besjn(double n, double x);

int main(int argc, char *argv[])
{
    double delta = 0;
    double eps = 1e-10;
    double step = 1e-4;

    std::cout << "Check started..." << std::endl;
    for (double x = 0; x < step; x += step)
    {
        double temp = (besjn(0, x + 0.5 * step) - besjn(0, x - 0.5 * step)) / step;
        temp += besjn(1, x);
        temp = fabs(temp);

        if (temp > delta)
            delta = temp;
        
        if (delta > eps)
        {
            std::cout << "Failed! on x = " << x << "; delta = " << delta << " > " << eps << std::endl;
            break;
        }
    }

    if (delta < eps)
        std::cout << "Approved! delta = " << delta << std::endl;

    return 0;
}

double besjn(double n, double x)
{
    double sum = 0;
    auto foo = [&n, &x](double t) { return cos(n * t - x * sin(t)); };

    const static double x1 = sqrt(5 - 2 * sqrt(10. / 7)) / 3;
    const static double x2 = sqrt(5 + 2 * sqrt(10. / 7)) / 3;
    const static double xn[] = {-x2 * M_PI, -x1 * M_PI, 0, x1 * M_PI, x2 * M_PI};

    const static double w0 = 128./255;
    const static double w1 = (322 + 13 * sqrt(70)) / 900;
    const static double w2 = (322 - 13 * sqrt(70)) / 900;
    const static double wn[] = {w2, w1, w0, w1, w2};

    double t = M_PI / 2;
    for(int n = 0; n < 5; ++n)
        sum += wn[n] * foo(t + xn[n]);

    return sum;
}
