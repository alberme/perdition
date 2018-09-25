#include <iostream>
#include "fonts.h"
using namespace std;

extern void showAnahiName()
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = g.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "Anahi V");

}


