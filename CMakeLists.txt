cmake_minimum_required (VERSION 2.6)
project(Bezier)

set (PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_SOURCE_DIR ${PROJECT_ROOT_DIR}/src)

set (BEZIER_SRCS ${PROJECT_SOURCE_DIR}/main.c)

add_executable(Bezier ${BEZIER_SRCS})

if(${CMAKE_COMPILER_IS_GNUCC})
	link_directories(/usr/local/lib)
	target_link_libraries(Bezier glfw3 GL 
	# glfw3 library dependencies
	glfw3 rt Xrandr Xinerama Xi Xcursor m dl Xrender drm Xdamage X11-xcb 
		xcb-glx xcb-dri2 xcb-dri3 xcb-present xcb-sync xshmfence Xxf86vm 
		Xfixes Xext X11 pthread xcb Xau Xdmcp
	)
  set (CMAKE_CXX_FLAGS "-std=c++11")
endif()

