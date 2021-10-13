#pragma once

#include "sprite.hpp"

class Car
{
public:
    Car();

    void setX(int x);
    void setOam();
private:
    Sprite front, back, wheel1, wheel2;
    s16 wheelsAngle=0;
};
