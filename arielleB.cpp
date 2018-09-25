//Arielle Battle
//Arielle's game source code

#include <iostream>
#include "fonts.h"


void ShowArielleName(int x, int y)
{
    	Rect r;
	r.bot = y - 170 ;
	r.left = x + 280;
	r.center = 1;
	ggprint8b(&r, 40, 0x00ffff44, "Arielle Battle");
}



