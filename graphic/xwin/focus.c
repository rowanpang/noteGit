#include<X11/Xlib.h>
#include<stdio.h>
#include<stdlib.h>

#define IR_MODE 8192
#define SHIFT_MODE 16
#define US_MODE 0

static int ErrorHandler (Display *display, XErrorEvent *error)
{
	printf("An error ocurred. We don't know anything.\n");
	return 0;
}    

int main()
{
	int rev;
	Window current_window;
	Display *d = XOpenDisplay(0);
	if(d == 0){
		printf("Cannot open display\n");
		exit(0);
	}

	Window root = DefaultRootWindow(d);
	XSetErrorHandler(ErrorHandler);

	XGetInputFocus(d,&current_window,&rev);
	XSelectInput(d,current_window, ButtonPressMask | FocusChangeMask);

	XEvent e;
	while(1) 
	{
		XNextEvent(d, &e);
		switch(e.type) {
			case FocusOut:
				printf("FocusOut\n");
				XGetInputFocus(d,&current_window,&rev);
				if(current_window == PointerRoot || current_window == None) {
					current_window = root;
					printf("root win\n");
				}
				/*XSelectInput(d,current_window,ButtonPressMask|KeyPressMask|KeyReleaseMask|FocusChangeMask);*/
				break;
			case KeyPress:                    
				printf("key pressed\n");
				break;
			case KeyRelease:
				printf("key relessed\n");
				break;
			
			case ButtonPress:/*Only one client at a time can select a ButtonPress event, which is associated with the event mask ButtonPressMask.*/
				printf("ButtonPress\n");
				XCloseDisplay(d);
				return 0;
		}
	}    
	XCloseDisplay(d);//*/
}
