#include "fonts.h"
#include <iostream>

extern void showFranciscoName(int x, int y)
{
    Rect r;
    r.bot = y - 190;
    r.left = x + 280;
    r.center = 0;
    ggprint8b(&r, 40, 0x00ffff44, "Francisco U");
}
