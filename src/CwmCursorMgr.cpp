#include <CwmI.h>

struct CwmCursorDef {
  CwmCursorType type;
  CCursorType   xtype;
};

static CwmCursorDef
cursor_def[] = {
  { CWM_CURSOR_TITLE , CURSOR_TYPE_TOP_LEFT_ARROW      },
  { CWM_CURSOR_N     , CURSOR_TYPE_TOP_SIDE            },
  { CWM_CURSOR_S     , CURSOR_TYPE_BOTTOM_SIDE         },
  { CWM_CURSOR_W     , CURSOR_TYPE_LEFT_SIDE           },
  { CWM_CURSOR_E     , CURSOR_TYPE_RIGHT_SIDE          },
  { CWM_CURSOR_NW    , CURSOR_TYPE_TOP_LEFT_CORNER     },
  { CWM_CURSOR_NE    , CURSOR_TYPE_TOP_RIGHT_CORNER    },
  { CWM_CURSOR_SW    , CURSOR_TYPE_BOTTOM_LEFT_CORNER  },
  { CWM_CURSOR_SE    , CURSOR_TYPE_BOTTOM_RIGHT_CORNER },
  { CWM_CURSOR_MOVE  , CURSOR_TYPE_FLEUR               },
  { CWM_CURSOR_MENU  , CURSOR_TYPE_SB_LEFT_ARROW       },
  { CWM_CURSOR_BUSY  , CURSOR_TYPE_WATCH               },
  { CWM_CURSOR_SELECT, CURSOR_TYPE_ARROW               },
  { CWM_CURSOR_QUERY , CURSOR_TYPE_QUESTION_ARROW      },
  { CWM_CURSOR_PLACE , CURSOR_TYPE_TOP_LEFT_ANGLE      },
  { CWM_CURSOR_NONE  , CURSOR_TYPE_TOP_LEFT_ARROW      },
};

CwmCursorMgr::
CwmCursorMgr(CwmScreen &screen) :
 screen_(screen)
{
  CwmColor *color = CwmResourceDataInst->getCursorColor(screen_);

  CRGBA fg_rgba = color->getFgRGBA();
  CRGBA bg_rgba = color->getBgRGBA();

  for (int i = 0; cursor_def[i].type != CWM_CURSOR_NONE; ++i) {
    CwmXCursor *cursor = new CwmXCursor(screen_, cursor_def[i].xtype);

    if (cursor_def[i].type != CWM_CURSOR_BUSY)
      cursor->recolor(bg_rgba, fg_rgba);

    cursor_list_.push_back(cursor);

    cursor_map_[cursor_def[i].type] = cursor;
  }
}

CwmCursorMgr::
~CwmCursorMgr()
{
  std::for_each(cursor_list_.begin(), cursor_list_.end(), CDeletePointer());
}

Cursor
CwmCursorMgr::
getCursor(CwmCursorType type) const
{
  CursorMap::const_iterator pcursor = cursor_map_.find(type);

  if (pcursor == cursor_map_.end())
    return None;

  return (*pcursor).second->getCursor()->getXCursor();
}

CwmXCursor::
CwmXCursor(CwmScreen &screen, CCursorType type)
{
  xcursor_ = new CXCursor(*screen.getCXScreen(), type);
}

CwmXCursor::
~CwmXCursor()
{
  delete xcursor_;
}

void
CwmXCursor::
recolor(CRGBA &bg, CRGBA &fg)
{
  xcursor_->recolor(bg, fg);
}
