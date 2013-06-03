#include "CwmI.h"

void
CwmPaste::
pasteText(CwmWMWindow *window, const string &text)
{
  for (int i = 0; i < (int) text.size(); i++)
    pasteChar(window, text[i]);
}

void
CwmPaste::
pasteChar(CwmWMWindow *window, int c)
{
  CwmUserWindow *user = window->getUser();

  if (user == NULL)
    return;

  char key_string[2];

  key_string[0] = c;
  key_string[1] = '\0';

  KeySym keysym = XStringToKeysym(key_string);

  if (keysym == NoSymbol)
    return;

  uint keycode = CwmMachineInst->keysymToKeycode(keysym);

  CwmMachineInst->sendKeyPressedEvent(user->getXWindow()->getXWin(),
                                      user->getX(), user->getY(),
                                      keycode);

  CwmMachineInst->sendKeyReleasedEvent(user->getXWindow()->getXWin(),
                                       user->getX(), user->getY(),
                                       keycode);
}
