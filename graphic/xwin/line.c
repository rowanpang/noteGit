/* first include the standard headers that we're likely to need */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){
	int screen_num, width, height;
	unsigned long background, border;
	Window win;
	XEvent ev;
	Display *dpy;
	GC pen;
	XGCValues values;

										/* First connect to the display server */
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		fprintf(stderr, "unable to connect to display\n");
		return 7;
	}

	/* these are macros that pull useful data out of the display object */
	/* we use these bits of info enough to want them in their own variables */
	screen_num = DefaultScreen(dpy);
	background = BlackPixel(dpy, screen_num);
	border = WhitePixel(dpy, screen_num);

	width = 40; /* start with a small window */
	height = 40;

	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), /* display, parent */
		0,0, /* x, y: the window manager will place the window elsewhere */
		width, height, /* width, height */
		2, border, /* border width & colour, unless you have a window manager */
		background); /* background colour */

	/* create the pen to draw lines with */
	values.foreground = WhitePixel(dpy, screen_num);
	values.line_width = 2;
	values.line_style = LineSolid;
	pen = XCreateGC(dpy, win, GCForeground|GCLineWidth|GCLineStyle,&values);


	/* tell the display server what kind of events we would like to see */
	XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask|ExposureMask );

	/* okay, put the window on the screen, please */
	XMapWindow(dpy, win);

	/* as each event that we asked about occurs, we respond.  In this *	 
	* case we note if the window's shape changed, and exit if a button *		
	* is pressed inside the window.  We also draw lines whenever a part *
	* of a window becomes viewable. */
	while(1){
		XNextEvent(dpy, &ev);
		switch(ev.type){
			case Expose:
				printf("Expose  to: %d by %d\n", width, height);
				XDrawLine(dpy, win, pen, 0, 0, width, height);
				XDrawLine(dpy, win, pen, width, 0, 0, height);
				XDrawLine(dpy, win, pen, width/2, 0, 0, height/2);
				XDrawLine(dpy, win, pen, width/2, 0, width/2, height);
				break;
			case ConfigureNotify:
				if (width != ev.xconfigure.width
				|| height != ev.xconfigure.height) {
					width = ev.xconfigure.width;
					height = ev.xconfigure.height;
					XClearWindow(dpy, ev.xany.window);
					printf("Size changed to: %d by %d\n", width, height);
				}
				break;
			case ButtonPress:
				XCloseDisplay(dpy);
				return 0;
		}
	}
}
