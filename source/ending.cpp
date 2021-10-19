#include "ending.hpp"

#include <nds.h>
#include "ending-letter.h"

void setLine(int address,int line)
{
    dmaFillWords(0x00000000,(void*)address,256*192);
    if(line<=-192)
    {
        return;
    }
    if(line<0)
    {
        line=-line;
        line<<=8;
        dmaCopyAsynch(ending_letterBitmap,(void*)(address+line),192*256);
        return;
    }
    if(line<885)
    {
        line<<=6;
        dmaCopyAsynch(ending_letterBitmap+line,(void*)address,192*256);
        return;
    }
    int lim=line-884;
    if(lim<192)
    {
        line<<=6;
        dmaCopyAsynch(ending_letterBitmap+line,(void*)address,(192-lim)*256);
        return;
    }
}

void runEnding()
{
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    dmaFillHalfWords(0x0000,(void*)0x06000000,0x1F800);
    dmaFillHalfWords(0x0000,(void*)0x06200000,0x1F800);

    videoSetMode(MODE_5_2D);
    bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    videoSetModeSub(MODE_5_2D);
    bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

    dmaCopy(ending_letterPal,BG_PALETTE,ending_letterPalLen);
    dmaCopy(ending_letterPal,BG_PALETTE_SUB,ending_letterPalLen);

    swiWaitForVBlank();
    for(int i=-384;i<1100;i++)
    {
        setLine(0x06000000,i);
        setLine(0x06200000,i+192+96);
        scanKeys();
        for(int j=(keysHeld()>0)*5;j<6;j++)
            swiWaitForVBlank();
    }

    bgClearControlBits(0,0xFFFF);
    bgClearControlBits(1,0xFFFF);
    bgClearControlBits(3,0xFFFF);

    bgClearControlBits(4,0xFFFF);
    bgClearControlBits(5,0xFFFF);
    bgClearControlBits(7,0xFFFF);
}
