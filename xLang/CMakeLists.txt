#--------------------------------------------------------------------------------------------------
# \file  [PROJECT_NAME].cmake
# \brief
#
# sudo apt-get install libxcursor-dev
#--------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.0)

project("xLang" CXX)
#--------------------------------------------------------------------------------------------------
# options
set(PROJECT_TYPE                CXX)
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_COLOR_MAKEFILE        ON)
set(CMAKE_VERBOSE_MAKEFILE      OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_QUIET        ON)
#--------------------------------------------------------------------------------------------------
# Find X11 package
find_package(X11 REQUIRED)
# find_package(Xcursor REQUIRED)


# Add the source file
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})

# Set compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -O2)

# Link necessary libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES} Xcursor Xpm png)
#--------------------------------------------------------------------------------------------------
