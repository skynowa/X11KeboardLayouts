#--------------------------------------------------------------------------------------------------
# \file  [PROJECT_NAME].cmake
# \brief
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

# Add the source file
add_executable(${PROJECT_NAME}
	main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
	${X11_INCLUDE_DIR})

# Link necessary libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
	${X11_LIBRARIES})
#--------------------------------------------------------------------------------------------------
