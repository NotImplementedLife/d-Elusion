#pragma once

#include "sprite.hpp"

class Cat : public Sprite
{
public:
    Cat();
    void updateMove(int toX,int toY);

    int mapX, mapY;

private:
    int frameId=0;
    int framesLen=0;
    int animIndex=0;

    int* crtKeyFrame = NULL;

    const int keyFramesDown[3] = { 0, 8, 16 };
    const int keyFramesUp[3] = { 1, 9, 17 };
    const int keyFramesLeft[3] = { 2, 10, 18 };
    const int keyFramesRight[3] = { 3, 11, 19 };

    const int keyFramesTopRight[2] = { 12, 20 };
    const int keyFramesTopLeft[2] = { 13, 21 };
    const int keyFramesBottomRight[2] = { 14, 22 };
    const int keyFramesBottomLeft[2] = { 15, 23 };

};
