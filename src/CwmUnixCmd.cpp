#include "CwmI.h"
#include "COSSignal.h"
#include "sys/wait.h"

UnixCmd::
UnixCmd(CwmScreen &screen, const string &command) :
 screen_(screen), command_(command), status_(0), add_display_(false)
{
}

void
UnixCmd::
run()
{
  pid_t pid = fork();

  if (pid == 0) {
    COSSignal::defaultSignal(SIGHUP );
    COSSignal::defaultSignal(SIGINT );
    COSSignal::defaultSignal(SIGQUIT);
    COSSignal::defaultSignal(SIGTERM);

    string command_string;

    string::size_type pos = command_.rfind('&');

    if (pos != string::npos)
      command_string += command_.substr(0, pos);
    else
      command_string += command_;

    if (add_display_) {
      string name = CwmMachineInst->getDisplayName();

      if (name != "")
        command_string += " -display " + name;
    }

    command_string += " &";

    execl("/bin/sh", "sh", "-c", command_string.c_str(), NULL);

    _exit(255);
  }

  CwmSigHandler sig_hup  = (CwmSigHandler) COSSignal::getSignalHandler(SIGHUP );
  CwmSigHandler sig_int  = (CwmSigHandler) COSSignal::getSignalHandler(SIGINT );
  CwmSigHandler sig_quit = (CwmSigHandler) COSSignal::getSignalHandler(SIGQUIT);
  CwmSigHandler sig_term = (CwmSigHandler) COSSignal::getSignalHandler(SIGTERM);

  COSSignal::ignoreSignal(SIGHUP );
  COSSignal::ignoreSignal(SIGINT );
  COSSignal::ignoreSignal(SIGQUIT);
  COSSignal::ignoreSignal(SIGTERM);

  int status;

  pid_t child_pid = wait(&status);

  while (child_pid != pid && child_pid != -1)
    child_pid = wait(&status);

  if (child_pid == -1)
    status = -1;

  COSSignal::addSignalHandler(SIGHUP , sig_hup );
  COSSignal::addSignalHandler(SIGINT , sig_int );
  COSSignal::addSignalHandler(SIGQUIT, sig_quit);
  COSSignal::addSignalHandler(SIGTERM, sig_term);

  status_ = status;
}
