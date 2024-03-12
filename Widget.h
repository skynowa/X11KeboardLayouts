/**
 * \file  widget.h
 * \brief
 */


#pragma once

#include "./ui_Widget.h"
#include "Config.h"
//-------------------------------------------------------------------------------------------------
class Widget :
    public QWidget
{
public:
             Widget() = delete;
    explicit Widget(const int langId);
            ~Widget() = default;

private:
    Q_OBJECT

    Ui::Widget ui;

    const QString _langCode;

    QString _langIdToCode(const int langId) const;
    void    _setPixmap();
    void    _alignToCursor();
};
//-------------------------------------------------------------------------------------------------
