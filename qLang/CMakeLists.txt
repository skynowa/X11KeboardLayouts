#--------------------------------------------------------------------------------------------------
# \file  CMakeLists.txt
# \brief
#--------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.5)

project(qLang VERSION 0.1 LANGUAGES CXX)
#--------------------------------------------------------------------------------------------------
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
find_package(Qt6)
#--------------------------------------------------------------------------------------------------
set(PROJECT_SOURCES
    main.cpp
    Widget.cpp
    Widget.h
    Widget.ui)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets)

qt_finalize_executable(${PROJECT_NAME})
#--------------------------------------------------------------------------------------------------
