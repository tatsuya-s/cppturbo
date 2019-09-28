#include <iostream>
#include "turbo_colormap.hpp"

int main()
{
    // Get the turbo color as simply
    // param[in] index in [0   - 255]
    // return    color in [0.0 - 1.0]

    const turbo::Color<float> color = turbo::GetColor(128);
    std::cout << color.r << ", " << color.g << ", " << color.b << std::endl;


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

    std::cout << floats  << std::endl;
    std::cout << doubles << std::endl;
    std::cout << uchars  << std::endl;
    std::cout << ints    << std::endl;

    return 0;
}