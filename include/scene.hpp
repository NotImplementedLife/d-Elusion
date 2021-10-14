#pragma once

#include "cat.hpp"
#include "car.hpp"
#include "cursor.hpp"
#include "panel.hpp"

class Scene
{
private:
    Cat cat;
    Car car;
    Cursor cursor;
    Panel panel;

public:
    Scene();
    void init(bool intro=false);

    void run();

    void game_over();

    void cat_moved(int dx,int dy);
};
