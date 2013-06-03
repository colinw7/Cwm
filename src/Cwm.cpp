#include "CwmI.h"
#include "CArgs.h"
#include "COSUser.h"
#include "COSSignal.h"

Cwm::
Cwm()
{
  argc_         = 0;
  argv_         = NULL;
  pedantic_     = false;
  synchronize_  = false;
  root_image_   = "";
  home_dir_     = "";
  print_hints_  = false;
  display_name_ = "";
  gnome_        = false;

  setHomeDir();
}

Cwm::
~Cwm()
{
}

Cwm *
Cwm::
getInstance()
{
  static Cwm *instance;

  if (instance == NULL)
    instance = new Cwm();

  return instance;
}

void
Cwm::
init(int argc, char **argv)
{
  CwmMachineInst->log("Start Cwm\n");

  saveArgs(argc, argv);

  parseArgs(&argc, argv);

  if (! CwmMachineInst->init(display_name_))
    term(1);

  if (synchronize_)
    CwmMachineInst->synchronize();

  setSignals();

  controlScreens();

  processWindows();

  drawRootImages();

  addCustomIcons();

  if (CwmResourceDataInst->getClickToFocus())
    initClickToFocus();

  processScreenStartup();

  CwmMessageInst->init();

  CwmWMWindow::processEvents();

  exit(0);
}

void
Cwm::
term(int retcode)
{
  CwmMachineInst->term();

  CwmMachineInst->log("End Cwm\n");

  exit(retcode);
}

void
Cwm::
saveArgs(int argc, char **argv)
{
  argc_ = argc;
  argv_ = new char * [argc + 3];

  int i;

  for (i = 0; i < argc; i++)
    argv_[i] = strdup(argv[i]);

  argv_[i] = NULL;
}

void
Cwm::
parseArgs(int *argc, char **argv)
{
  string opts = "\
 -display:s     (Set Display Name) \
--root:s        (Set Root Image) \
--debug:f       (Set Debug Mode) \
--sync:f        (Set X Server Synchronous) \
--pedantic:f    (Core Dump on Errors) \
--print_hints:f (Display Hints) \
--gnome         (Run Under Gnome)";

  CArgs *cargs = new CArgs(opts);

  cargs->parse(argc, argv);

  display_name_ = cargs->getStringArg ("-display");
  root_image_   = cargs->getStringArg ("--root");
  synchronize_  = cargs->getBooleanArg("--sync");
  pedantic_     = cargs->getBooleanArg("--pedantic");
  print_hints_  = cargs->getBooleanArg("--print_hints");
  gnome_        = cargs->getBooleanArg("--gnome");

  if (cargs->getBooleanArg("--debug"))
    CwmMachineInst->setDebug(true);
}

void
Cwm::
setHomeDir()
{
  home_dir_ = COSUser::getUserHome();
}

void
Cwm::
setSignals()
{
  COSSignal::addSignalHandler(SIGHUP , (CwmSigHandler) &Cwm::signalHandler);
  COSSignal::addSignalHandler(SIGINT , (CwmSigHandler) &Cwm::signalHandler);
  COSSignal::addSignalHandler(SIGTERM, (CwmSigHandler) &Cwm::signalHandler);
  COSSignal::addSignalHandler(SIGQUIT, (CwmSigHandler) &Cwm::signalHandler);
}

void
Cwm::
controlScreens()
{
  if (! CwmMachineInst->controlScreens())
    term(1);

  CwmMachineInst->flushEvents();
}

void
Cwm::
processWindows()
{
  CwmGrabServer grab;

  int num_screens = CwmMachineInst->getNumScreens();

  for (int i = 0; i < num_screens; ++i) {
    CwmScreen &screen = CwmMachineInst->getScreen(i);

    processScreenWindows(screen);
  }
}

void
Cwm::
processScreenWindows(CwmScreen &screen)
{
  Window *children;
  int     num_children;

  if (! screen.getWindows(&children, &num_children))
    return;

  for (int i = 0; i < num_children; ++i) {
    if (children[i] != None &&
        CwmEventMgrInst->isDestroyPending(children[i]))
      children[i] = None;

    if (children[i] != None)
      resetIconWindows(i, children, num_children);

    if (children[i] != None &&
        ! CwmMachineInst->isWMWindow(children[i]))
      children[i] = None;
  }

  for (int i = 0; i < num_children; i++) {
    if (children[i] != None &&
        CwmEventMgrInst->isDestroyPending(children[i]))
      children[i] = None;

    if (children[i] != None) {
      CwmMachineInst->unmapWindow(children[i]);

      screen.addWMWindow(children[i]);
    }
  }

  if (children != NULL)
    XFree(children);
}

void
Cwm::
resetIconWindows(int i, Window *children, int num_children)
{
  Window icon_window = CwmMachineInst->getWMIconWindowHint(children[i]);

  if (icon_window == None)
    return;

  for (int j = 0; j < num_children; ++j)
    if (children[j] == icon_window)
      children[j] = None;
}

void
Cwm::
drawRootImages()
{
  int num_screens = CwmMachineInst->getNumScreens();

  for (int i = 0; i < num_screens; ++i) {
    CwmScreen &screen = CwmMachineInst->getScreen(i);

    CwmDesk *desk = screen.getCurrentDesk();

    desk->drawRootImage();
  }
}

void
Cwm::
addCustomIcons()
{
  int num_screens = CwmMachineInst->getNumScreens();

  for (int i = 0; i < num_screens; ++i) {
    CwmScreen &screen = CwmMachineInst->getScreen(i);

    CwmCustomIconMgrInst->addCustomIcons(screen);
  }
}

void
Cwm::
initClickToFocus()
{
  CwmMachineInst->resetInputFocus();

  CwmScreen &screen = CwmMachineInst->getScreen(0);

  screen.getRoot()->warpPointer(0, 0);
}

void
Cwm::
processScreenStartup()
{
  CwmGrabServer grab;

  int num_screens = CwmMachineInst->getNumScreens();

  for (int i = 0; i < num_screens; ++i) {
    CwmScreen &screen = CwmMachineInst->getScreen(i);

    CwmResourceDataInst->loadStartupCommands(screen);
  }
}

void
Cwm::
printScreenWindows(CwmScreen &screen)
{
  Window *children;
  int     num_children;

  if (! screen.getWindows(&children, &num_children))
    return;

  for (int i = 0; i < num_children; ++i)
    CwmMachineInst->logf("Window %x\n", (int) children[i]);

  if (children != NULL)
    XFree(children);
}

void
Cwm::
restart(CwmScreen &, const string &program)
{
  if (program == "") {
    if (root_image_ != "") {
      argv_[argc_++] = strdup("--root");
      argv_[argc_++] = strdup(root_image_.c_str());
      argv_[argc_++] = NULL;
    }

    cleanup();

    execvp(argv_[0], argv_);
  }
  else {
    CStrWords words = CStrUtil::toWords(program, NULL);

    char **words1 = new char * [words.size() + 1];

    int i;

    for (i = 0; i < words.size(); ++i)
      words1[i] = (char *) words[i].getWord().c_str();

    words1[i] = NULL;

    cleanup();

    execvp(words1[0], words1);
  }
}

void
Cwm::
quit()
{
  cleanup();

  term(0);
}

void
Cwm::
cleanup()
{
  CwmWindowGlobalNotifyMgrInst->clear();

  CwmMachineInst->resetInputFocus();

  CwmCmdIconMgrInst->deleteAll();

  CwmCustomDeskIconMgrInst->deleteAll();

  CwmWindowEventFunctionMgrInst->removeAllEventFunctions();

  CwmRootEventFunctionMgrInst->deleteAll();

  CwmNamedMenuMgrInst->deleteAll();

  CwmMachineInst->term();
}

void
Cwm::
signalHandler(int sig)
{
  string sigstr = COSSignal::strsignal(sig);

  CwmMachineInst->debugf("Signal %s(%d) received\n", sigstr.c_str(), sig);

  if      (sig == SIGINT)
    CwmInst->restart(CwmMachineInst->getScreen(0), "");
  else if (sig == SIGTERM)
    CwmInst->quit();
}

void
Cwm::
setRootImage(const string &filename)
{
  root_image_ = filename;
}
