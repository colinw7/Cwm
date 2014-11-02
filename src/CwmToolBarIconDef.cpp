#include <CwmToolBarI.h>

CwmToolBarIconDefMgr *
CwmToolBarIconDefMgr::
getInstance()
{
  static CwmToolBarIconDefMgr *instance;

  if (! instance)
    instance = new CwmToolBarIconDefMgr();

  return instance;
}

CwmToolBarIconDefMgr::
CwmToolBarIconDefMgr()
{
}

CwmToolBarIconDefMgr::
~CwmToolBarIconDefMgr()
{
  ToolBarIconDefList::const_iterator picon1 = toolbar_icons_.begin();
  ToolBarIconDefList::const_iterator picon2 = toolbar_icons_.end  ();

  for ( ; picon1 != picon2; ++picon1)
    delete *picon1;
}

void
CwmToolBarIconDefMgr::
addToolBarIcon(const std::string &icon, const std::string &label,
               const std::string &command, bool add_display,
               const std::string &bg, const std::string &fg)
{
  CwmToolBarIconDef *toolbar_icon =
    new CwmToolBarIconDef(icon, label, command, add_display, bg, fg);

  toolbar_icons_.push_back(toolbar_icon);
}

void
CwmToolBarIconDefMgr::
addToolBarIcons(CwmToolBar &toolbar)
{
  ToolBarIconDefList::const_iterator picon1 = toolbar_icons_.begin();
  ToolBarIconDefList::const_iterator picon2 = toolbar_icons_.end  ();

  for ( ; picon1 != picon2; ++picon1)
    toolbar.addCmdIcon(*picon1);
}

CwmToolBarIconDef::
CwmToolBarIconDef(const std::string &icon, const std::string &label,
                  const std::string &command, bool add_display,
                  const std::string &bg, const std::string &fg) :
 icon_(icon), label_(label), command_(command),
 add_display_(add_display), bg_(bg), fg_(fg)
{
}

CwmToolBarIconDef::
~CwmToolBarIconDef()
{
}
