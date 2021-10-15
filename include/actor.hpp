#pragma once

#include "scene.hpp"

class Scene;

class Actor
{
public:
    void init(Scene *parent_scene);
    Sprite sprite;
    int blockX=0, blockY=0;
    int padding=8;
    void update();
    int oamIndex;
private:
    Scene* scene;
};
