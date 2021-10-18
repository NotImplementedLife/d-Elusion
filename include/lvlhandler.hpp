#pragma once

#include "levels_bin.h"

#define LEVEL_SIZE (4 + 32 * 4)

#define LEVEL_ADDRESS(i) (((u8*)levels_bin) + ( i*LEVEL_SIZE ))

inline bool lvlComplete[12] =
{
    1, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};
