//Arielle Battle
//Arielle's game source code

#include <iostream>
#include "font.h"


void ShowArielleName(int x, int y)
{
    	rect r;
	unsigned int c = x;
    	unsigned int c = 0x00ffff44;
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, c, "Arielle Battle");
}



