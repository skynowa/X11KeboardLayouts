/**
 * \file  main.cpp
 * \brief
 */


#include "Widget.h"
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
int main(int argc, char *argv[])
{
    if (0) {
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
                // std::cout << appTitle << STD_TRACE_VAR(xkbEvent->any.xkb_type) << std::endl;

                if (xkbEvent->any.xkb_type == XkbStateNotify) {
                    const int lang_id           = xkbEvent->state.group;
                    const std::string lang_code = (lang_id == 0) ? "en" : "ru";

                    std::cout << appTitle << STD_TRACE_VAR(lang_id) << ", " <<  STD_TRACE_VAR(lang_code)  << std::endl;

                    const std::string cmd = "/home/skynowa/Projects/X11KeboardLayouts/qLang/qLang " +
                        lang_code;

                    int iRv = std::system(cmd.c_str());
                    STD_UNUSED(iRv);
                }
            }
        }

        ::XCloseDisplay(display);

        return EXIT_SUCCESS;
    }

    // Constants
    const QString appTitle = "[qLang]";
    const int     interval = 800;

    // App
    QApplication app(argc, argv);

    const QStringList arguments = QCoreApplication::arguments();
    if (arguments.size() <= 1) {
        qDebug() << appTitle << "Bad arguments: " << arguments;
        return EXIT_FAILURE;
    }

    const QString paramLangCode = arguments.at(1);
    qDebug() << appTitle << "paramLangCode:" << paramLangCode;

    Widget widget(paramLangCode);
    widget.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint |
        Qt::WindowDoesNotAcceptFocus);
    widget.show();

    QTimer::singleShot(interval, &app, &QCoreApplication::quit);

    return app.exec();
}
//-------------------------------------------------------------------------------------------------
