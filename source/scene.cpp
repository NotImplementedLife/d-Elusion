#include "scene.hpp"

#include "tiles.h"

#include <nds.h>
#include <math.h>
#include <stdio.h>

Scene::Scene() { }

void Scene::init(bool intro)
{
	videoSetMode(MODE_5_2D);
	vramSetBankB(VRAM_B_MAIN_BG_0x06000000);

	setBackdropColor(ARGB16(1,31,31,31));
	setBackdropColorSub(ARGB16(1,31,31,31));

    bgInit(2, BgType_ExRotation, BgSize_ER_256x256, 1, 0);
    bgSetPriority(2,3);
    bgWrapOn(2);
    dmaCopy(tilesPal,BG_PALETTE,tilesPalLen);
    dmaCopy(tilesTiles, (void*)0x06000000, tilesTilesLen);

	initSprites();

	car.init();
    cat.init();
    cursor.init();
    cursor.setFish();

    car.setX(-128);
    car.setY(64);

    if(!intro)
    {
        lcdMainOnTop();

        u32* map=(u32*)0x06000800;
        for(int i=8;i--;)
        {
            for(int r=8;r--; *(map++) = 0x00010000, *(map++) = 0x00030002);
            for(int r=8;r--; *(map++) = 0x00050004, *(map++) = 0x00070006);
            for(int r=8;r--; *(map++) = 0x00090008, *(map++) = 0x000B000A);
            for(int r=8;r--; *(map++) = 0x000D000C, *(map++) = 0x000F000E);
        }

        panel.init();
    }
    else
    {
        lcdMainOnBottom();
    }
}

void Scene::run()
{
    touchPosition touch;
    while(1)
    {
        scanKeys();
		int held = keysHeld();
		int down = keysDown();

		if(down & KEY_TOUCH)
        {
            touchRead(&touch);
            iprintf("%d %d\n",touch.px,touch.py);
            PanelButton b = panel.getTouchedButton(touch.px,touch.py);
            if(b==Btn_Cursor)
            {
                cursor.setArrow();
            }
            else if(b==Btn_Fish)
            {
                cursor.setFish();
            }
            else if(b==Btn_Flag)
            {
                cursor.setFlag();
            }
        }

        if(held & KEY_UP)
        {
            if(cursor.y>5)
                cursor.y--;
        }
        else if(held & KEY_DOWN)
        {
            if(cursor.y<187)
            cursor.y++;
        }

        if(held & KEY_LEFT)
        {
            if(cursor.x>5)
                cursor.x--;
        }
        else if(held & KEY_RIGHT)
        {
            if(cursor.x<250)
                cursor.x++;
        }

		if(cursor.isFish())
        {
			int dx=cursor.x+8;
			int dy=cursor.y+8;
			cat.updateMove(dx, dy);
			dx-=128, dy-=96;

            float len=sqrt(dx*dx+dy*dy);
            dx=floatToFixed(1.61*dx/len,8);
            dy=floatToFixed(1.61*dy/len,8);

			 bgScrollf(2,dx,dy);
			 bgUpdate();
        }
        else
        {
            cat.updateMove(128,96);
        }
		cat.setOam();
        cursor.setOam(10);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
    }
}

void Scene::game_over()
{
    label:
    int msg=0;
    car.setX(-128);
    cat.reset();
    while(!msg)
    {
        scanKeys();

        if(keysHeld() & KEY_START)
        {
            break;
        }

        car.advance();
        cat.play_dead();

        car.setOam();
		cat.setOam();
		swiWaitForVBlank();
		oamUpdate(&oamMain);
    }
    goto label;
}
