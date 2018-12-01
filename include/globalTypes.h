#ifndef _GLOBALTYPES_H_
#define _GLOBALTYPES_H_

#include <GL/glx.h>
#include <stdlib.h>
#include <vector>

#include "fonts.h"

/** MACRO FUNCTIONS **/
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define MakeRGBA(v, r, g, b, a) (v)[0]=(r);(v)[1]=(g);(v)[2]=(b);(v)[3]=(a);
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                    (c)[1]=(a)[1]-(b)[1]; \
                    (c)[2]=(a)[2]-(b)[2]

/*** ENUMS ***/
// enum class BackgroundType { ImagePath, RGBA };

/*** TYPES ***/
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];
typedef struct t_resolution {
    int xres;
    int yres;
} XResolution;
typedef struct t_button {
    Rect r;
    void (*onClick)();
    char text[32];
    int id;
    int over;   // cursor is over the button
    int down;   // cursor is holding down on the button
    int click;  // cursor has stopped holding down inside button perimeter, registering as a button click
    float color[4];
    float dcolor[4];
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
typedef struct t_options_background {
    float color[4];
    const char *imagePath;
    XResolution res;
} BackgroundOptions;

/** CLASSES **/

class Image {
public:
    int width, height;
    unsigned char *data;
    ~Image();
    Image(const char *);
};

class Menu {
private:
    std::vector<Button>& buttons;
    Mouse *mouse;
    float *color;
    GLuint textureId;
    int hasImage;
    void checkButtons();
public:
    int numButtons;
    bool active;
    Rect rect;
    Menu(std::vector<Button>&, Mouse*, BackgroundOptions);
    void render();
};

class Level {
public:
	unsigned char arr[16][80];
	int nrows, ncols;
	int tilesize[2];
	Flt ftsz[2];
	Flt tile_base;
	Level();
	void removeCrLf(char *);
};

class Sprite {
public:
    int onoff;
    int frame;
    double delay;
    Vec pos;
    Image *image;
    GLuint tex;
    struct timespec time;
    Sprite();
};

class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
	Timers();
	double timeDiff(struct timespec*, struct timespec*);
	void timeCopy(struct timespec*, struct timespec*);
	void recordTime(struct timespec*);
};

class X11_wrapper {
private:
    Display *dpy;
    Window win;
    XResolution res;
    char windowTitle[40];
    void (*onWindowResize)(XResolution);
public:
    X11_wrapper(GLint[], XResolution, long, const char[], void (*)(XResolution));
    ~X11_wrapper();
    void setTitle(const char[]);
    void setupScreenRes(const int, const int);
    void reshapeWindow(const int, const int);
    void checkResize(XEvent*);
    bool getXPending();
    XEvent getXNextEvent();
    void swapBuffers();
};

#ifndef NO_GLOBAL_CLASS_H
/** 
 * Place any member declaration changes from Global class definition 
 * in walk2.cpp here
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
#endif // NO_GLOBAL_CLASS_H
#endif // _GLOBAL_TYPES_H_