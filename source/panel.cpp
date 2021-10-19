#include "panel.hpp"

#include <nds.h>
#include "panel-bg.h"

Panel::Panel() {}

void Panel::init()
{
    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    int id=bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

    decompress(panel_bgBitmap,(void*)0x06200000,LZ77Vram);

    bgSetPriority(id,0);

    dmaCopy(panel_bgPal,BG_PALETTE_SUB, panel_bgPalLen);

    buttonBgIndex[Btn_Cursor]  = getColorIndex(0x3DFF);
    buttonBgIndex[Btn_Fish]    = getColorIndex(0x3EDF);
    buttonBgIndex[Btn_Flag]    = getColorIndex(0x3FBF);
    buttonBgIndex[Btn_Restart] = getColorIndex(0x03E8);
    buttonBgIndex[Btn_Next]    = getColorIndex(0x3C40);
    buttonBgIndex[Btn_Back]    = getColorIndex(0x19FF);


    buttonFgIndex[Btn_Cursor]  = getColorIndex(0x037F);
    buttonFgIndex[Btn_Fish]    = getColorIndex(0x7FE0);
    buttonFgIndex[Btn_Flag]    = getColorIndex(0x001F);
    buttonFgIndex[Btn_Restart] = getColorIndex(0x6C1F);
    buttonFgIndex[Btn_Next]    = getColorIndex(0x7C80);
    buttonFgIndex[Btn_Back]    = getColorIndex(0x295F);

    setButtonState(Btn_Cursor,BtnState_Normal);
    setButtonState(Btn_Fish,BtnState_Normal);
    setButtonState(Btn_Flag,BtnState_Normal);
    setButtonState(Btn_Restart,BtnState_Hidden);
    setButtonState(Btn_Next,BtnState_Hidden);
    setButtonState(Btn_Back,BtnState_Hidden);
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
    buttonStates[button]=state;
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

PanelButton Panel::getTouchedButton(int x, int y)
{
    for(int b=0;b<buttonsCount;b++)
    {
        if(buttonStates[b]!=BtnState_Normal)
            continue;
        int o=4*b;
        int x1 = buttonHitBoxes[o];
        int y1 = buttonHitBoxes[o + 1];
        int x2 = buttonHitBoxes[o + 2];
        int y2 = buttonHitBoxes[o + 3];
        if(x1<=x && x<=x2 && y1<=y && y<=y2)
        {
            for(int i=0;i<4;i++)
                if(buttonStates[i]==BtnState_Selected)
                    setButtonState((PanelButton)i,BtnState_Normal);
            setButtonState((PanelButton)b,BtnState_Selected);
            return (PanelButton)b;
        }
    }
    return Btn_None;
}
