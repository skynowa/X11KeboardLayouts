#--------------------------------------------------------------------------------------------------
# \file  CMakeLists.txt
# \brief
#--------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.5)

project(qLang CXX)
#--------------------------------------------------------------------------------------------------
# options
set(PROJECT_TYPE                CXX)
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_COLOR_MAKEFILE        ON)
set(CMAKE_VERBOSE_MAKEFILE      OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_QUIET        ON)
#--------------------------------------------------------------------------------------------------
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(DIR_HOME $ENV{HOME})

# Qt
set(CMAKE_PREFIX_PATH "${DIR_HOME}/Qt/6.6.1/gcc_64")

# modules
if (EXISTS "${CMAKE_ROOT}/Modules/CMakeLib")
    list(APPEND CMAKE_MODULE_PATH
        "${CMAKE_ROOT}/Modules/CMakeLib"
        "${CMAKE_SOURCE_DIR}")
else()
    list(APPEND CMAKE_MODULE_PATH
        "${DIR_HOME}/Projects/CMakeLib"
        "${CMAKE_SOURCE_DIR}")
endif()
#--------------------------------------------------------------------------------------------------
find_package(Qt6 REQUIRED)
find_package(X11 REQUIRED)
#--------------------------------------------------------------------------------------------------
set(PROJECT_SOURCES
    main.cpp
    Config.h
    Widget.h
    Widget.cpp
    Widget.ui)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${X11_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${X11_LIBRARIES})

qt_finalize_executable(${PROJECT_NAME})
#--------------------------------------------------------------------------------------------------
