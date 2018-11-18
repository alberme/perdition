#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "globalTypes.h"

/**
 * Any changes made to the Global definition in walk2.cpp will also need to
 * be applied here so that other files that extern Global gl 
 * can see those changes too
 */
class Global {
public:
    unsigned char keys[65536];
    int xres, yres;
    int movie, movieStep;
    int walk;
    int credits;
    int walkFrame;
    int settings;
    int helpTab;
    int menu;
    double delay;
    Image *walkImage;
    GLuint walkTexture;
    GLuint mariogm734Texture;
    GLuint tinaTexture;
    GLuint animeTexture;
    GLuint jeremyTexture;
    GLuint cactusTexture;
    GLuint enemy1Texture;
    GLuint goblinTexture;
    GLuint settings_icon_Texture;
    Vec box[20];
    Sprite exp;
    Sprite exp44;
    Vec ball_pos;
    Vec ball_vel;
    //camera is centered at (0,0) lower-left of screen. 
    Flt camera[2];
    Mouse mouse;
    ~Global();
    Global();
};

#endif // _GLOBAL_H_