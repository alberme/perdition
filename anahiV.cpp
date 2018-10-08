// author: Anahi Vargas
// date: 10/7/2018
#include "fonts.h"
#include <GL/glx.h>

Rect r;

//show credit name
void showAnahiName(int x, int y)
{
    r.bot = y - 300;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Anahi Vargas");
}

//show credit picture
void showAnahiPicture(int x, int y, GLuint texid)
{
    float fx = (float)x;
    float fy  = (float)y;
    int wid = 40;
 
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid,  wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( wid,  wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( wid, -wid);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);

    glEnd();
    glPopMatrix();
}

//show settings icon top right
void showSettingsIcon(int x, int y, GLuint texid)
{
    float fx = (float)x;
    float fy  = (float)y;
    int wid = 22;
    
    //make icon background transparent
    //glClearColor(1, 1, 1, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_BLEND);
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid,  wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( wid,  wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( wid, -wid);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);

    glEnd();
    glPopMatrix();
}

//show settings options
void showSettings(int x, int y)
{
    r.bot = y - 30;
    r.left = x + 20;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ffff44, "C - Credits");
    ggprint8b(&r, 16, 0x00ffff44, "H - Help");
}

//show game control keys
void showHelp(int x, int y)
{
	for (int i = 0; i < 7; i++) {
	    r.left = x;
        r.bot = y  - 30;
	    r.center = 0;
	    if (i == 0) {
            ggprint8b(&r, 16, 0x00ffff44, "W   	Walk cycle");
        } else if (i ==1) {
            ggprint8b(&r, 16, 0x00ffff44, "D   	walk right");
        } else if (i == 2) {
            ggprint8b(&r, 16, 0x00ffff44, "A   	walk left");
        } else if (i == 3) {
            ggprint8b(&r, 16, 0x00ffff44, "E   	Explosion");
        } else if (i == 4) {
            ggprint8b(&r, 16, 0x00ffff44, "+   	faster");
        } else if (i == 5) {
            ggprint8b(&r, 16, 0x00ffff44, "-   	slower");
        } else if (i == 6) {
            ggprint8b(&r, 16, 0x00ffff44, "Spacebar	jump");
        }
        //ggprint8b(&r, 16, 0x00ffff44, "frame: %i", z);
        y -= 20;
   }
}
