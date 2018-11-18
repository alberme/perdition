// author: Anahi Vargas
// date: 11/11/2018

/*** INCLUDE ***/
#include <GL/glx.h>
#include <string.h>
#include <vector>
#include <iostream>

#include "fonts.h"
#include "log.h"
#include "globalTypes.h"
#include "Global.h"

/*** MACROS ***/
#define BUTTON_TEXT_COLOR 0x00ffffff
#define IMG_BACKGROUND_PATH "./images/bb.png"

/** GLOBAL **/
extern Global gl;

/*** FUNCTION PROTOTYPES ***/
void checkButtons();
void initMenuButtons();

// avoids polluting global namespace
namespace anahi {
    /*** CONSTANTS ***/
    // const char *BUTTON_TEXT[] = { "Play", "Options", "Quit", nullptr };
    // const std::vector<Button> butts {
    //     { .text = "Play" },
    //     { .text = "Options" },
    //     { .text = "Quit" }
    // };
    // // add in 

    // use a vector
    const Button BUTTON_ATTRIBUTES[] = {
        { .text = "Play" },
        { .text = "Options" },
        { .text = "Quit" },
        nullptr
    };

    /***  ***/
    int initialized, nbuttons;
    GLuint backgroundTex;
    Button buttons[3];
    //std::map<std::string, Button> buttonss;
}

void initMenu() // consider the struct options
{   
    Rect rec;
    Button newButton;
    Image bb(IMG_BACKGROUND_PATH);
    int i = 0;
    int x = gl.xres / 3;
    int y = gl.yres / 3;

    glGenTextures(1, &anahi::backgroundTex);
    glBindTexture(GL_TEXTURE_2D, anahi::backgroundTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bb.width, bb.height, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, bb.data);

    while (anahi::BUTTON_TEXT[i] != nullptr) {
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
        strcpy(newButton.text, anahi::BUTTON_TEXT[i]);
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

        anahi::buttons[i++] = newButton;
        anahi::nbuttons++;
        y += 100;
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

void showMenu()
{
    //Log("mouse x: %d, y: %d, lbutt: %d, rbutt: %d\n", gl.mouse.x, gl.mouse.y, gl.mouse.lbutton, gl.mouse.rbutton);
    Rect r;
    
    if (!anahi::initialized) {
        Log("initMenu(): Initializing main menu\n");
        initMenu();
        anahi::initialized = 1;
    }
    checkButtons(); // check state of the buttons on screen
    //draw buttons, highlight the button with mouse cursor over

    // glPushMatrix();
    // glTranslatef(, , 0);
    glBindTexture(GL_TEXTURE_2D, anahi::backgroundTex);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-gl.xres,  gl.yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( gl.xres,  gl.yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( gl.xres, -gl.yres);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-gl.xres, -gl.yres);
    glEnd();
    // glPopMatrix();
    for (int i = 0; i < anahi::nbuttons; i++) {
        if (anahi::buttons[i].over) {
            //draw a red highlight around button
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(anahi::buttons[i].r.left-2,  anahi::buttons[i].r.bot-2);
                glVertex2i(anahi::buttons[i].r.left-2,  anahi::buttons[i].r.top+2);
                glVertex2i(anahi::buttons[i].r.right+2, anahi::buttons[i].r.top+2);
                glVertex2i(anahi::buttons[i].r.right+2, anahi::buttons[i].r.bot-2);
                glVertex2i(anahi::buttons[i].r.left-2,  anahi::buttons[i].r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        // set color for button
        if (anahi::buttons[i].down) {
            glColor3fv(anahi::buttons[i].dcolor);
  
        } else {
            glColor3fv(anahi::buttons[i].color);
        }
        // draw a button
        glBegin(GL_QUADS);
            glVertex2i(anahi::buttons[i].r.left,  anahi::buttons[i].r.bot);
            glVertex2i(anahi::buttons[i].r.left,  anahi::buttons[i].r.top);
            glVertex2i(anahi::buttons[i].r.right, anahi::buttons[i].r.top);
            glVertex2i(anahi::buttons[i].r.right, anahi::buttons[i].r.bot);
        glEnd();

        // text position
        r.left = anahi::buttons[i].r.centerx;
        r.bot  = anahi::buttons[i].r.centery-8;
        r.center = 1;
        // check button state
        // click state means the button is 
        if (anahi::buttons[i].down) {
            ggprint16(&r, 0, anahi::buttons[i].text_color, "Pressed!");
        } else if (anahi::buttons[i].click) {
            // Log("%s button clicked", anahi::buttons[i].); 
            anahi::buttons[i].onClick();
        } else {
            ggprint16(&r, 0, anahi::buttons[i].text_color, anahi::buttons[i].text);
        }
    }
}

//play button will go here
void play()
{
    gl.menu = 0;
}

//settings button will go here
void settings()
{

}

//quit button will go here
void quit()
{

}

//checks if mouse cursor is over a button
// void checkButtons() 
// {
//     for (int i = 0; i < anahi::nbuttons; i++) {
//         anahi::buttons[i].over = 0;
//         anahi::buttons[i].down = 0;
//         // check for cursor over button
//         if (gl.mouse.x >= anahi::buttons[i].r.left &&
//                 gl.mouse.x <= anahi::buttons[i].r.right &&
//                 gl.mouse.y >= anahi::buttons[i].r.bot &&
//                 gl.mouse.y <= anahi::buttons[i].r.top) {
//             anahi::buttons[i].over = 1;
//             anahi::buttons[i].down = gl.mouse.lbutton || gl.mouse.rbutton ? 1 : 0;
//             #ifdef VERBOSE
//             Log("button id: %d, down: %d, over: %d", 
//                 anahi::buttons[i].id, anahi::buttons[i].down, anahi::buttons[i].over);
//             #endif
//         }
//     }    
// }
