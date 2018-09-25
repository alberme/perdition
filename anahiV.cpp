#include <iostream>
#include "fonts.h"
using namespace std;

extern void showAnahiName(int x, int y)
{
    Rect r;
    r.bot = y - 30;
    r.left = x + 300;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ffff44, "Anahi V");

}


