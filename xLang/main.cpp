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
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xcursor/Xcursor.h>

#include <X11/xpm.h>

#include <png.h>

#include <StdStream/StdStream.h>
#include <StdTest/StdTest.h>
//-------------------------------------------------------------------------------------------------
XImage* loadImage(const char* filename, Display* display, Visual* visual) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return nullptr;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Error creating PNG read struct" << std::endl;
        fclose(file);
        return nullptr;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        std::cerr << "Error creating PNG info struct" << std::endl;
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(file);
        return nullptr;
    }

    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "Error during PNG read" << std::endl;
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(file);
        return nullptr;
    }

    png_init_io(png, file);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    // Ensure PNG is in RGB or RGBA format
#if 1
	if (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA) {
		std::cerr << "Unsupported color type in PNG" << std::endl;
		png_destroy_read_struct(&png, &info, nullptr);
		fclose(file);
		return nullptr;
	}
#endif

    // Set up XImage
    XImage* ximage = XCreateImage(display, visual, DefaultDepth(display, DefaultScreen(display)), ZPixmap, 0, nullptr, width, height, 32, 0);

    if (!ximage) {
        std::cerr << "Error creating XImage" << std::endl;
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(file);
        return nullptr;
    }

    // Allocate memory for pixel data
    ximage->data = new char[width * height * 4];

    png_bytep* row_pointers = new png_bytep[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)(ximage->data) + (y * ximage->bytes_per_line);
    }

    png_read_image(png, row_pointers);

    delete[] row_pointers;

    fclose(file);
    png_destroy_read_struct(&png, &info, nullptr);

    return ximage;
}
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
Cursor
cursorCreate1(
	Display *display
)
{
	STD_TRACE_FUNC

	STD_TEST_PTR(display);

	static const unsigned char xlib_spinning_bits[] =
	{
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

	static const unsigned char xlib_spinning_mask_bits[] =
	{
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

    XColor fg {};
    fg.pixel = 0;
    fg.red   = 0;
    fg.green = 0;
    fg.blue  = 0;
    fg.flags = 0xf;

    XColor bg {};
    bg.pixel = 0xffffffff;
    bg.red   = 0xffff;
    bg.green = 0xffff;
    bg.blue  = 0xffff;
    bg.flags = 0xf;

	Pixmap cursor = ::XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
		(char *)xlib_spinning_bits, 32, 32, 0xffffffff, 0x0, 1);
	Pixmap mask = ::XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
		(char *)xlib_spinning_mask_bits, 32, 32, 0xffffffff, 0x0, 1);

    Cursor xcursor = ::XCreatePixmapCursor(display, cursor, mask, &fg, &bg, 2, 2);

    ::XFreePixmap(display, mask);
    ::XFreePixmap(display, cursor);

    return xcursor;
}
//-------------------------------------------------------------------------------------------------
Cursor
cursorCreate2(
	Display    *display,
	Window     &root,
	const char *xpmFilePath
)
{
	STD_TRACE_FUNC

	STD_TEST_PTR(display);
	STD_TEST(root > 0);
	STD_TEST_PTR(xpmFilePath);

	std::cout << STD_TRACE_VAR(display) << std::endl;
	std::cout << STD_TRACE_VAR(root) << std::endl;
	std::cout << STD_TRACE_VAR(xpmFilePath) << std::endl;

    Pixmap cursor_pixmap {};
    Pixmap mask_pixmap {};

    // Load XPM file
    errno = 0;

    Status status = ::XpmReadFileToPixmap(display, root, xpmFilePath, &cursor_pixmap, &mask_pixmap,
        nullptr);

    STD_TRACE_POINT

    STD_TEST(status == XpmSuccess);
    STD_TEST(cursor_pixmap != None);
    STD_TEST(mask_pixmap != None);

    // Define colors and hot spot coordinates
    XColor fg {};
	XColor bg {};

    unsigned int x_hot = 2;
    unsigned int y_hot = 2;

    // Create the cursor
    Cursor cursor = ::XCreatePixmapCursor(display, cursor_pixmap, mask_pixmap, &fg, &bg, x_hot, y_hot);
    STD_TEST(cursor != None);

    STD_TRACE_POINT

    // Clean up
    // ::XFreePixmap(display, mask_pixmap);
    // ::XFreePixmap(display, cursor_pixmap);

	return cursor;
}
//-------------------------------------------------------------------------------------------------
/**
 * Change cursor
 *
 * https://stackoverflow.com/questions/73833171/how-to-get-the-active-window-using-x11-xlib-c-api
 * https://github.com/UltimateHackingKeyboard/current-window-linux/blob/master/get-current-window.c
 */
void
cursorLoad(
	Display           *display,
	Window            &root,
	const std::string &cursorFilePath
)
{
	int iRv {None};

	STD_TRACE_POINT

	Cursor cursor = ::cursorCreate1(display);
	/// Cursor cursor = ::cursorCreate2(display, root, cursorFilePath.c_str());
	STD_TEST(cursor != None);

	STD_TRACE_POINT

    // Set the cursor for the root window
	{
	    iRv = ::XDefineCursor(display, root, cursor);
		STD_TEST(iRv != None);

		iRv = ::XFlush(display);
		STD_TEST(iRv != None);
	}

    std::cout << "Sleep ..." << std::endl;
    ::sleep(1);

	{
	    iRv = ::XUndefineCursor(display, root);
		STD_TEST(iRv != None);

		iRv = ::XFlush(display);
		STD_TEST(iRv != None);

		// Clean up
		::XFreeCursor(display, cursor);
	}
}
//-------------------------------------------------------------------------------------------------
Window
activeWindow(
	Display *display,
	Window  &rootWin
)
{
	// Get the atom of the property
	const Atom property = ::XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
	const Bool isDelete {False};
	// https://docs.rs/x11/0.0.36/x11/xlib/constant.XA_WINDOW.html
	const Atom xaWindow {XA_WINDOW};

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
		xaWindow,
		&type_return,   // should be XA_WINDOW
		&format_return, // should be 32
		&nitems_return, // should be 1 (zero if there is no such window)
		&bytes_left,    // should be 0 (i'm not sure but should be atomic read)
		&data           // should be non-null
	);

	if (iRv != Success) {
		std::cout << "iRv: " << iRv << std::endl;
		return {};
	}

	if (nitems_return != 1) {
		std::cout << "nitems_return: " << nitems_return << std::endl;
		return {};
	}

	const Window activeWin = ((Window *)data)[0];

	::XFree(data);

	return activeWin;
}
//-------------------------------------------------------------------------------------------------
void
pictureShow1(
	Display *display,
	Window  &root
)
{
	Visual* visual = DefaultVisual(display, DefaultScreen(display));
	int screen = DefaultScreen(display);

    // Get the cursor position
    int rootX, rootY, win_x_return, win_y_return;
    unsigned int mask;
    Window child;

    ::XQueryPointer(display, root, &root, &child, &rootX, &rootY, &win_x_return, &win_y_return, &mask);

    // Load the image
    XImage *image = ::loadImage("/home/skynowa/Projects/XMmappingNotify/Data/uk.png", display, visual);
    if (image == NULL) {
        std::cerr << "Failed to load image" << std::endl;
        return;
    }

    // Create a window to display the image
#if 0
	Window window = ::XCreateSimpleWindow(display, root, rootX, rootY, image->width, image->height, 0, 0, 0);
#else
	// Specify window attributes
	XSetWindowAttributes attributes;
	attributes.background_pixel = WhitePixel(display, screen);
	attributes.border_pixel = BlackPixel(display, screen);
	attributes.event_mask = ExposureMask | KeyPressMask;

	// Create the window
	Window window = XCreateWindow(display, root, rootX, rootY, image->width, image->height, 0,
								   DefaultDepth(display, screen), InputOutput,
								   DefaultVisual(display, screen),
								   CWBackPixel | CWBorderPixel | CWEventMask,
								   &attributes);

	// XSelectInput(display, window, ExposureMask | KeyPressMask);
#endif

    // Map the window
    ::XMapWindow(display, window);

//    Pixmap pixmap = ::XCreatePixmapFromBitmapData(display, root, image->data, image->width, image->height, 0xFFFFFFFF, 0xFFFFFFFF, DefaultDepth(display, 0));
    Pixmap pixmap = XCreatePixmap(display, root, image->width, image->height, DefaultDepth(display, DefaultScreen(display)));


    // Set the window background to the loaded image
    ::XSetWindowBackgroundPixmap(display, window, pixmap);

    // Flush the display
    ::XFlush(display);

    sleep(2);

    XDestroyWindow(display, window);
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	const std::string cursor_en = "/home/skynowa/Projects/XMmappingNotify/Data/cc.xpm";
	const std::string cursor_ru = "/home/skynowa/Projects/XMmappingNotify/Data/cc.xpm";

	Display *display = ::XOpenDisplay(NULL);
	if (display == nullptr) {
		std::cerr << "cannot open display" << std::endl;
		return EXIT_FAILURE;
	}

	// Get default root window of display
	Window rootWin = DefaultRootWindow(display);

	if (1) {
		::XSetErrorHandler(customErrorHandler);
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
					// ::cursorLoad(display, rootWin, cursor_en);
					params = "en";
				} else {
					// ::cursorLoad(display, rootWin, cursor_ru);
					params = "ru";
				}

				/// pictureShow1(display, rootWin);

				const std::string cmd = "/home/skynowa/Projects/build-qLang-Desktop-Debug/qLang " +  params;

				std::system(cmd.c_str());

				std::cout << "Ru: " << lang << " - end" << std::endl;

				///
				continue;

				if (0) {
					Window activeWin = ::activeWindow(display, rootWin);

					++ i;

					// https://tronche.com/gui/x/xlib/appendix/b/
					const auto x11_cursor_font_id = (i % 2 == 0) ? XC_arrow : XC_dotbox;

					Cursor cursor = ::XCreateFontCursor(display, x11_cursor_font_id);

					::XDefineCursor(display, activeWin, cursor);

					::XFreeCursor(display, cursor);
				}
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
