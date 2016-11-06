#include "std_c++.h"
#include "CTclCanvas/CTclCanvas.h"
#include "CwmTcl/CwmTcl.h"
#include "CwmTcl/CwmTclCanvas.h"
#include "CStrUtil/CStrUtil.h"

CwmTclCanvasCmd::
CwmTclCanvasCmd(CwmTcl *cwm) :
 CTclAppCanvasCmd(cwm), cwm_(cwm), canvas_(NULL)
{
  init();
}

CwmTclCanvasCmd::
~CwmTclCanvasCmd()
{
  delete canvas_;
}

CTclAppCanvas *
CwmTclCanvasCmd::
createInstance(CTclApp *)
{
  canvas_ = new CwmTclCanvas(cwm_);

  canvas_->setForeground(CRGB(0.0));
  canvas_->setBackground(CRGB(1.0));

  cwm_->addCanvas(canvas_);

  return (CTclAppCanvas *) canvas_->getCanvasWidget();
}

CwmTclCanvas::
CwmTclCanvas(CwmTcl *cwm) :
 CTclCanvas(cwm), cwm_(cwm)
{
}

bool
CwmTclCanvas::
instanceCommand(int, const char **)
{
  return true;
}

bool
CwmTclCanvas::
instanceConfigure(int, const char **)
{
  return true;
}

void
CwmTclCanvas::
draw()
{
  cwm_->redraw();
}

void
CwmTclCanvas::
mousePress()
{
  int button = getMouseButton();
  int x      = getMouseX();
  int y      = getMouseY();

  cwm_->mousePress(button, x, y);
}

void
CwmTclCanvas::
mouseMotion()
{
}

void
CwmTclCanvas::
mouseRelease()
{
}

void
CwmTclCanvas::
keyPress()
{
  string key = getKeyStr();

  cwm_->keyPress(key);
}

void
CwmTclCanvas::
keyRelease()
{
}
