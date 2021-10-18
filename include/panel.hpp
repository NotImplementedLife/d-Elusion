#pragma once

#include <nds.h>

enum PanelButton
{
    Btn_None    = -1,
    Btn_Cursor  =  0,
    Btn_Fish    =  1,
    Btn_Flag    =  2,
    Btn_Restart =  3,
    Btn_Next    =  4,
    Btn_Back    =  5,
};

enum PanelButtonState
{
    BtnState_Normal,
    BtnState_Selected,
    BtnState_Disabled,
    BtnState_Hidden
};

class Panel
{
public:
    Panel();
    void init();
    void setButtonState(PanelButton button,PanelButtonState state);
    PanelButton getTouchedButton(int x, int y);

private:
    int buttonBgIndex[6];
    int buttonFgIndex[6];

    static int getColorIndex(u16 color);

    static const int buttonsCount = 6;
    const int buttonHitBoxes[4 * buttonsCount] =
    {
        201,  70, 255, 101,  // Cursor
        201, 108, 255, 139,  // Fish
        201, 146, 255, 177,  // Flag
         79,  86, 171, 118,  // Restart
        109, 157, 141, 175,  // Next
         15, 158,  31, 178   // Back
    };

    int buttonStates[6] = {0, 0, 0, 0, 0, 0};
};
