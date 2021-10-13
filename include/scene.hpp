#pragma once

#include "cat.hpp"
#include "car.hpp"
#include "panel.hpp"

class Scene
{
private:
    Cat cat;
    Car car;
    Panel panel;

public:
    Scene();
    void init();

    void run();

    void game_over();
};
