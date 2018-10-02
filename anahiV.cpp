#include "fonts.h"
#include <GL/glx.h>
using namespace std;

extern void showAnahiName(int x, int y)
{
    Rect r;
    r.bot = y - 210;
    r.left = x + 280;
    r.center = 1;
    ggprint8b(&r, 40, 0x00ffff44, "Anahi Vargas");

}
void showTinaImage(int x, int y, GLuint texid)
{
    //show Tina image
    float fx = (float)x;
    float fy  = (float)y;
    glColor3ub(255, 255, 255);

    int wid = 40;
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

