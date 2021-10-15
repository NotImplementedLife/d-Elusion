#include <nds.h>
#include "scene.hpp"
#include "lvlhandler.hpp"

int main(void)
{
    Scene* scene = new Scene();
    scene->init(true);
    scene->execute();
    delete scene;

    scene=new Scene();
    scene->init();
    scene->setBlocks(LEVEL_ADDRESS(0));
    scene->execute();
    delete scene;


	return 0;
}
