#include <iostream>
#include "fonts.h"
using namespace std;

extern void showAnahiName(int x, int y)
{
    Rect r;
    r.bot = y - 180;
    r.left = x + 280;
    r.center = 0;
    ggprint8b(&r, 25, 0x00ffff44, "Anahi V");

}


