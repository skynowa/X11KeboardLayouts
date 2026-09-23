#pragma once

#include <QByteArray>
#include <QString>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

QString xkbLayoutCodeFromProperty(const QByteArray &property, unsigned int group);
QString xkbLayoutCode(Display *display, unsigned int group);
bool xkbShouldShowLayoutChange(const XkbStateNotifyEvent &event, int *observedGroup);
