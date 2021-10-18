#include "scene.hpp"
#include "lvlhandler.hpp"
#include "lvlselector.hpp"

#ifdef HAS_SOUND
#include "bgm_bin.h"
#endif

int main(void)
{
    powerOn(POWER_ALL_2D);

    #ifdef HAS_SOUND
    soundEnable();
    soundPlaySample(bgm_bin,SoundFormat_ADPCM,bgm_bin_size,28000,100,64,true,0);
    #endif
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
