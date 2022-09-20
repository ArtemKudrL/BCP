#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

unsigned long foo_eval_counter = 0;

double foo1(double param[2], double x)
{
    ++foo_eval_counter;

    double k = sqrt(2 * (param[0] - x));
    return k * tan(k * param[1]) - sqrt(2 * x);
}

double foo2(double param[2], double x)
{
    ++foo_eval_counter;

    double tg = tan(sqrt(2 * (param[0] - x)) * param[1]);
    return (param[0] - x) * tg * tg - x;
}

double derivative(double foo(double* param, double arg), double param[], double x, double h);
double dichotomy(double foo(double* param, double arg), double param[], double x1, double x2, double eps);
double iterations(double foo(double* param, double arg), double param[], double x0, double eps, double lam = 0);
double newton(double foo(double* param, double arg), double param[], double x0, double eps);

int main(int argc, char *argv[])
{
    double U0;
    double a;
    double eps;

    std::cout << "Enter U0: ";
    std::cin >> U0;
    std::cout << std::endl;

    std::cout << "Enter a: ";
    std::cin >> a;
    std::cout << std::endl;

    std::cout << "Enter eps: ";
    std::cin >> eps;
    std::cout << std::endl;

    std::cout.precision(- log10(eps) + 1);

    double xmin = U0 - M_PI * M_PI / 8 / a / a;
    if (xmin < 0) xmin = 0;
    double xmax = U0;
    double param[2] = {U0, a};

    unsigned long counter_temp = foo_eval_counter;
    std::cout << "Dichotomy answer: " << dichotomy(foo1, param, xmin, xmax, eps) << std::endl;
    std::cout << "Performed foo evaluations: " << foo_eval_counter - counter_temp << std::endl;

    counter_temp = foo_eval_counter;
    std::cout << "Iterations answer: " << iterations(foo1, param, 0.5 * (xmin + xmax), eps) << std::endl;
    std::cout << "Performed foo evaluations: " << foo_eval_counter - counter_temp << std::endl;

    counter_temp = foo_eval_counter;
    std::cout << "Newton answer: " << newton(foo1, param, 0.5 * (xmin + xmax), eps) << std::endl;
    std::cout << "Performed foo evaluations: " << foo_eval_counter - counter_temp << std::endl;

    return 0;
}

double derivative(double foo(double* param, double arg), double param[], double x, double h)
{
    return (foo(param, x + 0.5 * h) - foo(param, x - 0.5 * h)) / h;
}

double dichotomy(double foo(double* param, double arg), double param[], double x1, double x2, double eps)
{
    double xavg = 0.5 * (x1 + x2);
    double val;
    double sign = std::signbit(derivative(foo, param, xavg, x2 - x1)) ? - 1 : 1;

    if (x1 > x2)
        std::swap(x1, x2);

    while (x2 - x1 > eps)
    {
        val = sign * foo(param, xavg);

        if (val < 0 )
            x1 = xavg;
        else if (val > 0)
            x2 = xavg;
        else
            break;

        xavg = 0.5 * (x1 + x2);
    }

    return xavg;
}

double iterations(double foo(double* param, double arg), double param[], double x0, double eps, double lam)
{
    if (lam == 0)
    {
        lam = derivative(foo, param, x0, eps);

        double eps_temp = eps;
        while (lam == 0)
        {
            eps_temp *= 2;
            lam = derivative(foo, param, x0, eps_temp);
        }

        lam = 1 / lam;
    }

    double step = lam * foo(param, x0);

    while (fabs(step) > eps)
    {
        x0 -= step;

        step = lam * foo(param, x0);

        if (step == 0)
            break;
    }

    return x0;
}

double newton(double foo(double* param, double arg), double param[], double x0, double eps)
{
    double deriv = 0;
    double step = 0;

    do {
        if (step != 0)
            x0 -= step;

        deriv = derivative(foo, param, x0, eps);
        if (deriv == 0)
        {
            double eps_temp = eps;
            while (deriv == 0)
            {
                eps_temp *= 2;
                deriv = derivative(foo, param, x0, eps_temp);
            }
        }
        step = foo(param, x0) / deriv;

    } while (fabs(step) > eps);

    return x0;

}
