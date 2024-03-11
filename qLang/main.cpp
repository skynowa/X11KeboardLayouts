/**
 * \file  main.cpp
 * \brief
 */


#include "Widget.h"
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
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
