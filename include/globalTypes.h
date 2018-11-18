#ifndef _GLOBALTYPES_H_
#define _GLOBALTYPES_H_

/*** TYPES ***/
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];
typedef struct t_button {
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

/** CLASSES **/
class Menu {
private:
    Button *buttons;
    Mouse *mouse;
    void checkButtons();
public:
    int numButtons;
    bool active;
    Rect rect;
    Image backgroundImg;
    Menu();
    void render();
};

class Image {
    public:
        int width, height;
        unsigned char *data;
        ~Image();
        Image(const char *);
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

class X11_wrapper {
    private:
        Display *dpy;
        Window win;
    public:
        ~X11_wrapper();
        void setTitle();
        void setupScreenRes(const int, const int);
        X11_wrapper();
        void reshapeWindow(int, int);
        void checkResize(XEvent);
        bool getXPending();
        XEvent getXNextEvent();
        void swapBuffers();
};

#endif // _GLOBAL_TYPES_H_