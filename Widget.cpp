/**
 * \file  widget.cpp
 * \brief
 */


#include "Widget.h"
//-------------------------------------------------------------------------------------------------
Widget::Widget(
    const int a_langId
) :
    QWidget  (nullptr),
    _langCode( _langIdToCode(a_langId) )
{
    ui.setupUi(this);

    _setPixmap();
    _alignToCursor();
}
//-------------------------------------------------------------------------------------------------
QString
Widget::_langIdToCode(
    const int a_langId
) const
{
    static const QMap<int, QString> codes
    {
        {0, "en"},
        {1, "ru"},
        {2, "ua"}
    };

    auto it = codes.find(a_langId);
    if (it == codes.cend()) {
        qDebug() << STD_TRACE_VAR(a_langId);
        return {};
    }

    return it.value();
}
//-------------------------------------------------------------------------------------------------
void
Widget::_setPixmap()
{
    // https://flagicons.lipis.dev
    const QString iconPath = ICONS_DIR "/" + _langCode + ".svg";

    QFileInfo checkFile(iconPath);
    STD_TEST(checkFile.exists());
    STD_TEST(checkFile.isFile());

    QPixmap pixmap(iconPath);

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
