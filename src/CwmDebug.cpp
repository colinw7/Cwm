#include "CwmI.h"

void
CwmDebug::
print(const string &msg)
{
  if (debug_)
    CwmMachineInst->logf("Debug: %s", msg.c_str());
}
