/**
 * \file  Config.h
 * \brief config
 */


#pragma once
//-------------------------------------------------------------------------------------------------
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QCursor>
#include <QFileInfo>

#include <X11/XKBlib.h>
#include <X11/Xutil.h>

#include <StdStream/StdStream.h>
#include <StdTest/StdTest.h>
#include <StdLog/StdLog.h>
//-------------------------------------------------------------------------------------------------
#define ICONS_DIR "/home/skynowa/Projects/X11KeboardLayouts/Data"
//-------------------------------------------------------------------------------------------------
class TraceLog final :
    public stdlog::Log
{
public:
    TraceLog() :
        Log("Core", Log::Level::Trace, " ")
    {
    }
};
//-------------------------------------------------------------------------------------------------
