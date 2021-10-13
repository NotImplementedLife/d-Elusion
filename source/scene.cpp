#include "scene.hpp"

#include <nds.h>

Scene::Scene() { }

void Scene::init()
{
	videoSetMode(MODE_0_2D);

	setBackdropColor(ARGB16(1,31,31,31));
	setBackdropColorSub(ARGB16(1,31,31,31));

	initSprites();

	car.init();
    cat.init();

    car.setX(-128);
    car.setY(64);

    panel.init();
}

void Scene::run()
{
    touchPosition touch;
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
		cat.setOam();

		swiWaitForVBlank();


		oamUpdate(&oamMain);
    }
}

void Scene::game_over()
{
    label:
    int msg=0;
    car.setX(-128);
    cat.reset();
    while(!msg)
    {
        scanKeys();
        if(keysHeld() & KEY_START)
        {
            break;
        }

        car.advance();
        cat.play_dead();

        car.setOam();
		cat.setOam();
		swiWaitForVBlank();
		oamUpdate(&oamMain);
    }
    goto label;
}
