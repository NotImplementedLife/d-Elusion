#pragma once

#include <nds.h>

void initSprites();


class Sprite
{
private:
    u16 nbytes=0;
public:
    u16* crtGfx;
    u16** frames;
    int framesCount=0;
    int x=0,y=0;
    int priority=0;
    bool hidden = false;
    SpriteSize size;

    Sprite();
    Sprite(SpriteSize _size,const uint* tiles,int _framesCount);

    void setFrameIndex(int index);
    void setOam();
protected:
    int frameIndex=0;
};
