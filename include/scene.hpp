#pragma once

#include "cat.hpp"
#include "car.hpp"
#include "cursor.hpp"
#include "panel.hpp"
#include "actor.hpp"

class Actor;

class Scene
{
private:
    Cat cat;
    Car car;
    Cursor cursor;
    Panel panel;

    bool isIntro;

    Actor* flags[32];
    Actor* motherCat;
    u32 blocks[32];
    int startX=16, startY=16;
public:
    Scene();
    void init(bool intro=false);

    void run();

    void runIntro();

    void game_over(bool waitInput=true);

    void cat_moved(int dx,int dy);

    int rawX=16<<13, rawY=16<<13;
    int pxX=(rawX>>8), pxY=(rawY>>8);
    int blockX=16, blockY=16;

    void screenToBlock(int &x,int &y);
    void blockToScreen(int &x,int &y,int padding=0);

    Actor* flagOnBlock(int bX,int bY);
    Actor* flagFree(int &unused);

    void setBlocks(void* src);
    bool isBlockFree();

    void execute();
    void resetLevel();
    bool solved=false;
    bool failed=false;
};
