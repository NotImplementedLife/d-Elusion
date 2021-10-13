#pragma once

#include <nds.h>

enum PanelButton
{
    Btn_Cursor = 0,
    Btn_Fish = 1,
    Btn_Flag = 2
};

enum PanelButtonState
{
    BtnState_Normal,
    BtnState_Selected,
    BtnState_Disabled
};

class Panel
{
public:
    Panel();

    void setButtonState(PanelButton button,PanelButtonState state);

private:
    const int buttonBgIndex[3] = {  2, 4, 6 };
    const int buttonFgIndex[3] = {  3, 5, 7 };

    PrintConsole cs0;
};
