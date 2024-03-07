/**
 * \file  main.cpp
 * \brief
 */

#include "widget.h"

#include <QApplication>
#include <QTimer>
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Constants
    const int interval = 800;


    QApplication app(argc, argv);

    const QStringList arguments = QCoreApplication::arguments();
    if (arguments.size() <= 1) {
        qDebug() << "Bad arguments: " << arguments;
        return EXIT_FAILURE;
    }

    const QString paramLangCode = arguments.at(1);
    qDebug() << "paramLangCode:" << paramLangCode;

    Widget widget(paramLangCode);
    widget.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint |
        Qt::WindowDoesNotAcceptFocus);
    widget.show();

    QTimer::singleShot(interval, &app, &QCoreApplication::quit);

    return app.exec();
}
//-------------------------------------------------------------------------------------------------
