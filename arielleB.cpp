//Arielle Battle
//Arielle's game source code

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>

void ShowArielleName(int x, int y)
{
    Rect r;
    r.bot = y - 170 ;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Arielle Battle");
}

void showAriellePic(int x, int y, GLuint texid)
{
	glColor3ub(225, 255, 255);
	int wid = 40;
	glPushMatrix();
	glTranslated(200, 200, 0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
	glEnd();
	glPopMatrix();
}


