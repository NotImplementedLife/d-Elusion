#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cat.hpp"
#include "panel.hpp"

void playIntro(const Cat &cat);

int main(void)
{
	int i = 0;
	touchPosition touch;

	videoSetMode(MODE_0_2D);

	setBackdropColor(ARGB16(1,31,31,31));
	setBackdropColorSub(ARGB16(1,31,31,31));

	initSprites();
	Cat cat;

    playIntro(cat);


	Panel panel;

	while(1)
    {
		scanKeys();
		int held = keysHeld();
		if(held & KEY_TOUCH)
			touchRead(&touch);
        cat.updateMove(touch.px,touch.py);

		if(held & KEY_START) break;

		cat.setOam();

		swiWaitForVBlank();


		oamUpdate(&oamMain);
	}

	return 0;
}

void playIntro(const Cat &cat)
{
    lcdMainOnBottom();

}
