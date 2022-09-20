#include "gnuplot-iostream.h"
#include <map>
#include <numeric>
#include <vector>
#include <cmath>

double foo(double x)
{
    return 0.5 * x * x;
}

std::vector<double> tridiag(std::vector<double> A, std::vector<double> B, std::vector<double> C, std::vector<double> F);

int main(int argc, char *argv[])
{
    Gnuplot gp;

    double a = 10;

    int N = 100;

    std::vector<double> A(N);
    std::vector<double> B(N);
    std::vector<double> C(N);
    std::vector<double> D(N);
    std::vector<double> X(N);
    std::vector<double> U(N);

    double dx = 2 * a / (N - 1);
    for (int k = 0; k < N; ++k)
    {
        X[k] = k * dx - a;
        U[k] = exp(- X[k] * X[k]);

        A[k] = - 0.5 / dx / dx;
        B[k] = foo(X[k]) + 1 / dx / dx;
        C[k] = - 0.5 / dx / dx;
    }

    double E = 0;
    for (int i = 0; i < 20; ++i)
    {
        std::transform(B.begin(), B.end(), D.begin(), [&E](double H) { return H - E; });

        U = tridiag(A, D, C, U);

        double norm = std::inner_product(U.begin(), U.end(), U.begin(), 0.);
        norm = sqrt(norm);
        std::transform(U.begin(), U.end(), U.begin(), [&norm](double x) { return x / norm; });

        for(int k = 1; k < (N - 1); ++k)
        {
            D[k] = A[k] * U[k - 1] + B[k] * U[k] + C[k] * U[k + 1];
        }
        D[0] = B[0] * U[0] + C[0] * U[1];
        D[N - 1] = A[N - 1] * U[N - 2] + B[N - 1] * U[N - 1];

        E = std::inner_product(U.begin(), U.end(), D.begin(), 0.);
    }

    std::map<double, double> psi;
    for (int k = 0; k < N; ++k)
    {
        psi[X[k]] = fabs(U[k]) / sqrt(dx);
    }
    gp << "set xrange [-3:3]\n";
    gp << "set yrange [-1:1]\n";
    gp << "plot '-' with lines title 'eval', exp(-x**2 / 2) / sqrt(2) with lines title 'solution'\n";
    gp.send(psi);

    std::cout << "Energy: " << E << std::endl;

    return 0;
}

std::vector<double> tridiag(std::vector<double> A, std::vector<double> B, std::vector<double> C, std::vector<double> F)
{
    int N = F.size();

    for (int i = 1; i < N; ++i)
    {
        double w = A[i] / B[i - 1];
        B[i] -= w * C[i - 1];
        F[i] -= w * F[i - 1];
    }

    std::vector<double> X(N);
    X[N - 1] = F[N - 1] / B[N - 1];
    for (int i = N - 2; i >= 0; --i)
    {
        X[i] = (F[i] - C[i] * X[i + 1]) / B[i];
    }

    return X;
}
