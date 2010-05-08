QT += opengl
TARGET = gl
TEMPLATE = app

SOURCES += \
	mainwindow.cpp       \
	glwidget.cpp         \
	simulationthread.cpp \
	visframe.cpp         \
	main.cpp             \
	playbackslider.cpp   \
	timeplot.cpp         \
	fileformat.cpp

HEADERS += \
	mainwindow.h         \
	glwidget.h           \
	fileformat.h         \
	visframe.h           \
	playbackslider.h     \
	timeplot.h           \
	simulationthread.h

