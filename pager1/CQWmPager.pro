APPNAME = CQWmPager

include($$(MAKE_DIR)/qt_app.mk)

QT += x11extras

SOURCES += \
CQWmPager.cpp \
CQBypassWindow.cpp \

HEADERS += \
CQWmPager.h \
CQBypassWindow.h \

INCLUDEPATH += \

PRE_TARGETDEPS = \

unix:LIBS += \
-lX11 \
