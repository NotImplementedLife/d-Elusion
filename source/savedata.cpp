#include "savedata.hpp"

#include <fat.h>
#include <dirent.h>
#include <unistd.h>
#include "lvlhandler.hpp"
#include <stdio.h>

bool save_available = false;

bool save_inited = false;

__attribute__((section(".iwram"))) void saveInit()
{
	if(save_inited) return;
	if((save_available = fatInitDefault()))
	{
		DIR* dir = opendir("/data");
		if(!dir)
		{
			mkdir("/data", 0777);
		}
		else closedir(dir);
		
		dir = opendir("/data/d-Elusion");
		if(!dir)
		{
			mkdir("/data/d-Elusion", 0777);
		}
		else closedir(dir);
		
		if(access("/data/d-Elusion/d-Elusion.sav", F_OK) ==0)
		{
			FILE* fptr = fopen("/data/d-Elusion/d-Elusion.sav", "rb");
			u8 lvl;
			fread(&lvl,1,sizeof(u8),fptr);
			fclose(fptr);
			
			lvl = lvl>12 ? 12 : lvl;
			if(lvl==0) lvl=1;
			
			for(int i=0;i<lvl;i++) lvlComplete[i] = 1;
			for(int i=lvl;i<12;i++) lvlComplete[i] = 0;
		}
		else
		{
			FILE* fptr = fopen("/data/d-Elusion/d-Elusion.sav", "wb");
			u8 _1 = 1;
			fwrite(&_1, 1, sizeof(u8), fptr);
			fclose(fptr);
		}
		
		save_available = true;
	}
	save_inited=true;
}

void saveWrite()
{
	u8 lvl=11;
	for(;lvl>0 && lvlComplete[lvl]==0;lvl--);
	lvl++;
	FILE* fptr = fopen("/data/d-Elusion/d-Elusion.sav", "wb");	
	fwrite(&lvl, 1, sizeof(u8), fptr);
	fclose(fptr);
}