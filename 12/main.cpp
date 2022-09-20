#include "gnuplot-iostream.h"
#include <algorithm>
#include <cmath>
#include <complex>
#include <map>
#include <vector>

double rect(int n, int N)
{
    return (n >= 0 && n < N) ? 1 : 0;
}

double hann(int n, int N)
{
    return 0.5 * (1 - cos(2 * M_PI * n / (N - 1.)));
}

std::vector<std::complex<double>> stft(double w(int, int), std::vector<double> x);

int main(int argc, char *argv[])
{
    Gnuplot gp;

    int N = 100;

    double a0 = 1;
    double a1 = 0.002;
    double w0 = 5.1;
    double w1 = 5 * w0;
    double T = 2 * M_PI;

    std::vector<double> f(N);
    for (int n = 0; n < N; ++n)
    {
        double t = T * n / N;
        f[n] = a0 * sin(w0 * t) + a1 * sin(w1 * t);
    }

    auto rect_ft = stft(rect, f);
    auto hann_ft = stft(hann, f);

    std::vector<std::pair<double, double>> rft, hft;
    double max;
    for (int n = 0; n < N; ++n)
    {
        double w = 2 * M_PI * n / T;
        rft.push_back(std::make_pair(w, std::pow(std::abs(rect_ft[n]), 2)));
        hft.push_back(std::make_pair(w, std::pow(std::abs(hann_ft[n]), 2)));

        if (rft[n].second > max)
            max = rft[n].second;
        if (hft[n].second > max)
            max = hft[n].second;
    }

    gp << "set xrange [" << 0 << ":" << 2 * M_PI * (N - 1) / T << "]\n";
    gp << "set yrange [" << 0 << ":" << max << "]\n";
    gp << "plot '-' with lines title 'rect', '-' with lines title 'hann'\n";
    gp.send1d(rft).send1d(hft);

    return 0;
}

std::vector<std::complex<double>> stft(double w(int, int), std::vector<double> x)
{
    using namespace std::complex_literals;

    int N = x.size();
    double omega = 2 * M_PI / N;

    std::vector<std::complex<double>> X(N);
    for (int k = 0; k < N; ++k)
    {
        std::complex<double> sum;
        for (int n = 0; n < N; ++n)
        {
            sum += x[n] * w(n - k, N) * std::exp(- 1i * omega * (double)(k * n));
        }
        X[k] = sum;
    }

    return X;
}
