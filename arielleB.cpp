//Arielle Battle
//Arielle's game source code
//Description: my source code contains the function for showing my name and 
//	       my picture in the main walk2.cpp file

#include "fonts.h"
#include <GL/glx.h>

void ShowArielleName(int x, int y)
{
    Rect r;
    r.bot = y - 100;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Arielle Battle");
}

void showAriellePic(int x, int y, GLuint texid)
{
    static float angle2 = 0.0f;

	glColor3ub(225, 255, 255);
	int wid = 40;
	glPushMatrix();
	glTranslated(x, y, 0);
	glRotatef(angle2, 0.0f, 0.0f, 1.0f);	//rotates the picture
	angle2 += 1;	//the speed of the picture
	glBindTexture(GL_TEXTURE_2D, texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
	glEnd();
	glPopMatrix();
}


