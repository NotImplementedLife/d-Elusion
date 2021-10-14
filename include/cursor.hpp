#pragma once

#include "sprite.hpp"

class Cursor : public Sprite
{
public:
    Cursor();
    void init();

    const int Icon_Arrow = 0;
    const int Icon_Fish = 1;
    const int Icon_Flag = 2;

    void setArrow();
    void setFish();
    void setFlag();

    bool isArrow();
    bool isFish();
    bool isFlag();
};
