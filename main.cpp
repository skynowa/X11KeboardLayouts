/**
 * \file  main.cpp
 * \brief
 */


#include "Widget.h"
#include "LayoutCode.h"
#include <QSocketNotifier>
#include <memory>
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
    TraceLog()
        << "\n"
        << "---------------------" << appTitle << "--------------------" << "\n"
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
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

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

    std::unique_ptr<Widget> widget;
    int observedGroup {-1};
    QSocketNotifier notifier(ConnectionNumber(display), QSocketNotifier::Read);
    const auto onX11Ready = [&]() -> void
    {
        while (::XPending(display) > 0) {
            XEvent event {};
            ::XNextEvent(display, &event);

            if (event.type != xkbEventType) {
                continue;
            }

            const auto *xkbEvent = reinterpret_cast<const XkbEvent *>(&event);
            if (xkbEvent->any.xkb_type != XkbStateNotify) {
                continue;
            }

            if (!xkbShouldShowLayoutChange(xkbEvent->state, &observedGroup)) {
                continue;
            }

            widget = std::make_unique<Widget>(xkbLayoutCode(display, xkbEvent->state.group));
            widget->show();

            const int intervalMs = 700;
            QTimer::singleShot(intervalMs, widget.get(), &QWidget::hide);
        }
    };
    QObject::connect(&notifier, &QSocketNotifier::activated, &app, onX11Ready);
    onX11Ready();

    const int result = app.exec();

    widget.reset();
    ::XCloseDisplay(display);

    return result;
}
//-------------------------------------------------------------------------------------------------
