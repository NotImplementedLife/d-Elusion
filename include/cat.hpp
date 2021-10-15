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

    const int tilesStride=10;
    const int keyFramesDown[6] =
    {
        0 * tilesStride + 0,
        1 * tilesStride + 0,
        2 * tilesStride + 0,
        0 * tilesStride + 0,
        1 * tilesStride + 0,
        2 * tilesStride + 0
    };
    const int keyFramesUp[6] =
    {
        0 * tilesStride + 1,
        1 * tilesStride + 1,
        2 * tilesStride + 1,
        0 * tilesStride + 1,
        1 * tilesStride + 1,
        2 * tilesStride + 1
    };
    const int keyFramesLeft[6] =
    {
        0 * tilesStride + 2,
        1 * tilesStride + 2,
        2 * tilesStride + 2,
        0 * tilesStride + 2,
        1 * tilesStride + 2,
        2 * tilesStride + 2
    };
    const int keyFramesRight[6] =
    {
        0 * tilesStride + 3,
        1 * tilesStride + 3,
        2 * tilesStride + 3,
        0 * tilesStride + 3,
        1 * tilesStride + 3,
        2 * tilesStride + 3
    };

    const int keyFramesTopRight[6] =
    {
        1 * tilesStride + 4,
        2 * tilesStride + 4,
        1 * tilesStride + 4,
        2 * tilesStride + 4,
        1 * tilesStride + 4,
        2 * tilesStride + 4,
    };
    const int keyFramesTopLeft[6] =
    {
        1 * tilesStride + 5,
        2 * tilesStride + 5,
        1 * tilesStride + 5,
        2 * tilesStride + 5,
        1 * tilesStride + 5,
        2 * tilesStride + 5,
    };
    const int keyFramesBottomRight[6] =
    {
        1 * tilesStride + 6,
        2 * tilesStride + 6,
        1 * tilesStride + 6,
        2 * tilesStride + 6,
        1 * tilesStride + 6,
        2 * tilesStride + 6,
    };
    const int keyFramesBottomLeft[6] =
    {
        1 * tilesStride + 7,
        2 * tilesStride + 7,
        1 * tilesStride + 7,
        2 * tilesStride + 7,
        1 * tilesStride + 7,
        2 * tilesStride + 7,
    };

    const int keyFramesDead[4] =
    {
        0 * tilesStride + 4,
        0 * tilesStride + 5,
        0 * tilesStride + 6,
        0 * tilesStride + 7,
    };
};
