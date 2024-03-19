/**
 * \file  main.cpp
 * \brief
 */


#include "Widget.h"
//-------------------------------------------------------------------------------------------------
const QString appTitle("[qLang]");
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

   /**
    * \example
    *
    * QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning().noquote()
    */
    qDebug().noquote()
        << "\n"
        << "--------------------" << appTitle << "-------------------" << "\n"
        << " Type:        " << errorEvent->type                << "\n"
        << " Display:     " << errorEvent->display             << "\n"
        << " Resource ID: " << errorEvent->resourceid          << "\n"
        << " Serial:      " << errorEvent->serial              << "\n"
        << " Error code:  " << errorEvent->error_code          << "\n"
        << " Request code:" << errorEvent->request_code        << "\n"
        << " Minor code:  " << errorEvent->minor_code          << "\n"
        << " Msg:         " << errorText                       << "\n"
        << "--------------------------------------------------";

    // Return 0 - indicate that the error has been handled
    return 0;
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    Display *display = ::XOpenDisplay(nullptr);
    STD_TEST_PTR(display);

    if (1) {
        ::XSetErrorHandler(::customErrorHandler);
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
    ::XSelectInput(display, rootWindow, ButtonPressMask);
    ::XSync(display, False);

    for ( ;; ) {
        qDebug() << "";
        qDebug().noquote() << appTitle << "XNextEvent - watch...";

        XEvent event {};
        ::XNextEvent(display, &event);

        qDebug().noquote() << appTitle << "XNextEvent - fire:" << STD_TRACE_VAR(event.type);

        if (event.type == xkbEventType) {
            auto *xkbEvent = (XkbEvent *)&event;
            // qDebug() << appTitle << STD_TRACE_VAR(xkbEvent->any.xkb_type);

            if (xkbEvent->any.xkb_type == XkbStateNotify) {
                const int langId = xkbEvent->state.group;

                // Constants
                const int intervalMs = 700;

                // App
                QApplication app(argc, argv);

                Widget widget(langId);
                widget.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint |
                    Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
                widget.show();

                QTimer::singleShot(intervalMs, &app, &QApplication::quit);

                app.exec();
            }
        }
    }

    ::XCloseDisplay(display);

    return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
