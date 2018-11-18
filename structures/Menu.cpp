#include <GL/glx.h>

#include "globalTypes.h"
#include "log.h"

// lets use vector for buttons
Menu::Menu(Button buttons[], Mouse *mouse, BackgroundOptions options) {
    this->buttons = buttons;
    this->mouse = mouse;
    
    Rect rec;
    Button newButton;
    //Image bb();
    int i = 0;
    int x = gl.xres / 3;
    int y = gl.yres / 3;

    glGenTextures(1, &anahi::backgroundTex);
    glBindTexture(GL_TEXTURE_2D, anahi::backgroundTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bb.width, bb.height, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, bb.data);

    while (buttons[i] != (Button)nullptr) {
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

void Menu::checkButtons() {
    for (int i = 0; i < numButtons; i++) {
        buttons[i].over = 0;
        buttons[i].down = 0;
        // check for cursor over button
        if (mouse->x >= buttons[i].r.left &&
                mouse->x <= buttons[i].r.right &&
                mouse->y >= buttons[i].r.bot &&
                mouse->y <= buttons[i].r.top) {
            buttons[i].over = 1;
            buttons[i].down = mouse->lbutton || mouse->rbutton ? 1 : 0;
            Log("button id: %d, down: %d, over: %d", 
                buttons[i].id, buttons[i].down, buttons[i].over);
        }
    }   
}

void Menu::render() {
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