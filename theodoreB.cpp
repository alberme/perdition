//Theodore Barcelona
//Theo's game source code
#include <iostream>
#include "fonts.h"

extern void ShowTheodoreName(int x, int y)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "Theodore Barcelona");
}

