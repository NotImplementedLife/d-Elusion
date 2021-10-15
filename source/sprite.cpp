#include "sprite.hpp"

#include "car-sprite.h"
#include "cat-sprite.h"

void initSprites()
{
    vramSetBankA(VRAM_A_MAIN_SPRITE);

	vramSetBankF(VRAM_F_LCD);

	/*VRAM_F_EXT_SPR_PALETTE[0][0] = 0x463F;
	VRAM_F_EXT_SPR_PALETTE[0][1] = 0x0000;
	VRAM_F_EXT_SPR_PALETTE[0][2] = 0x001F;
	VRAM_F_EXT_SPR_PALETTE[0][3] = 0x4BE0;
	VRAM_F_EXT_SPR_PALETTE[0][4] = 0x03E9;
	VRAM_F_EXT_SPR_PALETTE[0][5] = 0x13E0;*/

	dmaCopy(cat_spritePal, &VRAM_F_EXT_SPR_PALETTE[0][0],cat_spritePalLen);
	dmaCopy(car_spritePal, &VRAM_F_EXT_SPR_PALETTE[1][0],car_spritePalLen);

	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);


	oamInit(&oamMain, SpriteMapping_1D_128, true);
}

Sprite::Sprite(){}

void Sprite::create(SpriteSize _size,const uint* tiles,int _framesCount)
{
    size=_size;
    framesCount=_framesCount;

    switch(size)
    {
        case SpriteSize_8x8  : nbytes = 8*8;   break;
        case SpriteSize_16x16: nbytes = 16*16; break;
        case SpriteSize_32x32: nbytes = 32*32; break;
        case SpriteSize_64x64: nbytes = 64*64; break;
        case SpriteSize_16x8 : nbytes = 16*8;  break;
        case SpriteSize_32x8 : nbytes = 32*8;  break;
        case SpriteSize_32x16: nbytes = 32*16; break;
        case SpriteSize_64x32: nbytes = 64*32; break;
        case SpriteSize_8x16 : nbytes = 8*16;  break;
        case SpriteSize_8x32 : nbytes = 8*32;  break;
        case SpriteSize_16x32: nbytes = 16*32; break;
        case SpriteSize_32x64: nbytes = 32*64; break;
    }

    frames=new u16*[framesCount];
    for(int i=0;i<framesCount;i++)
    {
        frames[i]=oamAllocateGfx(&oamMain, size, SpriteColorFormat_256Color);
        dmaCopy(tiles,frames[i],nbytes);
        tiles+=nbytes/4;
    }
    setFrameIndex(0);
}

void Sprite::setFrameIndex(int index)
{
    frameIndex=index;
    crtGfx = frames[index];
}

void Sprite::setOam(int oamIndex)
{
    oamSet(&oamMain,               //main graphics engine context
			oamIndex,                     //oam index (0 to 127)
			x, y,
			priority,              //priority, lower renders last (on top)
			0,					   //this is the palette index if multiple palettes or the alpha value if bmp sprite
			size,
			SpriteColorFormat_256Color,
			crtGfx,                //pointer to the loaded graphics
			-1,                    //sprite rotation data
			false,                 //double the size when rotating?
			hidden,			       //hide the sprite?
			false, false,          //vflip, hflip
			true	               //apply mosaic
			);
}
