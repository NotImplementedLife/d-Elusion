#include "scene.hpp"
#include "lvlhandler.hpp"
#include "lvlselector.hpp"
#include "bgm_bin.h"

int main(void)
{
    powerOn(POWER_ALL_2D);
    soundEnable();
    soundPlaySample(bgm_bin,SoundFormat_ADPCM,bgm_bin_size,28000,100,64,true,0);
    Scene* scene = new Scene();
    scene->init(true);
    scene->execute();
    delete scene;

    while(1)
    {
        LvlSelector* lvlSelector = new LvlSelector();
        lvlSelector->init();
        int level=lvlSelector->execute();
        delete lvlSelector;

        scene=new Scene();
        scene->init();
        scene->setBlocks(LEVEL_ADDRESS(level));
        scene->execute();

        if(!scene->failed && level<11)
            lvlComplete[level+1]=1;

        delete scene;
    }


	return 0;
}
