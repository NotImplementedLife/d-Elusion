#pragma once

#include "sprite.hpp"

class LvlSelector
{
private:
    Sprite icons[12];
    int hitTest(int x,int y);
public:
    void init();
    int execute();
};
