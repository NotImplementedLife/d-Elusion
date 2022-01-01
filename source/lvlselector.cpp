#include "lvlselector.hpp"

#include "save-warning.h"
#include "lvlicons-sprite.h"
#include "tutorial-bg.h"
#include "lvlhandler.hpp"

#include "savedata.hpp"

#include <nds.h>

void LvlSelector::init()
{
    oamClear(&oamMain,0,128);
    oamClear(&oamSub,0,128);
    bgClearControlBits(0,0xFFFF);
    bgClearControlBits(1,0xFFFF);
    bgClearControlBits(3,0xFFFF);

    bgClearControlBits(4,0xFFFF);
    bgClearControlBits(5,0xFFFF);
    bgClearControlBits(7,0xFFFF);

    setBackdropColor(ARGB16(1,31,31,31));
	setBackdropColorSub(ARGB16(1,31,31,31));

    lcdMainOnTop();
    videoSetMode(MODE_5_2D);	
	vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
	bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

	videoSetModeSub(MODE_5_2D);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

	dmaFillWords(0x0000,(void*)0x06200000,256*192);

	dmaCopy(save_warningPal,BG_PALETTE_SUB,save_warningPalLen);

	if(!save_available)
	{
		for(int i=0;i<32;i++)
			dmaCopy((u8*)save_warningBitmap+128*i,(void*)(0x06200000+256*(i+158)+2),128);
	}
	
	decompress(tutorial_bgBitmap,(void*)0x06000000,LZ77Vram);
	dmaCopy(tutorial_bgPal,BG_PALETTE,512);

    vramSetBankI(VRAM_I_LCD);
	dmaCopy(lvlicons_spritePal, &VRAM_I_EXT_SPR_PALETTE[0][0],lvlicons_spritePalLen);
	dmaCopy(lvlicons_spritePal, &VRAM_I_EXT_SPR_PALETTE[1][0],lvlicons_spritePalLen);
	dmaCopy(lvlicons_spritePal, SPRITE_PALETTE_SUB, lvlicons_spritePalLen);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	vramSetBankD(VRAM_D_SUB_SPRITE);
	oamInit(&oamSub, SpriteMapping_1D_128, true);


    for(int i=0;i<12;i++)
    {
        icons[i].create(&oamSub, SpriteSize_32x32,lvlicons_spriteTiles+512*i,2);
        icons[i].setFrameIndex(lvlComplete[i]==1 ? 0:1);
        icons[i].x=32+(i%4)*52 + 8*(i/4)*(2-i/4);
        icons[i].y=32+(i/4)*48 + 5*(i%4)*(i%4-2)-4;
    }
		
	//int x=save_warningBitmap[0];
}

#include <stdio.h>
int LvlSelector::execute()
{
    //consoleDemoInit();
    touchPosition touch;
    while(1)
    {
        scanKeys();
        if(keysDown() & KEY_TOUCH)
        {
            touchRead(&touch);
            int id=hitTest(touch.px,touch.py);
            if(id!=-1)
            {
                return id;
            }
        }

        for(int i=0;i<12;i++)
        {
            icons[i].setOam(i);
        }
        swiWaitForVBlank();
        oamUpdate(&oamSub);
    }
}

int LvlSelector::hitTest(int x,int y)
{
    int choice=-1;
    for(int i=0;i<12;i++)
    {
        int ix1=icons[i].x;
        int iy1=icons[i].y;
        int ix2=ix1+32;
        int iy2=iy1+32;
        if(lvlComplete[i] && ix1<=x && x<ix2 && iy1<=y && y<iy2)
        {
            choice=i;
            break;
        }
    }
    return choice;
}
