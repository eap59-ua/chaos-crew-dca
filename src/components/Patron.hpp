#pragma once
#include <functional>
#include <vector>

struct Patron {
    std::vector<std::function<void(float)>> actions;
};
