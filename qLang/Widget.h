/**
 * \file  widget.h
 * \brief
 */


#pragma once

#include "./ui_Widget.h"

#include <QWidget>
#include <QCursor>
//-------------------------------------------------------------------------------------------------
class Widget :
    public QWidget
{
public:
    explicit Widget(const QString &langCode);
    ~Widget() = default;

private:
    Q_OBJECT

    Ui::Widget ui;

    const QString _langCode;

    void setPixmap();
    void alignToCursor();
};
//-------------------------------------------------------------------------------------------------
