#include "panel.hpp"

#include <nds.h>
#include "panel-bg.h"
#include <stdio.h>

Panel::Panel() {}

void Panel::init()
{
    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    int id=bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

    dmaCopy(panel_bgBitmap,(void*)0x06200000,panel_bgBitmapLen);

    consoleInit(&cs0, 0, BgType_Text4bpp, BgSize_T_256x256, 26, 3, false, true);

    bgSetPriority(id,0);
    bgSetPriority(cs0.bgId,1);

    iprintf("\x1B[30m");
    iprintf("000");

    dmaCopy(panel_bgPal,BG_PALETTE_SUB, panel_bgPalLen);

    buttonBgIndex[Btn_Cursor] = getColorIndex(0x3DFF);
    buttonBgIndex[Btn_Fish]   = getColorIndex(0x3EDF);
    buttonBgIndex[Btn_Flag]   = getColorIndex(0x3FBF);
    buttonBgIndex[Btn_Restart]   = getColorIndex(0x03E8);

    buttonFgIndex[Btn_Cursor] = getColorIndex(0x037F);
    buttonFgIndex[Btn_Fish]   = getColorIndex(0x7FE0);
    buttonFgIndex[Btn_Flag]   = getColorIndex(0x001F);
    buttonFgIndex[Btn_Restart]   = getColorIndex(0x6C1F);

    setButtonState(Btn_Cursor,BtnState_Disabled);
    setButtonState(Btn_Fish,BtnState_Disabled);
    setButtonState(Btn_Flag,BtnState_Disabled);
    setButtonState(Btn_Restart,BtnState_Hidden);
}

void Panel::setButtonState(PanelButton button,PanelButtonState state)
{
    u16 bg,fg;
    switch(state)
    {
    case BtnState_Normal:
        bg= RGB15(31,31,31), fg=(button==Btn_Flag) ? RGB15(28,28,28) : RGB15(0,0,0);
        break;
    case BtnState_Selected:
        bg=(button==Btn_Flag) ? RGB15(31,31,31):RGB15(0,0,0), fg=(button==Btn_Flag) ? RGB15(31,0,0) :RGB15(31,31,31);
        break;
    case BtnState_Disabled:
        bg=RGB15(23,23,23), fg=RGB15(15,15,15);
        break;
    case BtnState_Hidden:
        bg=fg=RGB15(31,31,31);
    }

    BG_PALETTE_SUB[Panel::buttonBgIndex[button]]=bg;
    BG_PALETTE_SUB[Panel::buttonFgIndex[button]]=fg;
}


int Panel::getColorIndex(u16 color)
{
    for(int i=0;i<256;i++)
    {
        if(BG_PALETTE_SUB[i]==color)
            return i;
    }
    return 0;
}
