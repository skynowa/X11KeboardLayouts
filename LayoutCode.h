#pragma once

#include <QByteArray>
#include <QString>
#include <X11/Xlib.h>

QString xkbLayoutCodeFromProperty(const QByteArray &property, unsigned int group);
QString xkbLayoutCode(Display *display, unsigned int group);
