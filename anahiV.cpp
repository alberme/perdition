// author: Anahi Vargas
// date: 10/7/2018

/*** INCLUDE ***/
#include "fonts.h"
#include "log.h"
#include <GL/glx.h>
#include <X11/Xlib.h> // probably dont need this anymore
#include <string.h>
#include <map>

/*** MACROS ***/
#define BUTTON_TEXT_COLOR 0x00ffffff
// possibly define NUM_BUTTONS 3
#define MakeRGB(v, r, g, b) (v)[0]=(r);(v)[1]=(g);(v)[2]=(b);

/*** FUNCTION PROTOTYPES ***/
void checkButtons();
void initMenuButtons();

/*** CONSTANTS ***/
const char *BUTTON_TEXT[] = { "Play", "Options", "Quit", nullptr };


// i wanna throw these types into something called globalTypes.h
/*** TYPES ***/
typedef struct t_button {
    // we can also store functions pointers in here
    Rect r;
    void (*onClick)();
    char text[32];
    int id;
    int over;   // cursor is over the button
    int down;   // cursor is holding down on the button
    int click;  // cursor has stopped holding down inside button perimeter, registering as a button click
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} Button;

typedef struct t_mouse {
    // maybe a button state here, buttonrelease, buttonpress
    int eventType;
	int lbutton;
	int rbutton;
	int x;
	int y;
} Mouse;

/*** CLASSES ***/
class Global {
public:
    int initialized;
    int nbuttons;
    // std::map<char [], Button> buttonss;
    Button buttons[3];
    Global() {
        nbuttons = 0;
        initMenuButtons();
    }
} g;

void initMenuButtons() // i think its better to send an init button construct like a hash table or objects or something
{   
    Rect rec;
    Button newButton;
    int i = 0;
    int x = 0;
    int y = 20;

    while (BUTTON_TEXT[i] != nullptr) {
        // dimensions of button
        rec.width = 200;
        rec.height = 50;
        rec.left = 10 + x;
        rec.bot = 10 + y;
        rec.right = rec.left + rec.width;
        rec.top = rec.bot + rec.height;
        rec.centerx = (rec.left + rec.right) / 2;
        rec.centery = (rec.bot + rec.top) / 2;

        // button attributes and states
        strcpy(newButton.text, BUTTON_TEXT[i]);
        MakeRGB(newButton.color, 0.4f, 0.4f, 0.7f);
        MakeRGB(newButton.dcolor, newButton.color[0] * 0.5f, 
            newButton.color[1] * 0.5f, 
            newButton.color[2] * 0.5f
        );
        newButton.id = i;
        newButton.r = rec;
        newButton.down = 0;
        newButton.click = 0;
        newButton.text_color = BUTTON_TEXT_COLOR;

        g.buttons[i++] = newButton;
        g.nbuttons++;
        x += 20;
    }
}


//show credit name
void showAnahiName(int x, int y)
{
    Rect r;

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
    
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid,  wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( wid,  wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( wid, -wid);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);

        //make icon background transparent
        glClear(GL_COLOR_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glDisable(GL_BLEND);
    glEnd();
    glPopMatrix();
}

//show settings options
void showSettings(int x, int y)
{
    Rect r;

    r.bot = y - 30;
    r.left = x + 20;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ffff44, "C - Credits");
    ggprint8b(&r, 16, 0x00ffff44, "H - Help");
}

//show game control keys
void showHelp(int x, int y)
{
    Rect r;

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

void showMenu(const Mouse mouse) 
{
    Rect r;

    // check state of the buttons on screen
    checkButtons(mouse);
    //draw buttons, highlight the button with mouse cursor over
    for (int i = 0; i < g.nbuttons; i++) {
        if (g.buttons[i].over) {
            //draw a highlight around buttons
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(g.buttons[i].r.left-2,  g.buttons[i].r.bot-2);
                glVertex2i(g.buttons[i].r.left-2,  g.buttons[i].r.top+2);
                glVertex2i(g.buttons[i].r.right+2, g.buttons[i].r.top+2);
                glVertex2i(g.buttons[i].r.right+2, g.buttons[i].r.bot-2);
                glVertex2i(g.buttons[i].r.left-2,  g.buttons[i].r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        // set color for button
        if (g.buttons[i].down) {
            glColor3fv(g.buttons[i].dcolor);
  
        } else {
            glColor3fv(g.buttons[i].color);
        }
        // draw a button
        glBegin(GL_QUADS);
            glVertex2i(g.buttons[i].r.left,  g.buttons[i].r.bot);
            glVertex2i(g.buttons[i].r.left,  g.buttons[i].r.top);
            glVertex2i(g.buttons[i].r.right, g.buttons[i].r.top);
            glVertex2i(g.buttons[i].r.right, g.buttons[i].r.bot);
        glEnd();

        // text position
        r.left = g.buttons[i].r.centerx;
        r.bot  = g.buttons[i].r.centery-8;
        r.center = 1;
        // check button state
        // click state means the button is 
        if (g.buttons[i].down) {
            ggprint16(&r, 0, g.buttons[i].text_color, "Pressed!");
            g.buttons[i].onClick();
        } else {
            ggprint16(&r, 0, g.buttons[i].text_color, g.buttons[i].text);
        }
    }
}

void play()
{

}

void settings()
{

}

void quit()
{

}

//checks if mouse cursor is over a button
void checkButtons(const Mouse mouse) 
{
    for (int i = 0; i < g.nbuttons; i++) {
        g.buttons[i].over = 0;
        g.buttons[i].down = 0;
        // check for cursor over button
        if (mouse.x >= g.buttons[i].r.left &&
                mouse.x <= g.buttons[i].r.right &&
                mouse.y >= g.buttons[i].r.bot &&
                mouse.y <= g.buttons[i].r.top) {
            g.buttons[i].over = 1;
            g.buttons[i].down = mouse.lbutton || mouse.rbutton ? 1 : 0;
            #ifdef VERBOSE
            Log("button id: %d, down: %d, over: %d", 
                g.buttons[i].id, g.buttons[i].down, g.buttons[i].over);
            #endif
        } // else if cursor up inside button then click = 1;
    }

   // if left button is down, go ahead and render whatever the button correlates to        
}
