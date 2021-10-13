#include "car.hpp"

#include "car-sprite.h"

#include <stdio.h>

Car::Car() { }

void Car::init()
{

    front.create(SpriteSize_64x64, car_spriteTiles,2);
    front.x=0;
    front.y=0;
    front.priority=2;
    front.setFrameIndex(0);
    back.create(SpriteSize_64x64, car_spriteTiles,2);
    back.x=64;
    back.y=0;
    back.priority=2;
    back.setFrameIndex(1);

    wheel1.create(SpriteSize_16x16,car_spriteTiles,33);
    wheel1.x=23;
    wheel1.y=38;
    wheel1.priority=1;
    wheel1.setFrameIndex(32);

    wheel2.create(SpriteSize_16x16,car_spriteTiles,33);
    wheel2.x=93;
    wheel2.y=38;
    wheel2.priority=1;
    wheel2.setFrameIndex(32);
}

void Car::setX(int x)
{
    front.x=x;
    back.x=x+64;
    wheel1.x=x+23;
    wheel2.x=x+93;
}

void Car::setY(int y)
{
    front.y=y;
    back.y=y;
    wheel1.y=wheel2.y=y+38;
}

void Car::advance()
{
    if(front.x<256)
        setX(front.x+10);
}

void Car::setOam()
{
    wheelsAngle-=degreesToAngle(5);
    oamSet(&oamMain,               //main graphics engine context
			1,                     //oam index (0 to 127)
			front.x, front.y,
			front.priority,              //priority, lower renders last (on top)
			1,					   //this is the palette index if multiple palettes or the alpha value if bmp sprite
			front.size,
			SpriteColorFormat_256Color,
			front.crtGfx,                //pointer to the loaded graphics
			-1,                    //sprite rotation data
			false,                 //double the size when rotating?
			front.hidden,			       //hide the sprite?
			false, false,          //vflip, hflip
			true	               //apply mosaic
			);

    oamSet(&oamMain,               //main graphics engine context
			2,                    //oam index (0 to 127)
			back.x, back.y,
			back.priority,              //priority, lower renders last (on top)
			1,					   //this is the palette index if multiple palettes or the alpha value if bmp sprite
			back.size,
			SpriteColorFormat_256Color,
			back.crtGfx,                //pointer to the loaded graphics
			-1,                    //sprite rotation data
			false,                 //double the size when rotating?
			back.hidden,			       //hide the sprite?
			false, false,          //vflip, hflip
			true	               //apply mosaic
			);

    oamRotateScale(&oamMain, 0, wheelsAngle, intToFixed(1, 8), intToFixed(1, 8));
    oamSet(&oamMain,               //main graphics engine context
			3,                    //oam index (0 to 127)
			wheel1.x, wheel1.y,
			wheel1.priority,              //priority, lower renders last (on top)
			1,					   //this is the palette index if multiple palettes or the alpha value if bmp sprite
			wheel1.size,
			SpriteColorFormat_256Color,
			wheel1.crtGfx,                //pointer to the loaded graphics
			0,                    //sprite rotation data
			false,                 //double the size when rotating?
			wheel1.hidden,			       //hide the sprite?
			false, false,          //vflip, hflip
			true	               //apply mosaic
			);
    oamSet(&oamMain,               //main graphics engine context
			4,                    //oam index (0 to 127)
			wheel2.x, wheel2.y,
			wheel2.priority,              //priority, lower renders last (on top)
			1,					   //this is the palette index if multiple palettes or the alpha value if bmp sprite
			wheel2.size,
			SpriteColorFormat_256Color,
			wheel2.crtGfx,                //pointer to the loaded graphics
			0,                    //sprite rotation data
			false,                 //double the size when rotating?
			wheel2.hidden,			       //hide the sprite?
			false, false,          //vflip, hflip
			true	               //apply mosaic
			);
}
