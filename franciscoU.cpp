/* Author: Francisco Ulloa
* Date Modified: 9/26/18
*/

#include "fonts.h"
#include <GL/glx.h>

/*
* Notes: how to make cactus transparent in helpTab
*/

extern void showFranciscoName(int x, int y)
{
    Rect r;
    r.bot = y - 200;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Francisco Ulloa");
}

void showFranciscoPicture(int x, int y, GLuint texid)
{
    float fx = (float)x;
    float fy = (float)y;
    static float angle2 = 0.0f;

    glColor3ub(255, 255, 255);
    int wid = 40;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glRotatef(angle2, 0.0f, 0.0f, 1.0f);
    angle2 += 0.2;
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
   	glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid, wid);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(wid, wid);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(wid, -wid);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);
    glEnd();
    glPopMatrix();
}

void showHelpTab(int x, int y, GLuint texid)
{
    float fx = (float)x;
    float fy = (float)y;

    glColor3ub(255, 255, 255);
    int wid = 40;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
   	glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid, wid);
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(wid, wid);
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(wid, -wid);
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);
    glEnd();
    glPopMatrix();
}

extern void showHelpText(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ff00, "This is Clyde The Cactus. It doesn ## damage.");
}
