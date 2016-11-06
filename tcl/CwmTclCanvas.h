#include "CTclApp/CTclAppCanvas.h"
#include "CTclCanvas/CTclCanvas.h"

class CwmTcl;
class CwmTclCanvas;

class CwmTclCanvasCmd : public CTclAppCanvasCmd {
 private:
  CwmTcl       *cwm_;
  CwmTclCanvas *canvas_;

 public:
  CwmTclCanvasCmd(CwmTcl *cwm);
 ~CwmTclCanvasCmd();

  string getCommandName () const { return "CwmTclCanvas"; }
  string getCommandClass() const { return "CwmTclCanvas"; }

  CTclAppCanvas *createInstance(CTclApp *app);
};

class CwmTclCanvas : public CTclCanvas {
 private:
  CwmTcl *cwm_;

 public:
  CwmTclCanvas(CwmTcl *cwm);

 protected:
  bool instanceCommand(int argc, const char **argv);
  bool instanceConfigure(int argc, const char **argv);

  void draw();

  void mousePress();
  void mouseMotion();
  void mouseRelease();
  void keyPress();
  void keyRelease();
};
