#include <GL/glx.h>
#include <string.h>
#include <vector>

#include "globalTypes.h"
#include "log.h"

extern Global gl;

Menu::Menu(std::vector<Button>& initButtons, Mouse *mouse, BackgroundOptions options) : buttons(initButtons)
{
    this->mouse = mouse;
    numButtons = buttons.size();
    
    Rect rec;
    Button newButton;

    if (options.imagePath == nullptr) {
        color = options.color;
        hasImage = 0;
    } else {
        Image img(options.imagePath);
        hasImage = 1;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width, img.height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img.data);
    }

    // to determine inital x and y, you need to divide x and y by 3, then multiply that result by 3
    int x = options.res.xres / numButtons;
    int y = options.res.yres - (options.res.yres / numButtons);

    for (int i = 0; i < numButtons; ++i) {
        // dimensions of button
        rec.width = buttons[i].r.width || 200;
        rec.height = buttons[i].r.height || 50;
        rec.left = (buttons[i].r.left || 10) + x;
        rec.bot = (buttons[i].r.bot || 10) + y;
        rec.right = rec.left + rec.width;
        rec.top = rec.bot + rec.height;
        rec.centerx = (rec.left + rec.right) / 2;
        rec.centery = (rec.bot + rec.top) / 2;

      // button attributes and states
        strcpy(newButton.text, buttons[i].text);
        MakeRGBA(newButton.color, 0.4f, 0.4f, 0.7f, 1.0f);
        MakeRGBA(newButton.dcolor, newButton.color[0] * 0.5f, 
            newButton.color[1] * 0.5f, 
            newButton.color[2] * 0.5f,
            1.0f
        );
        newButton.id = i;
        newButton.r = rec;
        newButton.down = 0;
        newButton.click = 0;
        newButton.text_color = buttons[i].text_color;

        this->buttons[i] = newButton;
        y += 100; // could either do this or scale it according to yres
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
        }
    }   
}

void Menu::render() {
    Rect r;

    checkButtons(); // check state of the buttons on screen
    //draw buttons, highlight the button with mouse cursor over

    // glPushMatrix();
    // glTranslatef(, , 0);
    if (hasImage) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(-gl.xres,  gl.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i( gl.xres,  gl.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i( gl.xres, -gl.yres);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(-gl.xres, -gl.yres);
        glEnd();
    }
    
    // glPopMatrix();
    for (int i = 0; i < numButtons; i++) {
        if (buttons[i].over) {
            //draw a red highlight around button
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(buttons[i].r.left-2,  buttons[i].r.bot-2);
                glVertex2i(buttons[i].r.left-2,  buttons[i].r.top+2);
                glVertex2i(buttons[i].r.right+2, buttons[i].r.top+2);
                glVertex2i(buttons[i].r.right+2, buttons[i].r.bot-2);
                glVertex2i(buttons[i].r.left-2,  buttons[i].r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        // set color for button
        if (buttons[i].down) {
            glColor3fv(buttons[i].dcolor);
  
        } else {
            glColor3fv(buttons[i].color);
        }
        // draw a button
        glBegin(GL_QUADS);
            glVertex2i(buttons[i].r.left,  buttons[i].r.bot);
            glVertex2i(buttons[i].r.left,  buttons[i].r.top);
            glVertex2i(buttons[i].r.right, buttons[i].r.top);
            glVertex2i(buttons[i].r.right, buttons[i].r.bot);
        glEnd();

        // text position
        r.left = buttons[i].r.centerx;
        r.bot  = buttons[i].r.centery - 8;
        r.center = 1;

        // check button state
        if (buttons[i].down) {
            ggprint16(&r, 0, buttons[i].text_color, "Pressed!");
        } else if (buttons[i].click) {
            Log("button press --- id: %d, down: %d, over: %d", 
                buttons[i].id, buttons[i].down, buttons[i].over);
            try {
                buttons[i].onClick();
            } catch (const std::exception& e) {
            Log("Error: Menu::buttons[%d].onClick() - \
                Exception caught in event handler:\n\t%s", 
                buttons[i].id, 
                e.what()
            );
            throw e;
            }  
        } else {
            ggprint16(&r, 0, buttons[i].text_color, buttons[i].text);
        }
    }


}