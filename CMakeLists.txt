
project(gl-percolation)
cmake_minimum_required (VERSION 2.6.0)

find_package(Qt4)
set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})
include_directories (${QT_INCLUDE_DIR})
qt4_wrap_cpp(MOC_SOURCES
	mainwindow.h glwidget.h)
add_executable (gl
	mainwindow.cpp
	glwidget.cpp
	main.cpp
	${MOC_SOURCES})
target_link_libraries(gl
	${QT_LIBRARIES})

