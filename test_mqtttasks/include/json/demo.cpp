#include "nlohmann/json.hpp"
#include <iostream>
using json = nlohmann::json;

int main()
{
    json j = {{"pi", 3.141}, {"foo", "bar"}};
    std::cout << j << std::endl;

    json j2;
    j2["pi"] = 3.141;
    j2["foo"] = "bar";
    std::cout << j2 << std::endl;
    return 0;
}