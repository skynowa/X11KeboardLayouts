/**
 * \file  widget.cpp
 * \brief
 */


#include "Widget.h"
//-------------------------------------------------------------------------------------------------
Widget::Widget(
    const QString &a_layoutCode
) :
    QWidget  (nullptr),
    _layoutCode(a_layoutCode)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint |
        Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus |
        Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_X11DoNotAcceptFocus);

    _setPixmap();
    _alignToCursor();
}
//-------------------------------------------------------------------------------------------------
void
Widget::_setPixmap()
{
    // https://flagicons.lipis.dev
    const QString iconCode = _layoutCode == QStringLiteral("us") ? QStringLiteral("en") : _layoutCode;
    const QString iconPath = QStringLiteral(":/icons/") + iconCode + QStringLiteral(".svg");
    const QPixmap pixmap(iconPath);
    if (pixmap.isNull()) {
        ui.label->setText(_layoutCode.toUpper());
        ui.label->setAlignment(Qt::AlignCenter);
        return;
    }

    ui.label->setPixmap(pixmap);
}
//-------------------------------------------------------------------------------------------------
void
Widget::_alignToCursor()
{
    const QPoint pos = QCursor::pos();

    setGeometry(pos.x() + width() - 2, pos.y() - height() - 2, width(), height());
}
//-------------------------------------------------------------------------------------------------
