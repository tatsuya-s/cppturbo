#include <iostream>
#include "cppturbo.hpp"

int main()
{
    // Get the turbo color with selected format
    // param[in]  target value in [0.0 - 1.0] or [0 - 255]
    // param[out] turbo color in float, double, uchar, or int

    turbo::Color3f floats;
    turbo::Color3d doubles;
    turbo::Color3b uchars;
    turbo::Color3i ints;

    const double val_d = 0.5;
    const int    val_i = 128;

    turbo::GetColor(val_i, floats);
    turbo::GetColor(val_d, doubles);
    turbo::GetColor(val_i, uchars);
    turbo::GetColor(val_d, ints);

    std::cout << floats  << std::endl; // (r:0.64362, g:0.98999, b:0.23356)
    std::cout << doubles << std::endl; // (r:0.64362, g:0.98999, b:0.23356)
    std::cout << uchars  << std::endl; // (r:164, g:252, b:60)
    std::cout << ints    << std::endl; // (r:164, g:252, b:60)

    return 0;
}