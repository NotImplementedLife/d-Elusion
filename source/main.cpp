#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "scene.hpp"
#include "cat.hpp"
#include "car.hpp"
#include "panel.hpp"
#include "title-bg.h"

void playIntro(Cat &cat);

int main(void)
{
    Scene scene;
    scene.init();
    //scene.run();
    scene.game_over();

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
