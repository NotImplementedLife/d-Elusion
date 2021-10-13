#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cat.hpp"
#include "car.hpp"
#include "panel.hpp"
#include "title-bg.h"

void playIntro(Cat &cat);

int main(void)
{
	int i = 0;
	touchPosition touch;

	videoSetMode(MODE_0_2D);

	setBackdropColor(ARGB16(1,31,31,31));
	setBackdropColorSub(ARGB16(1,31,31,31));

	initSprites();
	Cat cat;
	Car car;



    //playIntro(cat);


	Panel panel;
	car.setX(-128);

	int x=-128;
	while(1)
    {
		scanKeys();
		int held = keysHeld();
		int dx=128,dy=96;
		if(held & KEY_TOUCH)
        {
			touchRead(&touch);
			dx=touch.px;
			dy=touch.py;
        }
        cat.updateMove(dx,dy);
        x+=4;
        car.setX(x);
        car.setOam();
		cat.setOam();

		swiWaitForVBlank();


		oamUpdate(&oamMain);
	}

	return 0;
}

void playIntro(Cat &cat)
{
    lcdMainOnBottom();

    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    dmaCopy(title_bgBitmap,(void*)0x06200000,title_bgBitmapLen);

    dmaFillHalfWords(0xFFFF,BG_PALETTE_SUB,256);

    for(int j=200;j--;)
    {
        cat.updateMove(128,0);
        cat.setOam();
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    dmaCopy(title_bgPal,BG_PALETTE_SUB, title_bgPalLen);

    while(1)
    {
        swiWaitForVBlank();
    }
}
