#pragma once

#include "definitions.h"

struct Object
{
    u8 y;
    u8 x;
    u8 tile_index;
    union {
        u8 flags;
        struct
        {
            u8 cgb_palette : 3;
            u8 bank : 1;
            u8 dmg_palette : 1;
            u8 x_flip : 1;
            u8 y_flip : 1;
            u8 priority : 1;
        };
    } flags;
};

struct IndexedObject
{
    Object obj;
    u8 index;
};

struct SpriteFrame
{
};