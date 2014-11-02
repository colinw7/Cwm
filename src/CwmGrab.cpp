#include <CwmI.h>

CwmGrabServer::
CwmGrabServer()
{
  CwmMachineInst->grabServer();
}

CwmGrabServer::
~CwmGrabServer()
{
  CwmMachineInst->ungrabServer();
}
