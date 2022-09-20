#include <cmath>
#include <utility>
#include <vector>
#include <map>

#define GNUPLOT_ENABLE_PTY
#include "gnuplot-iostream.h"

double foo(double x)
{
    return log(1 + x);
}

std::vector<double> solve(std::vector<double> mat, std::vector<double> vec);

int main(int argc, char *argv[])
{
    Gnuplot gp("gnuplot -persist");
    int n = 4;

    double mx, my;
    int mb = 1;

    while (n <= 15 && mb!= 3 && mb >= 0)
    {
        std::vector<double> mat(n * n);
        std::vector<double> vec(n);

        for (int k = 0; k < n; ++k)
        {
            double x = (double)k / n;

            for (int p = 0; p < n; ++p)
                mat[n * k + p] = pow((double)k / n, p);

            vec[k] = foo(x);
        }

        std::vector<double> ans = solve(mat, vec);
        std::map<double, double> xy;
        double ymin = 0, ymax = 0;

        for (double x = 0; x <= 1; x += 0.025)
        {
            double y = 0;
            for (int p = 0; p < n; ++p)
                y += ans[p] * pow(x, p);

            y -= foo(x);
            if (y > ymax)
                ymax = y;
            if (y < ymin)
                ymin = y;

            xy[1 + x] = y;
        }

        gp << "set xrange [1:2]\n";
        gp << "set yrange [" << ymin <<":" << ymax << "]\n";
        gp << "plot '-' with lines title 'P_{" << n << "}(x) - ln(x)'\n";
        gp.send(xy);

        gp.getMouse(mx, my, mb, "Left click for next polynom order. Right clck to close window.");
        ++n;
    }

    return 0;
}

std::vector<double> solve(std::vector<double> M, std::vector<double> V)
{
    int N = V.size();
    double temp;


    for (int j = 0; j < N; ++j)
    {
        temp = M[j * N + j];
        int imax = j;
        for(int i = j + 1; i < N; ++i)
        {
            if (M[i * N + j] > temp)
            {
                temp = M[i * N + j];
                imax = i;
            }
        }

        for (int k = j; k < N; ++k)
            std::swap(M[j * N + k], M[imax * N + k]);
        std::swap(V[j], V[imax]);

        temp = M[j * N + j];
        for (int k = j; k < N; ++k)
            M[j * N + k] /= temp;
        V[j] /= temp;

        for (int i = j + 1; i < N; ++i)
        {
            temp = M[i * N + j];
            for (int k = j; k < N; ++k)
            {
                M[i * N + k] -= temp * M[j * N + k];
            }

            V[i] -= temp * V[j];
        }
    }

    for (int j = N - 1; j >= 0; --j)
    {
        for (int i = j - 1; i >= 0; --i)
        {
            V[i] -= M[i * N + j] * V[j];
            M[i * N + j] = 0;
        }
    }

    return V;
}
