#include "sprite.hpp"

Sprite::Sprite(){}

void Sprite::create(OamState *oamState, SpriteSize _size,const uint* tiles,int _framesCount)
{
    oam=oamState;
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
        frames[i]=oamAllocateGfx(oam, size, SpriteColorFormat_256Color);
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
    oamSet(oam,               //main graphics engine context
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
