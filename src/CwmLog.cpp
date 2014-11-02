#include <CwmI.h>
#include <COSTime.h>

CwmLog::
CwmLog() :
 fileName_("/usr/tmp/Cwm.log")
{
  setDir();

  fileName_ = dir_ + "/Cwm.log";

  if (! open()) {
    fileName_  = dir_ + "/Cwm_";
    fileName_ += COSTime::getTimeString("%Y_%m_%d_%H_%M_%S");
    fileName_ += ".log";

    open();
  }
}

CwmLog::
~CwmLog()
{
  delete file_;
}

bool
CwmLog::
open()
{
  if (! CFile::exists(fileName_) || CFile::isRegular(fileName_)) {
    file_ = new CFile(fileName_);

    file_->remove();

    return file_->open(CFile::APPEND);
  }
  else {
    file_ = 0;

    return false;
  }
}

void
CwmLog::
print(const std::string &msg)
{
  if (file_ != 0) {
    file_->write(COSTime::getTimeString() + ": " + msg);

    file_->flush();
  }
}

void
CwmLog::
setDir()
{
  dir_ = "/usr/tmp";

  if (! CFile::isDirectory(dir_)) {
    dir_ = "/tmp";

    if (! CFile::isDirectory(dir_))
      dir_ = ".";
  }
}
