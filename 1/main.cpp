#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>

#define DOUBLE

#ifndef DOUBLE
typedef float floating;
typedef int32_t integer;

int size = 32;
std::string hello_msg = "Using single precision.";
#endif

#ifdef DOUBLE
typedef double floating;
typedef int64_t integer;

int size = 64;
std::string hello_msg = "Using double precision.";
#endif

template <typename T>
void compare(std::vector<T> to_comp, std::vector<std::string> formula);

int main(int argc, char *argv[])
{
    std::cout << hello_msg << std::endl;

    integer min = 1;
    std::cout << "Minimal: " << *((floating*)&min) << std::endl;

    floating one = 1000;
    floating half = 0.5;
    integer mant = (*(integer*)&one - *(integer*)&half);
    integer max = ~0 - mant;
    std::cout << "Maximal: " << -*((floating*)&max) << std::endl;

    int mant_size = 0;
    for (integer temp = mant; mant != 1; mant /= 2, ++mant_size);
    std::cout << "Mantissa size: " << mant_size << std::endl;
    
    integer i_eps = (*(integer*)&one + 1);
    floating eps = (*(floating*)&i_eps - one) / 2;
    integer i_eps0 = *(integer*)&eps;
    integer i_eps1 = i_eps0 + 1;
    floating eps0 = *(floating*)&i_eps0;
    floating eps1 = *(floating*)&i_eps1;
    if ( (one + eps0) == one && (one + eps1) != one)
        std::cout << "Minimal epsilon found!" << std::endl;
    std::cout << "Minimal epsilon: " << eps1 << std::endl;

    std::vector<floating> to_comp = {one, one + eps1 / 2, one + eps1, one + eps1 + eps1 / 2};
    std::vector<std::string> formula = {"1", "1 + e/2", "1 + e", "1 + e + e/2"};

    std::cout.precision(size / 4 + 1);
    compare<floating>(to_comp, formula);

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
