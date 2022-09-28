#include <CwmI.h>

void
CwmPaste::
pasteText(CwmWMWindow *window, const std::string &text)
{
  for (uint i = 0; i < text.size(); i++)
    pasteChar(window, text[i]);
}

void
CwmPaste::
pasteChar(CwmWMWindow *window, int c)
{
  auto *user = window->getUser();

  if (! user)
    return;

  char key_string[2];

  key_string[0] = char(c);
  key_string[1] = '\0';

  KeySym keysym = XStringToKeysym(key_string);

  if (keysym == NoSymbol)
    return;

  uint keycode = CwmMachineInst->keysymToKeycode(keysym);

  CwmMachineInst->sendKeyPressedEvent(user->getXWindow()->getXWin(),
                                      user->getX(), user->getY(), int(keycode));

  CwmMachineInst->sendKeyReleasedEvent(user->getXWindow()->getXWin(),
                                       user->getX(), user->getY(), int(keycode));
}
