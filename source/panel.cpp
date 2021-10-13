#include "panel.hpp"

#include <nds.h>
#include "panel-bg.h"
#include <stdio.h>


Panel::Panel()
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
    setButtonState(Btn_Cursor,BtnState_Disabled);
    setButtonState(Btn_Fish,BtnState_Selected);
    setButtonState(Btn_Flag,BtnState_Normal);
}

void Panel::setButtonState(PanelButton button,PanelButtonState state)
{
    u16 bg,fg;
    switch(state)
    {
    case BtnState_Normal:
        bg=RGB15(31,31,31), fg=RGB15(0,0,0);
        break;
    case BtnState_Selected:
        bg=RGB15(0,0,0), fg=RGB15(31,31,31);
        break;
    case BtnState_Disabled:
        bg=RGB15(23,23,23), fg=RGB15(15,15,15);
        break;
    }

    BG_PALETTE_SUB[Panel::buttonBgIndex[button]]=bg;
    BG_PALETTE_SUB[Panel::buttonFgIndex[button]]=fg;
}
