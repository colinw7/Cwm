#include "std_c++.h"
#include "CwmResEdit.k"
#include "CTclApp/CTclApp.h"
#include "CTclApp/CTclAppCommand.h"

class CwmResEdit : public CTclApp {
 public:
  CwmResEdit(int argc, char **argv) :
   CTclApp(argc, argv) {
    init();
  }

  string getTclStr() { return CwmResEdit_TK; }

 protected:
  void addCommands();
};

class CwmResEditLoad : public CTclAppCommand {
 private:
  CwmResEdit *res_edit_;

 public:
  CwmResEditLoad(CwmResEdit *res_edit) :
    CTclAppCommand(res_edit, "CwmResEditLoad"), res_edit_(res_edit) {
  }

 protected:
  bool proc(int argc, const char **argv);
};

int
main(int argc, char **argv)
{
  CwmResEdit app(argc, argv);
}

void
CwmResEdit::
addCommands()
{
  new CwmResEditLoad(this);
}

bool
CwmResEditLoad::
proc(int, const char **)
{
  return true;
}
