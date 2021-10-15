#include "cat.hpp"

#include "cat-sprite.h"

#include <stdio.h>

int sgn(int x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }

Cat::Cat()
{

}

void Cat::init()
{
    create(SpriteSize_32x32, cat_spriteTiles, 3*tilesStride);
    x = 256/2-16;
    y = 192/2-16;
    priority = 3;
}


void Cat::updateMove(int toX,int toY)
{
    const int a = 765;
    const int b = 1847;
    toX-=128;
    toY-=96;

    if(toX==0 && toY==0)
    {
        if(crtKeyFrame==keyFramesDown || crtKeyFrame==keyFramesBottomLeft || crtKeyFrame==keyFramesBottomRight)
            setFrameIndex(0);
        else if(crtKeyFrame==keyFramesUp || crtKeyFrame==keyFramesTopLeft || crtKeyFrame==keyFramesTopRight)
            setFrameIndex(1);
        else if(crtKeyFrame==keyFramesLeft)
            setFrameIndex(2);
        else if(crtKeyFrame==keyFramesRight)
            setFrameIndex(3);
        return;
    }

    int ax = a*toX, ay = a*toY;
    int bx = b*toX, by = b*toY;

    int d1 = sgn(ax-by);
    int d2 = sgn(bx-ay);
    int d3 = sgn(bx+ay);
    int d4 = sgn(ax+by);

    //iprintf("\e[1;1H\e[2J%2d %2d %2d %2d",d1,d2,d3,d4);

    int* kfr;

    if(d1==1 && d2==1 && d3==1 && d4==1)
        kfr = (int*)keyFramesRight;
    else if(d1== 1 && d2== 1 && d3==-1 && d4==-1)
        kfr = (int*)keyFramesUp;
    else if(d1==-1 && d2==-1 && d3==-1 && d4==-1)
        kfr = (int*)keyFramesLeft;
    else if(d1==-1 && d2==-1 && d3== 1 && d4== 1)
        kfr = (int*)keyFramesDown;

    else if(d1== 1 && d2== 1 && d3== 1 && d4==-1)
        kfr = (int*)keyFramesTopRight;
    else if(d1== 1 && d2==-1 && d3==-1 && d4==-1)
        kfr = (int*)keyFramesTopLeft;
    else if(d1==-1 && d2==-1 && d3==-1 && d4== 1)
        kfr = (int*)keyFramesBottomLeft;
    else if(d1==-1 && d2== 1 && d3== 1 && d4== 1)
        kfr = (int*)keyFramesBottomRight;

    if(kfr!=crtKeyFrame)
    {
        crtKeyFrame=kfr;
        animTimer=0;
        animIndex=0;
    }
    else animTimer++;
    if(animTimer==10)
    {
        animTimer=0;
        animIndex++;
        if(animIndex>=6)
            animIndex=0;
        setFrameIndex(kfr[animIndex]);
    }
}

void Cat::play_dead()
{
    if(crtKeyFrame != keyFramesDead)
    {
        crtKeyFrame = (int*)keyFramesDead;
        animTimer = 0;
    }

    if(animTimer==24)
    {
        setFrameIndex(keyFramesDead[0]);
    }
    else if(24<animTimer && animTimer<30 && animTimer%5==0)
    {
        y-=1;
        x+=5;
    }
    else if(30<=animTimer && animTimer<50 && animTimer%5==0)
    {
        y+=1;
        x+=5;
    }
    else if(animTimer==50)
    {
        setFrameIndex(keyFramesDead[1]);
    }
    else if(animTimer==75)
    {
        setFrameIndex(keyFramesDead[2]);
    }
    else if(animTimer==99)
    {
        setFrameIndex(keyFramesDead[3]);
    }

    if(animTimer<100)
        animTimer++;
}

void Cat::reset()
{
    crtKeyFrame=NULL;
    setFrameIndex(0);
    x = 256/2-16;
    y = 192/2-16;
}


