//Theodore Barcelona
//Updated: 10/2/18
//Theo's game source code
#include <math.h>
#include <GL/glx.h>
#include "fonts.h"

extern void showTheodoreName(int x, int y)
{
    Rect r;
    r.bot = y - 150;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Theodore Barcelona");
}

void showTheodorePicture(int x, int y, GLuint texid)
{
    //show picture
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

