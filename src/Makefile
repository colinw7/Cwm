CC = g++
AR = ar
RM = rm

CDEBUG = -g -DSHAPE -DCWM_USE_XT -DMWM_UTIL
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib
BIN_DIR = ../bin

all: $(BIN_DIR)/Cwm

SRC = \
CwmCmdIcon.cpp \
CwmColormapMgr.cpp \
CwmColorMgr.cpp \
CwmConfig.cpp \
Cwm.cpp \
CwmCursorMgr.cpp \
CwmCustomIcon.cpp \
CwmDebug.cpp \
CwmDecorationClose.cpp \
CwmDecorationContainer.cpp \
CwmDecoration.cpp \
CwmDecorationESide.cpp \
CwmDecorationMaximize.cpp \
CwmDecorationMenu.cpp \
CwmDecorationMinimize.cpp \
CwmDecorationNECorner.cpp \
CwmDecorationNSide.cpp \
CwmDecorationNWCorner.cpp \
CwmDecorationSECorner.cpp \
CwmDecorationSSide.cpp \
CwmDecorationSWCorner.cpp \
CwmDecorationTitle.cpp \
CwmDecorationWindow.cpp \
CwmDecorationWSide.cpp \
CwmDeskIcon.cpp \
CwmDeskMgr.cpp \
CwmEventMgr.cpp \
CwmFocusMgr.cpp \
CwmFontMgr.cpp \
CwmFrameWindow.cpp \
CwmFunctionDef.cpp \
CwmGnome.cpp \
CwmGrab.cpp \
CwmGraphics.cpp \
CwmGrid.cpp \
CwmHints.cpp \
CwmIdle.cpp \
CwmImageMgr.cpp \
CwmInfo.cpp \
CwmLog.cpp \
CwmMachine.cpp \
CwmMailMgr.cpp \
CwmMain.cpp \
CwmMask.cpp \
CwmMenu.cpp \
CwmMessage.cpp \
CwmMoveFeedback.cpp \
CwmMoveWindow.cpp \
CwmPaste.cpp \
CwmResizeFeedback.cpp \
CwmResizeWindow.cpp \
CwmResourceData.cpp \
CwmRoot.cpp \
CwmScreen.cpp \
CwmScreenMenu.cpp \
CwmTabWindow.cpp \
CwmToolBarClock.cpp \
CwmToolBarCmd.cpp \
CwmToolBar.cpp \
CwmToolBarIconArea.cpp \
CwmToolBarIconDef.cpp \
CwmToolBarMenu.cpp \
CwmUnixCmd.cpp \
CwmUserWindow.cpp \
CwmWindow.cpp \
CwmWindowImageList.cpp \
CwmWindowMenu.cpp \
CwmWindowStack.cpp \
CwmWMWindow.cpp \
CwmXEvent.cpp \
CwmXPixmap.cpp \
CGradient.cpp \
CImageDraw.cpp \
CImageGC.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-DSHAPE \
-I$(INC_DIR) \
-I../../CXLib/include \
-I../../CImageLib/include \
-I../../CFont/include \
-I../../COS/include \
-I../../CArgs/include \
-I../../CConfig/include \
-I../../CFile/include \
-I../../CMath/include \
-I../../CGlob/include \
-I../../CStrUtil/include \
-I../../CRGBName/include \
-I../../CTimer/include \
-I../../CUtil/include \
-I. \

LFLAGS = \
-L$(LIB_DIR) \
-L../../CXLib/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../COS/lib \
-L../../CArgs/lib \
-L../../CConfig/lib \
-L../../CTimer/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../CGlob/lib \
-L../../CRGBName/lib \
-L../../CStrUtil/lib \
-L../../CUtil/lib \

LIBS = \
-lCXLib -lCTimer -lCImageLib -lCFont -lCArgs -lCConfig -lCFile -lCUtil -lCOS \
-lCGlob -lCStrUtil -lCRGBName \
-lXt -lX11 -lpng -ljpeg

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/Cwm

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) $(CDEBUG) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

.SUFFIXES: .cpp

$(OBJ_DIR)/main.o: main.cpp
	$(CC) $(CDEBUG) -o $(OBJ_DIR)/main.o -c main.cpp -I$(INC_DIR) -I.

$(BIN_DIR)/Cwm: $(OBJS)
	$(CC) $(LDEBUG) -o $(BIN_DIR)/Cwm $(OBJS) $(LFLAGS) $(LIBS)
