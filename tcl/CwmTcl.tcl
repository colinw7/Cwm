proc CwmTclCreateWidgets { parent } {
  wm title $parent. "CwmTcl"

  wm protocol $parent. WM_DELETE_WINDOW CwmTclExit

  gset cwm_tcl_root $parent.

  CwmTclCanvas $parent.canvas -width 400 -height 400

  bind $parent.canvas <Enter> {focus %W}

  pack $parent.canvas  -side top -fill both -expand true -anchor nw

  gset cwm_tcl_canvas $parent.canvas

  after 250 CwmTclCheckUpdateCB
}

proc CwmTclRedraw { } {
  global cwm_tcl_canvas

  $cwm_tcl_canvas set fg white

  $cwm_tcl_canvas clear

  CwmTclSetScreenScale

  set xb 0
  set yb 0

  CwmTclPixelToWindow 10 10 xb yb

  set windows [cwm_get_windows]

  set rwindows [lreverse $windows]

  set x1 0
  set y1 0
  set x2 0
  set y2 0

  foreach window $rwindows {
    CwmTclGetWindowRect $window x1 y1 x2 y2

    $cwm_tcl_canvas set fg gray

    $cwm_tcl_canvas fill rect $x1 $y1 $x2 $y2

    $cwm_tcl_canvas set fg black

    $cwm_tcl_canvas fill rect [expr $x1 + $xb] [expr $y1 + $yb] \
                              [expr $x2 - $xb] [expr $y2 - $yb]
  }
}

proc CwmTclMousePress { button x y } {
  set window [CwmTclGetXYWindow $x $y]

  if {$button == 1} {
    if {$window != 0} {
      cwm_raise_window $window
    }
  }
}

proc CwmTclGetXYWindow { x y } {
  CwmTclSetScreenScale

  set windows [cwm_get_windows]

  set x1 0
  set y1 0
  set x2 0
  set y2 0

  foreach window $windows {
    CwmTclGetWindowRect $window x1 y1 x2 y2

    if {$x >= $x1 && $x <= $x2 && $y >= $y1 && $y <= $y2} {
      return $window
    }
  }

  return 0
}

proc CwmTclSetScreenScale { } {
  global cwm_tcl_canvas

  set screen_width  [ cwm_get_screen_value width ]
  set screen_height [ cwm_get_screen_value height]

  set canvas_width  [winfo width  $cwm_tcl_canvas]
  set canvas_height [winfo height $cwm_tcl_canvas]

  gset cwm_tcl_xscale [expr (1.0 * $canvas_width ) / $screen_width ]
  gset cwm_tcl_yscale [expr (1.0 * $canvas_height) / $screen_height]
}

proc CwmTclGetWindowRect { window x1 y1 x2 y2 } {
  upvar $x1 x1_up
  upvar $y1 y1_up
  upvar $x2 x2_up
  upvar $y2 y2_up

  set x1_up [cwm_get_window_value $window x     ]
  set y1_up [cwm_get_window_value $window y     ]
  set x2_up [expr $x1_up + [cwm_get_window_value $window width ] - 1]
  set y2_up [expr $y1_up + [cwm_get_window_value $window height] - 1]

  CwmTclPixelToWindow $x1_up $y1_up x1_up y1_up
  CwmTclPixelToWindow $x2_up $y2_up x2_up y2_up
}

proc CwmTclPixelToWindow { px py wx wy } {
  upvar $wx wx_up
  upvar $wy wy_up

  global cwm_tcl_xscale cwm_tcl_yscale

  set wx_up [expr $px * $cwm_tcl_xscale]
  set wy_up [expr $py * $cwm_tcl_yscale]
}

proc CwmTclCheckUpdateCB { } {
  CwmTclCheckUpdate

  after 250 CwmTclCheckUpdateCB
}

proc lreverse { l } {
  set num [llength $l]

  set l1 { }

  for {set i [expr $num - 1] } {$i >= 0} {incr i -1} {
    lappend l1 [lindex $l $i]
  }

  return $l1
}

CwmTclCreateWidgets ""
