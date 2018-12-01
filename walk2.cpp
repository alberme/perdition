//3350
//program: PERDITION.cpp
//author:  GROUP 2
//date:    fall 2018
//
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//framework courtesy: Gordon Giesel
//
//This program includes:
//  multiple sprite-sheet animations
//  a level tiling system
//  parallax scrolling of backgrounds
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

#include "log.h"
//#include "ppm.h"
#include "fonts.h"
#define NO_GLOBAL_CLASS_H /* prevents Global class header from being included
						preventing redeclaration compilation error */
#include "globalTypes.h"

using namespace std;

/** MACROS **/
// macro functions are in globalTypes.h
#define SPACE_BAR 0x20
#define ALPHA 1
//#define MENU_ANAHI
#define XWIN_TITLE "3350 - PERDITION"
#define XWIN_EVENT_MASK ExposureMask | KeyPressMask | KeyReleaseMask |    \
					ButtonPress | ButtonReleaseMask | PointerMotionMask | \
					StructureNotifyMask | SubstructureNotifyMask

/** CONSTANTS **/
const float timeslice = 1.0f;
const float gravity = -0.2f;
const Image img[11] = {
    "./images/walk.gif",
    "./images/exp.png",
    "./images/exp44.png",
    "./images/mariogm734.png",
    "./images/anime.png",
    "./images/jeremy.gif",
    "./images/tina.png",
    "./images/cactus.png",
    "./images/enemy1.png",
    "./images/goblin.png",
    "./images/settings_icon.png"};

bool push_start = false;

/** PROTOTYPES **/
void initOpengl();
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics();
void render();
void onWindowResize(XResolution);

/** GLOBAL OBJECTS **/

/**
 * left Global here for convenience sake. If you add, remove, change
 * member declarations in this Global definition, you will also need to apply
 * those changes to globalTypes.h so that other files that extern Global gl
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
	~Global() {
	    logClose();
	}
	Global() {
	    logOpen();
	    camera[0] = camera[1] = 0.0;
	    movie=0;
	    movieStep=2;
	    xres=800;
	    yres=600;
	    walk=0;
	    credits =0;
	    settings = 0;
	    walkFrame=0;
        menu = 1;   // start off with menu
	    walkImage=NULL;
	    MakeVector(ball_pos, 520.0, 0, 0);
	    MakeVector(ball_vel, 0, 0, 0);
	    delay = 0.1;
	    exp.onoff=0;
	    exp.frame=0;
	    exp.image=NULL;
	    exp.delay = 0.02;
	    exp44.onoff=0;
	    exp44.frame=0;
	    exp44.image=NULL;
	    exp44.delay = 0.022;
	    for (int i=0; i<20; i++) {
            box[i][0] = rnd() * xres;
            box[i][1] = rnd() * (yres-220) + 220.0;
            box[i][2] = 0.0;
	    }
	    memset(keys, 0, 65536);
	}
} gl;

Timers timers;		//Setup timers
X11_wrapper x11(
	(GLint[]) { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None },
	(XResolution) { xres: gl.xres, yres: gl.yres },
	XWIN_EVENT_MASK,
	XWIN_TITLE,
	onWindowResize
);	// X Windows variables
Level lev;

int main(void)
{
    initOpengl();
    init();
    int done = 0;
    while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
	/*if (getchar() == ' ')
	  {
	  extern void jump();
	  jump();
	  }*/
	physics();
	render();
		x11.swapBuffers();
    }
    cleanup_fonts();
    return 0;
}

void onWindowResize(XResolution res) {
	Log("Window resized to %dx%d\n", res.xres, res.yres);
	gl.xres = res.xres;
	gl.yres = res.yres;
}

unsigned char *buildAlphaData(const Image *img)
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a,b,c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data+0);
    unsigned char t1 = *(data+1);
    unsigned char t2 = *(data+2);
    for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
    }
    return newdata;
}

void initOpengl(void)
{
    //-------------------------------------------------------------------------
    glGenTextures(1, &gl.mariogm734Texture);
    //-------------------------------------------------------------------------
    //mario texture
    //
    int w_mario = img[3].width;
    int h_mario = img[3].height;
    //
    glBindTexture(GL_TEXTURE_2D, gl.mariogm734Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_mario, h_mario, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[3].data);
    //-------------------------------------------------------------------------
    //
    glGenTextures(1, &gl.animeTexture);
    //-------------------------------------------------------------------------
    //anime texture
    //
    int w_anime = img[4].width;
    int h_anime = img[4].height;
    //
    glBindTexture(GL_TEXTURE_2D, gl.animeTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_anime, h_anime, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[4].data);
    //-------------------------------------------------------------------------
    //
    glGenTextures(1, &gl.jeremyTexture);
    //-------------------------------------------------------------------------
    //jeremy texture
    //
    int w_jeremy = img[5].width;
    int h_jeremy = img[5].height;
    //
    glBindTexture(GL_TEXTURE_2D, gl.jeremyTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_jeremy, h_jeremy, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
    //-------------------------------------------------------------------------
    //OpenGL initialization
    glGenTextures(1, &gl.tinaTexture);
    //-------------------------------------------------------------------------
    //tina texture
    //
    int w_tina = img[6].width;
    int h_tina  = img[6].height;
    //
    glBindTexture(GL_TEXTURE_2D, gl.tinaTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_tina, h_tina, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[6].data);
    //-------------------------------------------------------------------------

    //helpTab
    //cactus texture
    //
    glGenTextures(1, &gl.cactusTexture);

    int w_cactus = img[7].width;
    int h_cactus = img[7].height;

    glBindTexture(GL_TEXTURE_2D, gl.cactusTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_cactus, h_cactus, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[7].data);
    //-------------------------------------------------------------------------
    glGenTextures(1, &gl.enemy1Texture);
    //-------------------------------------------------------------------------
    //enemy1 texture
    //
    int w_enemy1 = img[8].width;
    int h_enemy1 = img[8].height;
    //
    glBindTexture(GL_TEXTURE_2D, gl.enemy1Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_enemy1, h_enemy1, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[8].data);
    glGenTextures(1, &gl.settings_icon_Texture);
    //-------------------------------------------------------------------------
    glGenTextures(1, &gl.goblinTexture);
    //-------------------------------------------------------------------------
    //goblin texture
    //
    int w_goblin = img[9].width;
    int h_goblin = img[9].height;
    //
    glBindTexture(GL_TEXTURE_2D, gl.goblinTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_goblin, h_goblin, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[9].data);
    //settings icon texture
    //
    int w_settings_icon = img[10].width;
    int h_settings_icon  = img[10].height;
    //
    glBindTexture(GL_TEXTURE_2D, gl.settings_icon_Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_settings_icon, h_settings_icon, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, img[10].data);
    //-------------------------------------------------------------------------

    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //
    int w = img[0].width;
    int h = img[0].height;
    //
    //create opengl texture elements
    glGenTextures(1, &gl.walkTexture);
    //-------------------------------------------------------------------------
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *walkData = buildAlphaData(&img[0]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    free(walkData);
    //-------------------------------------------------------------------------
    //create opengl texture elements
    w = img[1].width;
    h = img[1].height;
    glGenTextures(1, &gl.exp.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    unsigned char *xData = buildAlphaData(&img[1]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, xData);
    free(xData);
    //-------------------------------------------------------------------------
    w = img[2].width;
    h = img[2].height;
    //create opengl texture elements
    glGenTextures(1, &gl.exp44.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    xData = buildAlphaData(&img[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, xData);
    free(xData);
}

void init() {

}

void checkMouse(XEvent *e)
{
    //printf("checkMouse()...\n"); fflush(stdout);
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;

    if (e->type != ButtonRelease && e->type != ButtonPress &&
	    e->type != MotionNotify)
        return;
    if (e->type == ButtonRelease) {
        if (e->xbutton.button == 1) {
            //Left button is down
            gl.mouse.lbutton = 0;
        }
        if (e->xbutton.button == 3) {
            //Right button is down
            gl.mouse.rbutton = 0;
        }
        return;
    }
    if (e->type == ButtonPress) {

        if (e->xbutton.button==1) {
            //Left button is down
            gl.mouse.lbutton = 1;
            push_start = true;

        }
        if (e->xbutton.button==3) {
            //Right button is down
            gl.mouse.rbutton = 1;
            push_start = true;

        }
    }
    if (e->type == MotionNotify) {
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            //Mouse moved
            savex = e->xbutton.x;
            savey = e->xbutton.y;
            gl.mouse.x = e->xbutton.x;
            gl.mouse.y = e->xbutton.y;
        }
    }
}

void screenCapture()
{
    static int fnum = 0;
    static int vid = 0;
    if (!vid) {
	system("mkdir ./vid");
	vid = 1;
    }
    unsigned char *data = (unsigned char *)malloc(gl.xres * gl.yres * 3);
    glReadPixels(0, 0, gl.xres, gl.yres, GL_RGB, GL_UNSIGNED_BYTE, data);
    char ts[32];
    sprintf(ts, "./vid/pic%03i.ppm", fnum);
    FILE *fpo = fopen(ts,"w");	
    if (fpo) {
	fprintf(fpo, "P6\n%i %i\n255\n", gl.xres, gl.yres);
	unsigned char *p = data;
	//go backwards a row at a time...
	p = p + ((gl.yres-1) * gl.xres * 3);
	unsigned char *start = p;
	for (int i=0; i<gl.yres; i++) {
	    for (int j=0; j<gl.xres*3; j++) {
		fprintf(fpo, "%c",*p);
		++p;
	    }
	    start = start - (gl.xres*3);
	    p = start;
	}
	fclose(fpo);
	char s[256];
	sprintf(s, "convert ./vid/pic%03i.ppm ./vid/pic%03i.gif", fnum, fnum);
	system(s);
	unlink(ts);
    }
    ++fnum;
}

int checkKeys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    if (e->type != KeyPress && e->type != KeyRelease)
	return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    gl.keys[key]=1;
    if (e->type == KeyRelease) {
	gl.keys[key]=0;
	if (key == XK_Shift_L || key == XK_Shift_R)
	    shift=0;
	return 0;
    }
    gl.keys[key]=1;
    if (key == XK_Shift_L || key == XK_Shift_R) {
	shift=1;
	return 0;
    }
    (void)shift;
    switch (key) {
	case XK_s:
	    screenCapture();
	    push_start = true;
	    break;
	case XK_m:
	    gl.movie ^= 1;
	    break;
	case XK_w:
	    timers.recordTime(&timers.walkTime);
	    gl.walk ^= 1;
	    break;
	case XK_e:
	    exit(0);
	    break;
	case XK_f:
	    gl.exp44.pos[0] = 200.0;
	    gl.exp44.pos[1] = -60.0;
	    gl.exp44.pos[2] =   0.0;
	    timers.recordTime(&gl.exp44.time);
	    gl.exp44.onoff ^= 1;
	    break;
	case XK_Left:
	    break;
	case XK_Right:
	    break;
	case XK_Up:
	    break;
	case XK_Down:
	    break;
	case XK_equal:
	    gl.delay -= 0.005;
	    if (gl.delay < 0.005)
		gl.delay = 0.005;
	    break;
	case XK_minus:
	    gl.delay += 0.005;
	    break;
	case XK_Escape:
	    return 1;
	    break;
	case XK_o:
	    gl.settings ^= 1;
	    break;
	case XK_c:
	    gl.credits ^= 1;
	    break;
	case XK_h:
	    gl.helpTab ^= 1;
	    break;	
	case XK_space:
	    break;
    }
    return 0;
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen*xlen + ylen*ylen + zlen*zlen;
    if (len == 0.0) {
	MakeVector(vec, 0.0, 0.0, 1.0);
	return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return(len);
}

void physics(void)
{
    if (gl.walk || gl.keys[XK_Right] || gl.keys[XK_Left]) {
	//man is walking...
	//when time is up, advance the frame.
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
	if (timeSpan > gl.delay) {
	    //advance
	    ++gl.walkFrame;
	    if (gl.walkFrame >= 16)
		gl.walkFrame -= 16;
	    timers.recordTime(&timers.walkTime);
	}
	for (int i=0; i<20; i++) {
	    if (gl.keys[XK_Left]) {
		gl.box[i][0] += 1.0 * (0.05 / gl.delay);
		if (gl.box[i][0] > gl.xres + 10.0)
		    gl.box[i][0] -= gl.xres + 10.0;
		gl.camera[0] -= 2.0/lev.tilesize[0] * (0.05 / gl.delay);
		if (gl.camera[0] < 0.0)
		    gl.camera[0] = 0.0;
	    } else {
		gl.box[i][0] -= 1.0 * (0.05 / gl.delay);
		if (gl.box[i][0] < -10.0)
		    gl.box[i][0] += gl.xres + 10.0;
		gl.camera[0] += 2.0/lev.tilesize[0] * (0.05 / gl.delay);
		if (gl.camera[0] < 0.0)
		    gl.camera[0] = 0.0;
	    }
	}
	if (gl.exp.onoff) {
	    gl.exp.pos[0] -= 2.0 * (0.05 / gl.delay);
	}
	if (gl.exp44.onoff) {
	    gl.exp44.pos[0] -= 2.0 * (0.05 / gl.delay);
	}
    }
    if (gl.exp.onoff) {
	//explosion is happening
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&gl.exp.time, &timers.timeCurrent);
	if (timeSpan > gl.exp.delay) {
	    //advance explosion frame
	    ++gl.exp.frame;
	    if (gl.exp.frame >= 23) {
		//explosion is done.
		gl.exp.onoff = 0;
		gl.exp.frame = 0;
	    } else {
		timers.recordTime(&gl.exp.time);
	    }
	}
    }
    if (gl.exp44.onoff) {
	//explosion is happening
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&gl.exp44.time, &timers.timeCurrent);
	if (timeSpan > gl.exp44.delay) {
	    //advance explosion frame
	    ++gl.exp44.frame;
	    if (gl.exp44.frame >= 16) {
		//explosion is done.
		gl.exp44.onoff = 0;
		gl.exp44.frame = 0;
	    } else {
		timers.recordTime(&gl.exp44.time);
	    }
	}
    }
    //====================================
    //Adjust position of ball.
    //Height of highest tile when ball is?
    //====================================
    Flt dd = lev.ftsz[0];
    int col = (int)((gl.camera[0]+gl.ball_pos[0]) / dd);
    col = col % lev.ncols;
    int hgt = 0;
    for (int i=0; i<lev.nrows; i++) {
	if (lev.arr[i][col] != ' ') {
	    hgt = (lev.nrows-i) * lev.tilesize[1];
	    break;
	}
    }
    if (gl.ball_pos[1] < (Flt)hgt) {
	gl.ball_pos[1] = (Flt)hgt;
	MakeVector(gl.ball_vel, 0, 0, 0);
    } else {
	gl.ball_vel[1] -= 0.9;
    }
    gl.ball_pos[1] += gl.ball_vel[1];
}

void render(void)
{	
#ifdef MENU_ANAHI
    if (gl.menu) {
        extern void showMenu();
        showMenu();
		return;
    }
#else
    if(!push_start)	{
	extern void menu(int x, int y);
	menu(100, gl.yres-155);
#endif
	if (gl.credits) {

		//display names
		extern void showFranciscoName(int x, int y);
		extern void showAnahiName(int x, int y);
		extern void showTheodoreName(int x, int y);
		extern void ShowArielleName(int x, int y);

		showFranciscoName(100, gl.yres-155);
		showAnahiName(100, gl.yres-175);
		showTheodoreName(100, gl.yres-105);
		ShowArielleName(100, gl.yres-135);

		//displays images
		extern void showAnahiPicture(int x, int y, GLuint texid);
		extern void showFranciscoPicture(int x, int y, GLuint texid);
		extern void showTheodorePicture(int x, int y, GLuint texid);
		extern void showAriellePic(int x, int y, GLuint texid);

		showAnahiPicture(250, gl.yres-475, gl.tinaTexture);
		showFranciscoPicture(250, gl.yres-350, gl.jeremyTexture);
		showTheodorePicture(250, gl.yres-100, gl.mariogm734Texture);
		showAriellePic(250, gl.yres-220, gl.animeTexture);

		return;
	}
#ifndef MENU_ANAHI
    } else {
#endif
	Rect r;
	//Clear the screen
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);


	// show settings icon top right
	extern void showSettingsIcon(int x, int y, GLuint texid);
	showSettingsIcon(gl.xres-30, gl.yres-30, gl.settings_icon_Texture);

	//display settings
	if (gl.settings) {
		extern void showSettings(int x, int y);
		showSettings(100, gl.yres-100);
		return;
	}

	if (gl.helpTab) {
		extern void showHelpTab(int x, int y, GLuint texid);
		extern void showHelpText(int x, int y);
		extern void showHelp(int x, int y);
		showHelpTab(250, 475, gl.cactusTexture);
		showHelpText(450, 450); 
		showHelp(50, gl.yres-60);
		return;
	}


	float cx = gl.xres/2.0;
	float cy = gl.yres/2.0;
	//
	//show ground
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.2);
	glVertex2i(0,       220);
	glVertex2i(gl.xres, 220);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2i(gl.xres,   0);
	glVertex2i(0,         0);
	glEnd();
	//
	//show boxes as background
	for (int i=0; i<20; i++) {
		glPushMatrix();
		glTranslated(gl.box[i][0],gl.box[i][1],gl.box[i][2]);
		glColor3f(0.2, 0.2, 0.2);
		glBegin(GL_QUADS);
		glVertex2i( 0,  0);
		glVertex2i( 0, 30);
		glVertex2i(20, 30);
		glVertex2i(20,  0);
		glEnd();
		glPopMatrix();
	}
	//
	//========================
	//Render the tile system
	//========================
	int tx = lev.tilesize[0];
	int ty = lev.tilesize[1];
	Flt dd = lev.ftsz[0];
	Flt offy = lev.tile_base;
	int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
	int col = (int)(gl.camera[0] / dd);
	col = col % lev.ncols;
	//Partial tile offset must be determined here.
	//The leftmost tile might be partially off-screen.
	//cdd: camera position in terms of tiles.
	Flt cdd = gl.camera[0] / dd;
	//flo: just the integer portion
	Flt flo = floor(cdd);
	//dec: just the decimal portion
	Flt dec = (cdd - flo);
	//offx: the offset to the left of the screen to start drawing tiles
	Flt offx = -dec * dd;
	//Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
	for (int j=0; j<ncols_to_render; j++) {
		int row = lev.nrows-1;
		for (int i=0; i<lev.nrows; i++) {
		if (lev.arr[row][col] == 'w') {
			glColor3f(0.8, 0.8, 0.6);
			glPushMatrix();
			//put tile in its place
			glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
			glBegin(GL_QUADS);
			glVertex2i( 0,  0);
			glVertex2i( 0, ty);
			glVertex2i(tx, ty);
			glVertex2i(tx,  0);
			glEnd();
			glPopMatrix();
		}
		if (lev.arr[row][col] == 'b') {
			glColor3f(0.9, 0.2, 0.2);
			glPushMatrix();
			glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
			glBegin(GL_QUADS);
			glVertex2i( 0,  0);
			glVertex2i( 0, ty);
			glVertex2i(tx, ty);
			glVertex2i(tx,  0);
			glEnd();
			glPopMatrix();
		}
		--row;
		}
		col = (col+1) % lev.ncols;
	}
	glColor3f(1.0, 1.0, 0.1);
	glPushMatrix();
	//put ball in its place
	glTranslated(gl.ball_pos[0], lev.tile_base+gl.ball_pos[1], 0);
	glBegin(GL_QUADS);
	glVertex2i(-10, 0);
	glVertex2i(-10, 20);
	glVertex2i( 10, 20);
	glVertex2i( 10, 0);
	glEnd();
	glPopMatrix();
	//--------------------------------------
	//
	//#define SHOW_FAKE_SHADOW
#ifdef SHOW_FAKE_SHADOW
	glColor3f(0.25, 0.25, 0.25);
	glBegin(GL_QUADS);
	glVertex2i(cx-60, 150);
	glVertex2i(cx+50, 150);
	glVertex2i(cx+50, 130);
	glVertex2i(cx-60, 130);
	glEnd();
#endif
	//
	//
	float h = 100.0;		// changed size of character 200.0 is original size
	float w = h * 0.4;		// 0.5 is the original size
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = gl.walkFrame % 8;
	int iy = 0;
	if (gl.walkFrame >= 8)
		iy = 1;
	float fx = (float)ix / 8.0;
	float fy = (float)iy / 2.0;
	glBegin(GL_QUADS);
	if (gl.keys[XK_Left]) {
		glTexCoord2f(fx+.125, fy+.5); glVertex2i(cx-w, cy-h);
		glTexCoord2f(fx+.125, fy);    glVertex2i(cx-w, cy+h);
		glTexCoord2f(fx,      fy);    glVertex2i(cx+w, cy+h);
		glTexCoord2f(fx,      fy+.5); glVertex2i(cx+w, cy-h);
	} else {
		glTexCoord2f(fx,      fy+.5); glVertex2i(cx-w, cy-h);
		glTexCoord2f(fx,      fy);    glVertex2i(cx-w, cy+h);
		glTexCoord2f(fx+.125, fy);    glVertex2i(cx+w, cy+h);
		glTexCoord2f(fx+.125, fy+.5); glVertex2i(cx+w, cy-h);
	}
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	//
	//
	if (gl.exp.onoff) {
		h = 80.0;
		w = 80.0;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glTranslated(gl.exp.pos[0], gl.exp.pos[1], gl.exp.pos[2]);
		int ix = gl.exp.frame % 5;
		int iy = gl.exp.frame / 5;
		float tx = (float)ix / 5.0;
		float ty = (float)iy / 5.0;
		glBegin(GL_QUADS);
		glTexCoord2f(tx,     ty+0.2); glVertex2i(cx-w, cy-h);
		glTexCoord2f(tx,     ty);     glVertex2i(cx-w, cy+h);
		glTexCoord2f(tx+0.2, ty);     glVertex2i(cx+w, cy+h);
		glTexCoord2f(tx+0.2, ty+0.2); glVertex2i(cx+w, cy-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}
	//
	//
	if (gl.exp44.onoff) {
		h = 80.0;
		w = 80.0;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glTranslated(gl.exp44.pos[0], gl.exp44.pos[1], gl.exp44.pos[2]);
		int ix = gl.exp44.frame % 4;
		int iy = gl.exp44.frame / 4;
		float tx = (float)ix / 4.0;
		float ty = (float)iy / 4.0;
		glBegin(GL_QUADS);
		glTexCoord2f(tx,      ty+0.25); glVertex2i(cx-w, cy-h);
		glTexCoord2f(tx,      ty);      glVertex2i(cx-w, cy+h);
		glTexCoord2f(tx+0.25, ty);      glVertex2i(cx+w, cy+h);
		glTexCoord2f(tx+0.25, ty+0.25); glVertex2i(cx+w, cy-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}

	//show enemy
	extern void showEnemy1(int x, int y, GLuint Texid);
	showEnemy1(700, 240, gl.enemy1Texture);

	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ffff44, "H    	Help/Info");
	ggprint8b(&r, 16, 0x00ffff44, "O    	Settings");
	ggprint8b(&r, 16, 0x00ffff44, "E	Exit");
	if (gl.movie) {
		screenCapture();
	}
	#ifndef MENU_ANAHI
    }
	#endif
}





