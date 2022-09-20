#include "gnuplot-iostream.h"
#include <map>
#include <vector>
#include <cmath>

std::vector<double> tridiag(std::vector<double> A, std::vector<double> B, std::vector<double> C, std::vector<double> F);

int main(int argc, char *argv[])
{
    Gnuplot gp;

    int N = 100;

    std::vector<double> A(N);
    std::vector<double> B(N);
    std::vector<double> C(N);
    std::vector<double> F(N);
    std::vector<double> X(N);

    double step = M_PI / N;
    for (int k = 0; k < N; ++k)
    {
        X[k] = k * step;

        A[k] = 1 / step / step;
        B[k] = - 2 / step / step;
        C[k] = 1 / step / step;
        F[k] = sin(X[k]);
    }

    double a1 = 1, b1 = 1, c1 = 1;
    double an = 1, bn = 1, cn = - 1;

    A[0] = 0;
    B[0] = a1 - b1 / step;
    C[0] = b1 / step;
    F[0] = c1;
    
    A[N - 1] = - bn / step;
    B[N - 1] = an + bn / step;
    C[N - 1] = 0;
    F[N - 1] = cn;

    std::vector<double> Y = tridiag(A, B, C, F);
    std::map<double, double> xy;
    double ymin = 0, ymax = 0;
    for (int k = 0; k < N; ++k)
    {
        xy[X[k]] = Y[k];

        if (Y[k] < ymin)
            ymin = Y[k];

        if (Y[k] > ymax)
            ymax = Y[k];
    }

    gp << "set xrange [0:pi]\n";
    gp << "set yrange [" << ymin << ":" << ymax << "]\n";
    gp << "plot '-' with lines title 'solution'\n";
    gp.send(xy);

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
