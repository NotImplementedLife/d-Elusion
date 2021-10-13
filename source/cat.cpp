#include "cat.hpp"

#include "cat-sprite.h"

#include <stdio.h>

int sgn(int x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }

Cat::Cat() : Sprite(SpriteSize_32x32, cat_spriteTiles, 24)
{
    x = 256/2-16;
    y = 192/2-16;
}

void Cat::updateMove(int toX,int toY)
{
    const int a = 765;
    const int b = 1847;
    toX-=128;
    toY-=96;

    int ax = a*toX, ay = a*toY;
    int bx = b*toX, by = b*toY;

    int d1 = sgn(ax-by);
    int d2 = sgn(bx-ay);
    int d3 = sgn(bx+ay);
    int d4 = sgn(ax+by);

    iprintf("\e[1;1H\e[2J%2d %2d %2d %2d",d1,d2,d3,d4);

    /*int* kfr;

    if(d1*d2>=0)
        kfr = keyFramesRight;
    else if(d1>=0 && d2<=0)
        kfr = keyFramesTopRight;
    else if(d2>=0 && d3<0)
        kfr = keyFramesTop;
    else if()*/



    //mapX+=dx;
    //mapY+=dy;
}


