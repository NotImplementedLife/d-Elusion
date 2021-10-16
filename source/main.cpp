#include "scene.hpp"
#include "lvlhandler.hpp"
#include "lvlselector.hpp"

int main(void)
{
    Scene* scene = new Scene();
    scene->init(true);
    //scene->execute();
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
        delete scene;

        if(level<11)
            lvlComplete[level+1]=1;
    }


	return 0;
}
