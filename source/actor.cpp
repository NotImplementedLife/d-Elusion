#include "actor.hpp"

void Actor::init(Scene* parent_scene)
{
    scene=parent_scene;
}

void Actor::update()
{
    int x=blockX, y=blockY;
    // compute sprite position
    scene->blockToScreen(x,y,padding);
    if(x>-64 && y>-64 && x<255 && y<192)
    {
        sprite.x=x;
        sprite.y=y;
    }
    else
    {
        sprite.x=256;
        sprite.y=192;
    }
    sprite.setOam(oamIndex);
}
