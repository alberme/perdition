#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>

#include "globalTypes.h"
#include "log.h"

X11_wrapper::X11_wrapper(GLint attributes[], XResolution res, 
    long eventMask, const char title[], void (*onResize)(XResolution))
{
    this->res = res;
    GLint *att = attributes;
    onWindowResize = onResize;
    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
    XSetWindowAttributes swa;
    setupScreenRes(res.xres, res.yres);
    dpy = XOpenDisplay(nullptr);

    if (dpy == nullptr) {
        printf("\n\tcannot connect to X server\n\n");
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == nullptr) {
        printf("\n\tno appropriate visual found\n\n");
        exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = eventMask;
    /* ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask | PointerMotionMask | 
        StructureNotifyMask | SubstructureNotifyMask;*/
    win = XCreateWindow(dpy, root, 0, 0, res.xres, res.yres, 0,
        vi->depth, InputOutput, vi->visual,
        CWColormap | CWEventMask, &swa);
    GLXContext glc = glXCreateContext(dpy, vi, nullptr, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    setTitle(title);
}

X11_wrapper::~X11_wrapper()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void X11_wrapper::setTitle(const char name[])
{
    XMapWindow(dpy, win);
    XStoreName(dpy, win, name);
    strcpy(windowTitle, name);
}

void X11_wrapper::setupScreenRes(const int w, const int h)
{
    this->res.xres = w;
    this->res.yres = h;
}

void X11_wrapper::reshapeWindow(const int width, const int height)
{
    //window has been resized.
    setupScreenRes(width, height);
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, this->res.xres, 0, this->res.yres, -1, 1);
    setTitle(windowTitle);    
}

void X11_wrapper::checkResize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
        return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != this->res.xres || xce.height != this->res.yres) {
        //Window size did change.
        reshapeWindow(xce.width, xce.height);
        try {
            if (onWindowResize) {
                onWindowResize(res);
            }
        } catch (const std::exception& e) {
            Log("Error: X11_wrapper::onWindowResize - \
                Exception caught in event handler:\n\t%s", e.what());
            throw e;
        }
    }
}

bool X11_wrapper::getXPending()
{
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers()
{
    glXSwapBuffers(dpy, win);
}