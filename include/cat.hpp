#pragma once

#include "sprite.hpp"

class Cat : public Sprite
{
public:
    Cat();
    void init();
    void play_dead();
    void updateMove(int toX,int toY);

    void reset();

    int mapX, mapY;

private:
    int animTimer=0;
    int framesLen=0;
    int animIndex=0;

    int* crtKeyFrame = NULL;

    const int keyFramesDown[6] = { 0, 8, 16, 0, 8, 16 };
    const int keyFramesUp[6] = { 1, 9, 17, 1, 9, 17 };
    const int keyFramesLeft[6] = { 2, 10, 18, 2, 10, 18 };
    const int keyFramesRight[6] = { 3, 11, 19, 3, 11, 19 };

    const int keyFramesTopRight[6] = { 12, 20, 12, 20, 12, 20 };
    const int keyFramesTopLeft[6] = { 13, 21, 13, 21, 13, 21 };
    const int keyFramesBottomRight[6] = { 14, 22, 14, 22, 14, 22 };
    const int keyFramesBottomLeft[6] = { 15, 23, 15, 23, 15, 23 };

    const int keyFramesDead[4] = {4, 5, 6, 7};
};
