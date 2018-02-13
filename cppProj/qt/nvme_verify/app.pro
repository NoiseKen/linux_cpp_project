TARGET = nvme_verify_ap.exe
QMAKE_CXXFLAGS += -Wno-unused-result -DOS_LINUX
LIBSRC		= $(HOME)/work/cppProj/library/src
QLIBSRC		= $(HOME)/work/cppProj/QLibrary/src
TEMPLATE    = app
FORMS       = main_window.ui
INCLUDEPATH = $${LIBSRC}/../include $${QLIBSRC}/../include
HEADERS += $${QLIBSRC}/../include/event_ctrl.h spec_field.h
SOURCES     = main.cpp form_app.cpp nvme_device.cpp nvme_ioctrl.cpp spec_field.cpp spec_dword.cpp	\
		lib_extract.cpp nvme_task.cpp \		
		$${QLIBSRC}/console_cli.cpp	\
		$${QLIBSRC}/event_ctrl.cpp	\
		$${QLIBSRC}/html_string.cpp	\
		$${QLIBSRC}/utils_qt.cpp	\
		$${LIBSRC}/io_ctrl.cpp	\
		$${LIBSRC}/string_list.cpp	\
		$${LIBSRC}/extension_string.cpp	\
		$${LIBSRC}/value_list.cpp	\
		$${LIBSRC}/terminal_ctrl.cpp	\
		$${LIBSRC}/ansi_gen.cpp	\
		$${LIBSRC}/legacy_string.cpp
