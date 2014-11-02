#include <CwmI.h>

CwmXEvent::
CwmXEvent()
{
}

CwmXEvent::
~CwmXEvent()
{
}

CwmXEvent *
CwmXEvent::
getInstance()
{
  static CwmXEvent *instance;

  if (! instance)
    instance = new CwmXEvent();

  return instance;
}

bool
CwmXEvent::
isKeyPressEvent(XKeyPressedEvent *event, int key)
{
  char key_string[2];

  key_string[0] = key;
  key_string[1] = '\0';

  KeySym keysym = XStringToKeysym(key_string);

  if (keysym == NoSymbol) {
    CwmMachineInst->logf("Unknowm KeySym Name %s\n", key_string);
    return false;
  }

  uint keycode = CwmMachineInst->keysymToKeycode(keysym);

  if (event->keycode == keycode &&
      (event->state == 0 || event->state == 1))
    return true;

  return false;
}

bool
CwmXEvent::
isEscapeKeyEvent(XKeyPressedEvent *event)
{
  KeySym keysym = XStringToKeysym("Escape");

  uint keycode = CwmMachineInst->keysymToKeycode(keysym);

  if (event->keycode == keycode && event->state == 0)
    return true;

  return false;
}
