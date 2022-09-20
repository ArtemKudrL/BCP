#include "gnuplot-iostream.h"
#include <map>
#include <unistd.h>
#include <vector>
#include <cmath>

std::vector<double> tridiag(std::vector<double> A, std::vector<double> B, std::vector<double> C, std::vector<double> F);

int main(int argc, char *argv[])
{
    Gnuplot gp;

    double L = 1;
    double sigma = 0.1;

    int N = 100;
    double dt = 0.02;
    double tmax = 5;

    std::vector<double> A(N);
    std::vector<double> B(N);
    std::vector<double> C(N);
    std::vector<double> F(N);
    std::vector<double> X(N);
    std::vector<double> U(N);

    double dx = L / N;
    double a = sigma * dt / dx / dx;
    for (int k = 0; k < N; ++k)
    {
        X[k] = k * dx;
        U[k] = 6 * X[k] * pow(1 - X[k] / L, 2);

        if (k == 0 || k == N - 1)
            continue;

        A[k] = - 0.5 * a;
        B[k] = 1 + a;
        C[k] = - 0.5 * a;
    }

    A[0] = 0;
    B[0] = 1;
    C[0] = 0;
    
    A[N - 1] = 0;
    B[N - 1] = 1;
    C[N - 1] = 0;

    std::map<double, double> xy;
    std::vector<std::pair<double, double>> Tt;
    xy[L] = 0;
    gp << "set xrange [0:" << L << "]\n";
    gp << "set yrange [0:1]\n";
    for (double t = 0; t <= tmax; t += dt)
    {
        double Tmax = 0;
        for (int k = 0; k < N; ++k)
        {
            xy[X[k]] = U[k];
            
            if (U[k] > Tmax)
                Tmax = U[k];
        }
        Tt.push_back(std::make_pair(t, Tmax));

        gp << "plot '-' with lines title 'solution'\n";
        gp.send(xy);
        gp.flush();
        ::usleep(dt * 1e+6);

        for (int k = 1; k < (N - 1); ++k)
        {
            F[k] = U[k] + 0.5 * a * (U[k + 1] - 2 * U[k] + U[k - 1]);
        }
        F[0] = 0;
        F[N - 1] = 0;

        U = tridiag(A, B, C, F);
    }

    std::vector<std::pair<double, double>> to_fit;
    for (int k = 0.75 * N; k < N; ++k)
        to_fit.push_back(Tt[k]);

    gp << "set xrange [0:" << tmax << "]\n";
    gp << "f(x) = a * exp(- b * x)\n";
    gp << "fit f(x) '-' via a,b\n";
    gp.send1d(to_fit);
    gp << "plot '-' with points title 'Tmax', f(x) with lines title 'exp fit'\n";
    gp.send(Tt);
    gp.flush();

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
