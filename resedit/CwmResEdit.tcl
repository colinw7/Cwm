package require Tix

proc createWidgets { parent } {
  set notebook $parent.notebook

  tixNoteBook $notebook

  $notebook add general  -label "General"
  $notebook add menus    -label "Menus"
  $notebook add root     -label "Root"
  $notebook add desks    -label "Desks"
  $notebook add windows  -label "Windows"
  $notebook add cmdicons -label "Command Icons"

  createGeneralWidgets [$notebook subwidget general]

  pack $notebook -fill both -expand true
}

proc createGeneralWidgets { parent } {
  set notebook $parent.notebook

  tixNoteBook $notebook

  $notebook add misc     -label "Misc"
  $notebook add window   -label "Window"
  $notebook add menu     -label "Menu"
  $notebook add icon     -label "Icon"
  $notebook add info     -label "Info"
  $notebook add feedback -label "Feedback"
  $notebook add cursor   -label "Cursor"
  $notebook add toolbar  -label "Toolbar"

  createGeneralMiscWidgets     [$notebook subwidget misc    ]
  createGeneralWindowWidgets   [$notebook subwidget window  ]
  createGeneralMenuWidgets     [$notebook subwidget menu    ]
  createGeneralIconWidgets     [$notebook subwidget icon    ]
  createGeneralInfoWidgets     [$notebook subwidget info    ]
  createGeneralFeedbackWidgets [$notebook subwidget feedback]
  createGeneralCursorWidgets   [$notebook subwidget cursor  ]
  createGeneralToolbarWidgets  [$notebook subwidget toolbar ]

  pack $notebook -fill both -expand true
}

proc createGeneralMiscWidgets { parent } {
  createStringPrompt $parent "double_click" "Double Click" \
    general_double_click 13
}

proc createGeneralWindowWidgets { parent } {
  createStringPrompt $parent "title_align"           "Title Align" \
    general_window_title_align           20
  createStringPrompt $parent "click_to_focus"        "Click To Focus" \
    general_window_click_to_focus        20
  createStringPrompt $parent "focus_auto_raise"      "Focus Auto Raise" \
    general_window_focus_auto_raise      20
  createStringPrompt $parent "position_is_frame"     "Position Is Frame" \
    general_window_position_is_frame     20
  createStringPrompt $parent "enable_warp"           "Enable Warp" \
    general_window_enable_warp           20
  createStringPrompt $parent "focus_grab_keys"       "Focus Grab Keys" \
    general_window_focus_grab_keys       20
  createStringPrompt $parent "move_opaque"           "Move Opaque" \
    general_window_move_opaque           20
  createStringPrompt $parent "client_auto_place"     "Client Auto Place" \
    general_window_client_auto_place     20
  createStringPrompt $parent "interactive_placement" "Interactive Placement" \
    general_window_interactive_placement 20
  createStringPrompt $parent "position_on_screen"    "Position On Screen" \
    general_window_position_on_screen    20

  #------

  set normal_frame $parent.normal_frame

  tixLabelFrame $normal_frame -label "Normal"

  createGeneralWindowNormalWidgets [$normal_frame subwidget frame]

  pack $normal_frame -side top -anchor w

  #------

  set focus_frame $parent.focus_frame

  tixLabelFrame $focus_frame -label "Focus"

  createGeneralWindowFocusWidgets [$focus_frame subwidget frame]

  pack $focus_frame -side top -anchor w
}

proc createGeneralWindowNormalWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_window_normal_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_window_normal_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_window_normal_font       11
}

proc createGeneralWindowFocusWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_window_focus_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_window_focus_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_window_focus_font       11
}

proc createGeneralMenuWidgets { parent } {
  set title_frame $parent.title_frame

  tixLabelFrame $title_frame -label "Title"

  createGeneralMenuTitleWidgets [$title_frame subwidget frame]

  pack $title_frame -side top -anchor w

  #------

  set item_frame $parent.item_frame

  tixLabelFrame $item_frame -label "Item"

  createGeneralMenuItemWidgets [$item_frame subwidget frame]

  pack $item_frame -side top -anchor w
}

proc createGeneralMenuTitleWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_menu_title_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_menu_title_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_menu_title_font       11
}

proc createGeneralMenuItemWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_menu_item_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_menu_item_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_menu_item_font       11
}

proc createGeneralIconWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_icon_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_icon_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_icon_font       11
}

proc createGeneralInfoWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_info_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_info_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_info_font       11
}

proc createGeneralFeedbackWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_feedback_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_feedback_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_feedback_font       11
}

proc createGeneralCursorWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_cursor_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_cursor_foreground 11
}

proc createGeneralToolbarWidgets { parent } {
  set background_frame $parent.background_frame

  tixLabelFrame $background_frame -label "Background"

  createGeneralToolbarBackgroundWidgets [$background_frame subwidget frame]

  pack $background_frame -side top -anchor w 

  #------

  set menu_frame $parent.menu_frame
  
  tixLabelFrame $menu_frame -label "Menu"

  createGeneralToolbarMenuWidgets [$menu_frame subwidget frame]
  
  pack $menu_frame -side top -anchor w
  
  #------
  
  set icon_normal_frame $parent.icon_normal_frame

  tixLabelFrame $icon_normal_frame -label "Icon (Normal)"

  createGeneralToolbarIconNormalWidgets [$icon_normal_frame subwidget frame]
  
  pack $icon_normal_frame -side top -anchor w

  #------

  set icon_focus_frame $parent.icon_focus_frame

  tixLabelFrame $icon_focus_frame -label "Icon (Focus)"

  createGeneralToolbarIconNormalWidgets [$icon_focus_frame subwidget frame]

  pack $icon_focus_frame -side top -anchor w

  #------

  set clock_frame $parent.clock_frame

  tixLabelFrame $clock_frame -label "Clock"

  createGeneralToolbarClockWidgets [$clock_frame subwidget frame]

  pack $clock_frame -side top -anchor w
}

proc createGeneralToolbarBackgroundWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_toolbar_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_toolbar_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_toolbar_font       11
}

proc createGeneralToolbarMenuWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_toolbar_menu_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_toolbar_menu_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_toolbar_menu_font       11
}

proc createGeneralToolbarIconNormalWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_toolbar_icon_normal_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_toolbar_icon_normal_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_toolbar_icon_normal_font       11
}

proc createGeneralToolbarIconFocusWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_toolbar_icon_focus_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_toolbar_icon_focus_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_toolbar_icon_focus_font       11
}

proc createGeneralToolbarClockWidgets { parent } {
  createStringPrompt $parent "background" "Background" \
    general_toolbar_icon_clock_background 11
  createStringPrompt $parent "foreground" "Foreground" \
    general_toolbar_icon_clock_foreground 11
  createStringPrompt $parent "font"       "Font" \
    general_toolbar_icon_clock_font       11
  createStringPrompt $parent "format"     "Format" \
    general_toolbar_icon_clock_format     11
}

proc createStringPrompt { parent name label var width } {
  set frame $parent.$name

  frame $frame

  label $frame.label -text $label -width $width
  entry $frame.entry -textvariable $var

  pack $frame.label $frame.entry -side left -anchor w

  pack $frame -side top -anchor w
}

createWidgets ""
