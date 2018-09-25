//Arielle Battle
//Arielle's game source code

#include <iostream>
#include "fonts.h"


void ShowArielleName(int x, int y)
{
    	Rect r;
	r.bot = y - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ffff44, "Arielle Battle");
}



