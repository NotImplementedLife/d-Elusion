#include "scene.hpp"

#ifdef HAS_SOUND
#include "honk_bin.h"
#endif

#include "tiles.h"
#include "title-bg.h"
#include "icons-sprite.h"
#include "cat-sprite.h"
#include "car-sprite.h"

#include <nds.h>
#include <math.h>

Scene::Scene() { }

void Scene::init(bool intro)
{
    oamClear(&oamMain,0,128);
    oamClear(&oamSub,0,128);
    isIntro=intro;
	videoSetMode(MODE_5_2D);
	vramSetBankB(VRAM_B_MAIN_BG_0x06000000);

	setBackdropColor(ARGB16(1,31,31,31));
	setBackdropColorSub(ARGB16(1,31,31,31));

    bgInit(2, BgType_ExRotation, BgSize_ER_256x256, 1, 0);
    bgSetPriority(2,3);
    bgWrapOn(2);	
    dmaCopy(tilesPal,BG_PALETTE,tilesPalLen);
    dmaCopy(tilesTiles, (void*)0x06000000, tilesTilesLen);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankI(VRAM_I_LCD);
	vramSetBankD(VRAM_D_LCD);
	vramSetBankF(VRAM_F_LCD);
	dmaCopy(cat_spritePal, &VRAM_F_EXT_SPR_PALETTE[0][0],cat_spritePalLen);
	dmaCopy(car_spritePal, &VRAM_F_EXT_SPR_PALETTE[1][0],car_spritePalLen);
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
	oamInit(&oamMain, SpriteMapping_1D_128, true);

	car.init();
    cat.init();
    cursor.init();
    cursor.setArrow();

    car.setX(-128);
    car.setY(64);

    u32* map=(u32*)0x06000800;
    if(!intro)
    {
        for(int i=0;i<flagsCount;i++)
        {
            flags[i]=new Actor();
            flags[i]->init(this);
            flags[i]->oamIndex=16+i;
            flags[i]->sprite.create(&oamMain, SpriteSize_16x16, icons_spriteTiles, 4);
            flags[i]->sprite.priority = 3;
            flags[i]->sprite.setFrameIndex(3);
            flags[i]->blockX=-10;
            flags[i]->blockY=-10;
        }

        motherCat=new Actor();
        motherCat->init(this);
        motherCat->oamIndex=7;
        motherCat->sprite.create(&oamMain, SpriteSize_32x64,cat_spriteTiles,30);
        motherCat->sprite.priority=3;
        motherCat->sprite.setFrameIndex(4);
        motherCat->blockX=-10;
        motherCat->blockY=-10;
        motherCat->padding=0;		


        lcdMainOnTop();
        for(int i=8;i--;)
        {
            for(int r=8;r--; *(map++) = 0x00010000, *(map++) = 0x00030002);
            for(int r=8;r--; *(map++) = 0x00050004, *(map++) = 0x00070006);
            for(int r=8;r--; *(map++) = 0x00090008, *(map++) = 0x000B000A);
            for(int r=8;r--; *(map++) = 0x000D000C, *(map++) = 0x000F000E);
        }

        blockX=blockY=16;
        rawX=rawY=16<<13;
        pxX=(rawX>>8)+16;
        pxY=(rawY>>8)+16;

        panel.init();
        panel.setButtonState(Btn_Cursor,BtnState_Selected);

        bgSetScroll(2,16,16);
        bgUpdate();
    }
    else
    {
        lcdMainOnBottom();
        dmaFillWords((u32)0x00050005,map,2048);
        cursor.hidden=true;
        rawY=3<<13;

        videoSetModeSub(MODE_5_2D);
        vramSetBankC(VRAM_C_SUB_BG_0x06200000);
        bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
        decompress(title_bgBitmap,(void*)0x06200000,LZ77Vram);

        dmaFillHalfWords(0xFFFF,BG_PALETTE_SUB,256);
    }
}

void Scene::run()
{
    if(isIntro)
    {
        runIntro();
        return;
    }
    cat.reset();
    touchPosition touch;
    int msg = 0;
    while(!msg)
    {
        scanKeys();
		int held = keysHeld();
		int down = keysDown();

		if(down & KEY_TOUCH)
        {
            touchRead(&touch);
            PanelButton b = panel.getTouchedButton(touch.px,touch.py);
            if(b==Btn_Cursor)
            {
                cursor.setArrow();
            }
            else if(b==Btn_Fish)
            {
                cursor.setFish();
            }
            else if(b==Btn_Flag)
            {
                cursor.setFlag();
            }
        }
        if(down & KEY_A)
        {
            if(cursor.isArrow())
            {
                // get clicked block coordinates
                int cX=cursor.x+2;
                int cY=cursor.y;
                screenToBlock(cX,cY);

                Actor* flag = flagOnBlock(cX,cY);
                if(flag!=NULL)
                {
                    flag->blockX=flag->blockY=-10;
                    panel.setButtonState(Btn_Flag,BtnState_Normal);
                }
            }
            else if(cursor.isFlag())
            {
                int cX=cursor.x+8;
                int cY=cursor.y+8;
                screenToBlock(cX,cY);

                Actor* flag = flagOnBlock(cX,cY);
                if(flag==NULL)
                {
                    int count=0;
                    flag=flagFree(count);

                    if(flag!=NULL)
                    {
                        flag->blockX=cX;
                        flag->blockY=cY;
                    }

                    if(count==1)
                    {
                        cursor.setArrow();
                        panel.setButtonState(Btn_Flag,BtnState_Disabled);
                        panel.setButtonState(Btn_Cursor,BtnState_Selected);
                    }
                }
            }
        }

        if(held & KEY_UP)
        {
            if(cursor.y>5)
                cursor.y--;
        }
        else if(held & KEY_DOWN)
        {
            if(cursor.y<187)
            cursor.y++;
        }

        if(held & KEY_LEFT)
        {
            if(cursor.x>5)
                cursor.x--;
        }
        else if(held & KEY_RIGHT)
        {
            if(cursor.x<250)
                cursor.x++;
        }

		if(cursor.isFish())
        {
			int dx=cursor.x;
			int dy=cursor.y;
			cat.updateMove(dx, dy);
			dx-=128, dy-=96;

            float len=sqrt(dx*dx+dy*dy);
            dx=floatToFixed(0.5*dx/len,8);
            dy=floatToFixed(0.5*dy/len,8);

            rawX+=dx;
            rawY+=dy;
            pxX=(rawX>>8)+16;
            pxY=(rawY>>8)+16;
            blockX=(cat.x+16+pxX-128)>>5;
            blockY=(cat.y+16+pxY- 96)>>5;

            if(blockX<0 || blockX>31 || blockY<0 || blockY>31)
            {
                msg=1;
            }
            else if(!isBlockFree())
            {
                msg=1;
            }
            else if(blockX==motherCat->blockX && (blockY==motherCat->blockY || blockY==motherCat->blockY+1))
            {
                msg=2; // level complete
            }


			bgScrollf(2,dx,dy);
            bgUpdate();
        }
        else
        {
            cat.updateMove(128,96);
        }
		cat.setOam();
        cursor.setOam(10);

        for(int i=0;i<flagsCount;i++)
        {
            flags[i]->update();
        }
        motherCat->update();
		swiWaitForVBlank();

		oamUpdate(&oamMain);
    }

    if(msg==1)
    {
        game_over();
    }
    else if(msg==2)
    {
        solved=true;
        panel.setButtonState(Btn_Cursor,BtnState_Disabled);
        panel.setButtonState(Btn_Fish,BtnState_Disabled);
        panel.setButtonState(Btn_Flag,BtnState_Disabled);
        panel.setButtonState(Btn_Next,BtnState_Normal);
        while(1)
        {
            scanKeys();
            int down = keysDown();


            if(down & KEY_START)
            {
                break;
            }
            else if(down & KEY_TOUCH)
            {
                touchRead(&touch);
                PanelButton b = panel.getTouchedButton(touch.px,touch.py);
                if(b==Btn_Next)
                {
                    break;
                }
            }
            swiWaitForVBlank();
        }
    }
}

void Scene::runIntro()
{
    int msg = 0;
    while(!msg)
    {
        int dx=128;
		int dy=0;
		cat.updateMove(dx, dy);
		dx-=128, dy-=96;

        float len=sqrt(dx*dx+dy*dy);
        dx=floatToFixed(0.5*dx/len,8);
        dy=floatToFixed(0.5*dy/len,8);

        rawX+=dx;
        rawY+=dy;
        blockX=rawX>>13;
        blockY=rawY>>13;
        pxX=(rawX>>8)+16;
        pxY=(rawY>>8)+16;

        if(blockX<0 || blockX>31 || blockY<0 || blockY>31)
        {
            msg=1;
        }

        bgScrollf(2,dx,dy);
        bgUpdate();
		cat.setOam();
        cursor.setOam(10);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
    }

    if(msg==1)
    {
        game_over(false);
        swiWaitForVBlank();
    }
}

void Scene::game_over(bool waitInput)
{
    #ifdef HAS_SOUND
    if(!isIntro)
    {
        soundPlaySample(honk_bin,SoundFormat_ADPCM,honk_bin_size,65000,20,64,false,0);
        for(int i=20;i--;swiWaitForVBlank());
    }
    #endif
    u16* pal=new u16[256];
    dmaCopy(title_bgPal,pal,title_bgPalLen);
    touchPosition touch;
    int msg=0;

    cursor.hidden=true;
    cursor.setOam(10);
    car.setX(-128);

    int q=0;
    int frames=0;

    while(!msg)
    {
        frames++;
        scanKeys();
        int down = keysDown();

        if(frames>200 && waitInput)
        {
            if(down & KEY_START)
            {
                msg=1;
            }
            else if(down & KEY_TOUCH)
            {
                touchRead(&touch);
                PanelButton b = panel.getTouchedButton(touch.px,touch.py);
                if(b==Btn_Restart)
                {
                    msg=2;
                }
                else if(b==Btn_Back)
                {
                    failed=true;
                    msg=3;
                }
            }
        }
        if(frames==200 && waitInput)
        {
            panel.setButtonState(Btn_Cursor,BtnState_Disabled);
            panel.setButtonState(Btn_Fish,BtnState_Disabled);
            panel.setButtonState(Btn_Flag,BtnState_Disabled);
            panel.setButtonState(Btn_Restart,BtnState_Normal);
            panel.setButtonState(Btn_Back,BtnState_Normal);
        }

        if(frames>200 && (frames & 1) && !waitInput)
        {
            if(q<32)
            {
                for(int i=1;i<256;i++) 
                {
                    int color=pal[i];
                    int r = color & 0x1F;
                    int g = (color>>5) & 0x1F;
                    int b = (color>>10) & 0x1F;

                    r=((31-q)*31+q*r)/31;
                    g=((31-q)*31+q*g)/31;
                    b=((31-q)*31+q*b)/31;

                    BG_PALETTE_SUB[i]=RGB15(r,g,b);
                }
            }

            q++;
            if(q==50)
            {
                msg=1;
            }
        }

        car.advance();
        cat.play_dead();

        car.setOam();
		cat.setOam();

		swiWaitForVBlank();
		oamUpdate(&oamMain);
    }

    if(msg==2) // Restart button pressed
    {
        resetLevel();
        delete pal;
        return;
    }

    if(msg==3) // Back button pressed
    {
        failed=true;
        delete pal;
        solved=true;
        return;
    }

    msg=0;
    while(!msg)
    {
        scanKeys();

        if(keysDown())
        {
            msg=1;
        }
        swiWaitForVBlank();
    }
    delete pal;
    solved=true;
}

void Scene::screenToBlock(int &x,int &y)
{
    x+=pxX -128; x>>=5;
    y+=pxY - 96; y>>=5;
}

void Scene::blockToScreen(int &x,int &y,int padding)
{
    x<<=5; x+=128-pxX+padding;
    y<<=5; y+= 96-pxY+padding;
}

Actor* Scene::flagOnBlock(int bX,int bY)
{
    for(int i=0;i<flagsCount;i++)
    {
        if(flags[i]->blockX==bX && flags[i]->blockY==bY)
            return flags[i];
    }
    return NULL;
}

Actor* Scene::flagFree(int &unused)
{
    Actor* flag=NULL;
    unused=0;
    for(int i=0;i<flagsCount;i++)
    {
        if(flags[i]->blockX==-10 && flags[i]->blockY==-10)
        {
            unused++;
            flag=flags[i];
        }
    }
    return flag;
}

void Scene::setBlocks(u8* src)
{
    //iprintf("\x1B[30m");
    startX = blockX = *(src++);
    startY = blockY = *(src++);
    rawX=blockX<<13;
    rawY=blockY<<13;
    pxX=(rawX>>8)+16;
    pxY=(rawY>>8)+16;
    motherCat->blockX=(*((u8*)src++));
    motherCat->blockY=(*((u8*)src++));
    dmaCopy(src,blocks,128);
}

bool Scene::isBlockFree()
{
    return (((u32)blocks[blockY]) & ((u32)(1<<blockX)))==0;
}

void Scene::execute()
{
    while(!solved)
    {
        run();
    }
}

void Scene::resetLevel()
{
    blockX=startX;
    blockY=startY;
    rawX=blockX<<13;
    rawY=blockY<<13;
    pxX=(rawX>>8)+16;
    pxY=(rawY>>8)+16;
    panel.init();
    cursor.setArrow();
    cursor.hidden=false;
    panel.setButtonState(Btn_Cursor,BtnState_Selected);
    bgSetScroll(2,16,16);
    bgUpdate();

    for(int i=0;i<32;i++)
    {
        flags[i]->update();
    }
    motherCat->update();
	swiWaitForVBlank();

    oamUpdate(&oamMain);
}
