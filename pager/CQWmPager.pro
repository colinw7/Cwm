TEMPLATE = app

QT += widgets x11extras

TARGET = CQWmPager

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11 -DGNUPLOT_EXPR -DRGB_UTIL

MOC_DIR = .moc

CONFIG += debug

# Input
SOURCES += \
CQWmPager.cpp \

HEADERS += \
CQWmPager.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CXLib/include \
../../CFont/include \
../../CImageLib/include \
../../CTimer/include \
../../CReadLine/include \
../../CFile/include \
../../COS/include \
../../CUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CRGBName/include \
../../CXML/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CXLib/lib \
-L../../CFont/lib \
-L../../CImageLib/lib \
-L../../CConfig/lib \
-L../../CFile/lib \
-L../../CFileUtil/lib \
-L../../CTimer/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCXLib -lCFont -lCImageLib -lCTimer -lCConfig -lCUtil \
-lCRGBName -lCFile -lCFileUtil -lCOS -lCStrUtil -lCRegExp \
-lXt -lX11 -lpng -ljpeg -ltre -lreadline
