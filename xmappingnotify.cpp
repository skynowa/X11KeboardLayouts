/**
 * \file  xmappingnotify.c
 * \brief
 *
 * \see   https://stackoverflow.com/questions/35569562/how-to-catch-keyboard-layout-change-event-and-get-current-new-keyboard-layout-on
 */


#include <stdio.h>
#include <iostream>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	XEvent   e;
	Display *d {};

	if (!(d = XOpenDisplay(NULL))) {
		fprintf(stderr, "cannot open display\n");
		return 1;
	}

	XKeysymToKeycode(d, XK_F1);

    int xkbEventType {};
    XkbQueryExtension(d, 0, &xkbEventType, 0, 0, 0);
    XkbSelectEventDetails(d, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);

    XSync(d, False);

    while (1) {
        XNextEvent(d, &e);

        if (e.type == xkbEventType) {
            XkbEvent* xkbEvent = (XkbEvent*) &e;
            if (xkbEvent->any.xkb_type == XkbStateNotify) {
                int lang = xkbEvent->state.group;
                if (lang == 1) {
                    std::cout << lang << std::endl;
                } else {
                	std::cout << lang << std::endl;
                }
            }
        }
    }

    return 0;
}
//-------------------------------------------------------------------------------------------------
