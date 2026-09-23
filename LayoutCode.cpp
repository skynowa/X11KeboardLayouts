#include "LayoutCode.h"

#include <QList>
#include <X11/Xatom.h>

QString
xkbLayoutCodeFromProperty(const QByteArray &a_property, unsigned int a_group)
{
    const QString fallback = QStringLiteral("G%1").arg(a_group + 1);
    const QList<QByteArray> fields = a_property.split('\0');
    if (fields.size() < 3) {
        return fallback;
    }

    const QList<QByteArray> layouts = fields.at(2).split(',');
    if (a_group >= static_cast<unsigned int>(layouts.size())) {
        return fallback;
    }

    const QString code = QString::fromLatin1(layouts.at(a_group)).trimmed().toLower();
    return code.isEmpty() ? fallback : code;
}

QString
xkbLayoutCode(Display *a_display, unsigned int a_group)
{
    const QString fallback = QStringLiteral("G%1").arg(a_group + 1);
    const Atom namesAtom = ::XInternAtom(a_display, "_XKB_RULES_NAMES", True);
    if (namesAtom == None) {
        return fallback;
    }

    Atom actualType {};
    int actualFormat {};
    unsigned long itemCount {};
    unsigned long bytesAfter {};
    unsigned char *property {};
    const int status = ::XGetWindowProperty(a_display, DefaultRootWindow(a_display), namesAtom,
        0, 1024, False, XA_STRING, &actualType, &actualFormat, &itemCount, &bytesAfter, &property);
    if (status != Success || actualType != XA_STRING || actualFormat != 8 ||
        bytesAfter != 0 || property == nullptr)
    {
        if (property != nullptr) {
            ::XFree(property);
        }
        return fallback;
    }

    const QByteArray names(reinterpret_cast<const char *>(property), static_cast<qsizetype>(itemCount));
    ::XFree(property);
    return xkbLayoutCodeFromProperty(names, a_group);
}
