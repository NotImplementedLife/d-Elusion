#pragma once

#include <nds.h>

enum PanelButton
{
    Btn_Cursor  = 0,
    Btn_Fish    = 1,
    Btn_Flag    = 2,
    Btn_Restart = 3
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

private:
    int buttonBgIndex[5];
    int buttonFgIndex[5];

    PrintConsole cs0;
    static int getColorIndex(u16 color);
};
