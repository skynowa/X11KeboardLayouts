/**
 * \file  xmappingnotify.c
 * \brief
 *
 * \see   https://stackoverflow.com/questions/35569562/how-to-catch-keyboard-layout-change-event-and-get-current-new-keyboard-layout-on
 */


#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
//-------------------------------------------------------------------------------------------------
static const unsigned char xlib_spinning_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x7c, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00,
    0xfc, 0x3b, 0x00, 0x00, 0x7c, 0x38, 0x00, 0x00, 0x6c, 0x54, 0x00, 0x00,
    0xc4, 0xdc, 0x00, 0x00, 0xc0, 0x44, 0x00, 0x00, 0x80, 0x39, 0x00, 0x00,
    0x80, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char xlib_spinning_mask_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
    0x1e, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
    0xfe, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0xfe, 0x3b, 0x00, 0x00,
    0xfe, 0x7f, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00,
    0xee, 0xff, 0x01, 0x00, 0xe4, 0xff, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00,
    0xc0, 0x7f, 0x00, 0x00, 0x80, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//-------------------------------------------------------------------------------------------------
Cursor
cursorCreateSpinning (Display * dpy)
{
    XColor fg;
    fg.pixel = 0;
    fg.red = 0;
    fg.green = 0;
    fg.blue = 0;
    fg.flags = 0xf;

    XColor bg;
    bg.pixel = 0xffffffff;
    bg.red = 0xffff;
    bg.green = 0xffff;
    bg.blue = 0xffff;
    bg.flags = 0xf;

    Pixmap cursor = XCreatePixmapFromBitmapData(dpy, DefaultRootWindow(dpy),
                                          (char *) xlib_spinning_bits,
                                          32, 32, 0xffffffff, 0x0, 1);
    Pixmap mask = XCreatePixmapFromBitmapData(dpy, DefaultRootWindow(dpy),
                                          (char *) xlib_spinning_mask_bits,
                                          32, 32, 0xffffffff, 0x0, 1);

    Cursor xcursor = XCreatePixmapCursor(dpy, cursor, mask, &fg, &bg, 2, 2);

    XFreePixmap(dpy, mask);
    XFreePixmap(dpy, cursor);

    return xcursor;
}
//-------------------------------------------------------------------------------------------------
Cursor
cursorLoad(
	Display *display,
	Window  &root
)
{

	Cursor cursor = ::cursorCreateSpinning(display);

    // Set the cursor for the root window
    ::XDefineCursor(display, root, cursor);

    // Flush and wait
    ::XFlush(display);

    // Clean up
    /// ::XFreeCursor(display, cursor);

	return cursor;
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
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

	while (1) {
		XEvent event {};
		::XNextEvent(display, &event);

		Cursor cursor {};

		if (event.type == xkbEventType) {
			auto *xkbEvent = (XkbEvent *)&event;
			if (xkbEvent->any.xkb_type == XkbStateNotify) {
				// Get default root window of display
				Window rootWin = DefaultRootWindow(display);

				const int lang = xkbEvent->state.group;
				if (lang == 0) {
					std::cout << "En: " << lang << std::endl;

					if (cursor != 0) {
						::XFreeCursor(display, cursor);
					}
				} else {
					std::cout << "Ru: " << lang << std::endl;

					cursor = ::cursorLoad(display, rootWin);
				}

			   /**
				* Change cursor
				*
				* https://stackoverflow.com/questions/73833171/how-to-get-the-active-window-using-x11-xlib-c-api
				* https://github.com/UltimateHackingKeyboard/current-window-linux/blob/master/get-current-window.c
				*/

				// Get the atom of the property
				Atom       property = ::XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
				const Bool isDelete {False};
				// https://docs.rs/x11/0.0.36/x11/xlib/constant.XA_WINDOW.html
				const Atom XA_WINDOW {33};

				// Return values
				Atom           type_return {};
				int            format_return {};
				unsigned long  nitems_return {};
				unsigned long  bytes_left {};
				unsigned char *data {};

				int iRv = ::XGetWindowProperty(
					display,
					rootWin,
					property,
					0,              // no offset
					1,              // one Window
					isDelete,
					XA_WINDOW,
					&type_return,   // should be XA_WINDOW
					&format_return, // should be 32
					&nitems_return, // should be 1 (zero if there is no such window)
					&bytes_left,    // should be 0 (i'm not sure but should be atomic read)
					&data           // should be non-null
				);

				if (iRv != Success) {
					std::cout << "iRv: " << iRv << std::endl;
				}

				if (nitems_return != 1) {
					std::cout << "nitems_return: " << nitems_return << std::endl;
				}

				Window activeWin = ((Window *) data)[0];

				++ i;

				const auto x11_cursor_font_id = (i % 2 == 0) ? XC_arrow : XC_dotbox;

				Cursor cursor = ::XCreateFontCursor(display, x11_cursor_font_id);

				::XDefineCursor(display, activeWin, cursor);

				::XFreeCursor(display, cursor);
				::XFree(data);
			}
		}
	}

	XCloseDisplay(display);

	return 0;
}
//-------------------------------------------------------------------------------------------------


#if 0

// https://github.com/Cougar/pwm/blob/master/cursor.c
void set_cursor(Window win, int cursor)
{
	XDefineCursor(wglobal.dpy, win, cursors[cursor]);
}

#endif
