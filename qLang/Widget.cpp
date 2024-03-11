/**
 * \file  widget.cpp
 * \brief
 */


#include "Widget.h"
//-------------------------------------------------------------------------------------------------
Widget::Widget(
    const QString &a_langCode
) :
    QWidget  (nullptr),
    _langCode(a_langCode)
{
    ui.setupUi(this);

    setPixmap();
    alignToCursor();
}
//-------------------------------------------------------------------------------------------------
void
Widget::setPixmap()
{
    // https://flagicons.lipis.dev
    QPixmap pixmap("/home/skynowa/Projects/X11KeboardLayouts/qLang/Data/" + _langCode + ".svg");

    ui.label->setPixmap(pixmap);
}
//-------------------------------------------------------------------------------------------------
void
Widget::alignToCursor()
{
    const QPoint pos = QCursor::pos();

    setGeometry(pos.x() + width() - 2, pos.y() - height() - 2, width(), height());
}
//-------------------------------------------------------------------------------------------------
