/**
 * \file  main.cpp
 * \brief
 *
 * \see
 * - https://stackoverflow.com/questions/35569562/how-to-catch-keyboard-layout-change-event-and-get-current-new-keyboard-layout-on
 * - https://www-h.eng.cam.ac.uk/help/tpl/graphics/X/X11R5/node20.html
 * - https://github.com/skynowa/find-cursor/blob/master/find-cursor.c
 * - https://github.com/Cougar/pwm/blob/master/cursor.c
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
const std::string appTitle = "[xLang] ";
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
        << "---------------------- xLang ---------------------" << "\n"
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
	Display *display = ::XOpenDisplay(nullptr);
	if (display == nullptr) {
		std::cerr << appTitle << "cannot open display" << std::endl;
		return EXIT_FAILURE;
	}

	if (1) {
		::XSetErrorHandler(customErrorHandler);
	}

   /**
	* Note: We might never get a MappingNotify event if the
	* modifier and keymap information was never cached in Xlib.
	* The next line makes sure that this happens initially.
	*/
    ::XKeysymToKeycode(display, XK_F1);


	int xkbEventType {};
	::XkbQueryExtension(display, 0, &xkbEventType, 0, 0, 0);

	::XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask,
		XkbGroupStateMask);

    Window rootWindow = DefaultRootWindow(display);
    XSelectInput(display, rootWindow, ButtonPressMask);

	::XSync(display, False);

	for ( ;; ) {
		std::cout << std::endl;
		std::cout << appTitle << "XNextEvent - watch..." << std::endl;

		XEvent event {};
		::XNextEvent(display, &event);

		std::cout << appTitle << "XNextEvent - fire" << std::endl;

		std::cout << appTitle << STD_TRACE_VAR(event.type) << std::endl;

		if (event.type == xkbEventType) {
			auto *xkbEvent = (XkbEvent *)&event;
			std::cout << appTitle << STD_TRACE_VAR(xkbEvent->any.xkb_type) << std::endl;

			if (xkbEvent->any.xkb_type == XkbStateNotify) {
				const int lang_id           = xkbEvent->state.group;
				const std::string lang_code = (lang_id == 0) ? "en" : "ru";

				// std::cout << appTitle << STD_TRACE_VAR(lang_id) << ", " <<  STD_TRACE_VAR(lang_code)  << std::endl;

				const std::string cmd = "/home/skynowa/Projects/X11KeboardLayouts/"
					"build-qLang-Desktop-Debug/qLang " +  lang_code;

				int iRv = std::system(cmd.c_str());
				STD_UNUSED(iRv);
			}
		}
	}

	::XCloseDisplay(display);

	return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
