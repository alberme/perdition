//Credits Function & My Sourcefile for Game Project: Perdition
//By: Theodore Barcelona
//Group #: 2
//Updated: 10/8/18
//Theo's game source code
//Description: show name/picture in credits
#include <math.h>
#include <GL/glx.h>
#include "fonts.h"
//Sound that will be implemented later
//#include <stdio.h>
//#include <string.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#ifdef USE_OPENAL_SOUND
//#include </usr/include/AL/alut.h>
//#endif

void showTheodoreName(int x, int y)
{
    //show name and position
    Rect r;
    r.bot = y - 0;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Theodore Barcelona");
}
void showTheodorePicture(int x, int y, GLuint texid)
{
    //show picture and position
    float fx = (float)x;
    float fy = (float)y;
    glColor3ub(255, 255, 255);
    
    int wid=40;
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,-wid);
         glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
         glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, wid);
         glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();
}
//void soundTest(int x,  int y)
//{
// Sound will be implemented here.
//}
