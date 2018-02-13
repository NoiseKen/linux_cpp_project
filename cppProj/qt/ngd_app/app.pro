TARGET = ngd_app.exe
QMAKE_CXXFLAGS += -Wno-unused-result -std=c++11 -DOS_LINUX
LIBSRC		= $(HOME)/work/cppProj/library/src
QLIBSRC		= $(HOME)/work/cppProj/QLibrary/src
TEMPLATE    = app
FORMS       = main_window.ui vcl_nvme_dev.ui
INCLUDEPATH = $${LIBSRC}/../include $${QLIBSRC}/../include
HEADERS += $${QLIBSRC}/../include/event_ctrl.h 

SOURCES	= main.cpp form_app.cpp vcl_nvme_dev.cpp nvme_io.cpp time_log.cpp \
		$${QLIBSRC}/event_ctrl.cpp	\
		$${QLIBSRC}/html_string.cpp	\
		$${QLIBSRC}/utils_qt.cpp	\
		$${LIBSRC}/io_ctrl.cpp	\
		$${LIBSRC}/extension_string.cpp	\
		$${LIBSRC}/string_list.cpp	\

