#include "cursor.hpp"

#include "icons-sprite.h"

Cursor::Cursor() {}

void Cursor::init()
{
    create(&oamMain, SpriteSize_16x16, icons_spriteTiles, 4);
    x = 256/2-8;
    y = 192/2-24;
    priority = 1;
}

void Cursor::setArrow()
{
    setFrameIndex(Icon_Arrow);
}
void Cursor::setFish()
{
    setFrameIndex(Icon_Fish);
}

void Cursor::setFlag()
{
    setFrameIndex(Icon_Flag);
}

bool Cursor::isArrow() { return frameIndex==Icon_Arrow; }
bool Cursor::isFish() { return frameIndex==Icon_Fish; }
bool Cursor::isFlag() { return frameIndex==Icon_Flag; }
