/**
 * \file  main.cpp
 * \brief
 *
 * \see
 * - https://stackoverflow.com/questions/35569562/how-to-catch-keyboard-layout-change-event-and-get-current-new-keyboard-layout-on
 * - https://www-h.eng.cam.ac.uk/help/tpl/graphics/X/X11R5/node20.html
 * - https://github.com/skynowa/find-cursor/blob/master/find-cursor.c
 *
 * sudo apt-get install libxpm-dev
 */


#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <StdStream/StdStream.h>
#include <StdTest/StdTest.h>
//-------------------------------------------------------------------------------------------------
int
customErrorHandler(
	Display     *display,
	XErrorEvent *errorEvent
)
{
	STD_TEST_PTR(display);
	STD_TEST_PTR(errorEvent);

    char errorText[1024] {};
    ::XGetErrorText(display, errorEvent->error_code, errorText, sizeof(errorText));

    std::cerr
        << "\n"
        << "---------------------- ERROR ---------------------" << "\n"
        << " Type:         " << errorEvent->type                << "\n"
        << " Display:      " << errorEvent->display             << "\n"
        << " Resource ID:  " << errorEvent->resourceid          << "\n"
        << " Serial:       " << errorEvent->serial              << "\n"
        << " Error code:   " << errorEvent->error_code          << "\n"
        << " Request code: " << errorEvent->request_code        << "\n"
        << " Minor code:   " << errorEvent->minor_code          << "\n"
        << " Msg:          " << errorText                       << "\n"
        << "--------------------------------------------------" << std::endl;

    return 0; // Return 0 to indicate that the error has been handled
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	if (1) {
		::XSetErrorHandler(customErrorHandler);
	}

	Display *display = ::XOpenDisplay(NULL);
	if (display == nullptr) {
		std::cerr << "cannot open display" << std::endl;
		return EXIT_FAILURE;
	}

	::XKeysymToKeycode(display, XK_F1);

	int xkbEventType {};
	::XkbQueryExtension(display, 0, &xkbEventType, 0, 0, 0);

	::XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask,
		XkbGroupStateMask);

	::XSync(display, False);

	int i {};

	while (true) {
		std::cout << std::endl;
		std::cout << "XNextEvent - watch..." << std::endl;

		XEvent event {};
		::XNextEvent(display, &event);

		std::cout << "XNextEvent - fire" << std::endl;

		if (event.type == xkbEventType) {
			auto *xkbEvent = (XkbEvent *)&event;
			if (xkbEvent->any.xkb_type == XkbStateNotify) {
				const int lang = xkbEvent->state.group;
				std::cout << "En: " << lang << " - start" << std::endl;

				std::string params;
				if (lang == 0) {
					params = "en";
				} else {
					params = "ru";
				}

				const std::string cmd = "/home/skynowa/Projects/XMmappingNotify/"
					"build-qLang-Desktop-Debug/qLang " +  params;

				int iRv = std::system(cmd.c_str());
				STD_UNUSED(iRv);

				std::cout << "Ru: " << lang << " - end" << std::endl;
			}
		}
	}

	::XCloseDisplay(display);

	return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------


#if 0

// https://github.com/Cougar/pwm/blob/master/cursor.c
void set_cursor(Window win, int cursor)
{
	XDefineCursor(wglobal.dpy, win, cursors[cursor]);
}

#endif
