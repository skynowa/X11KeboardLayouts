#--------------------------------------------------------------------------------------------------
# \file  [PROJECT_NAME].cmake
# \brief
#--------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.0)

project("xmappingnotify")
#--------------------------------------------------------------------------------------------------
# options
set(PROJECT_TYPE                CXX)
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_COLOR_MAKEFILE        ON)
set(CMAKE_VERBOSE_MAKEFILE      OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_QUIET        ON)
#--------------------------------------------------------------------------------------------------
# Add the source file
add_executable(xmappingnotify xmappingnotify.cpp)

# Set compiler flags
target_compile_options(xmappingnotify PRIVATE -Wall -O2)

# Link necessary libraries
target_link_libraries(xmappingnotify PRIVATE X11 Xpm)
#--------------------------------------------------------------------------------------------------
