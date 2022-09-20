#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>

double foo(int n)
{
    return n % 2 == 0 ? 1. / n : - 1. / n;
}

template <typename T>
void compare(std::vector<T> to_comp, std::vector<std::string> formula);

int main(int argc, char *argv[])
{
    std::cout.precision(16);
    int N = 1e+5;

    std::vector<double> sum(4);
    std::fill(sum.begin(), sum.end(), 0);

    auto ptr = sum.begin();

    // from begin to end
    for (int n = 1; n < N; ++n)
        *ptr += foo(n);
    ++ptr;

    // from end to begin
    for (int n = N - 1; n > 0; --n)
        *ptr += foo(n);
    ++ptr;

    // from begin to end separate
    double positive = 0,
           negative = 0;
    for (int n = 1; n < N; ++n)
    {
        double temp = foo(n);
        if (temp > 0)
            positive += temp;
        else
            negative += temp;
    }
    *(ptr++) = positive + negative;

    // from end to begin separate
    positive = 0;
    negative = 0;
    for (int n = N - 1; n > 0; --n)
    {
        double temp = foo(n);
        if (temp > 0)
            positive += temp;
        else
            negative += temp;
    }
    *(ptr++) = positive + negative;

    std::vector<std::string> formula = {"1,2..N", "N,N-1..1", "1,3..N-1 + 2,4..N", "N,N-2..2 + N-1,N-3..1"};
    compare<double>(sum, formula);

    return 0;
}

template <typename T>
void compare(std::vector<T> to_comp, std::vector<std::string> formula)
{
    std::vector<std::pair<T, std::string>> arr(to_comp.size());

    auto it1 = to_comp.begin();
    auto it2 = formula.begin();
    for (auto &it : arr)
    {
        it.first = *(it1++);
        it.second = *(it2++);
    }

    auto comp_foo = [](const std::pair<T, std::string> &a, const std::pair<T, std::string> &b) -> bool { return a.first > b.first; };
    std::sort(arr.begin(), arr.end(), comp_foo);

    std::cout << arr.front().second << ": " << arr.front().first << std::endl;
    for (auto it = arr.begin(); it != arr.end() - 1; ++it)
    {
        if (comp_foo(*it, *(it + 1)))
            std::cout << " > " << std::endl;
        else
            std::cout << " == " << std::endl;

        std::cout << (it + 1)->second << ": " << (it + 1)->first << std::endl;
    }
}
