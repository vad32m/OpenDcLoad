/**
 * Coordinates for UI elements of parameter setup view
 */

#ifndef SRC_UI_VIEWS_PARAM_SETUP_VIEW_LAYOUT_H_
#define SRC_UI_VIEWS_PARAM_SETUP_VIEW_LAYOUT_H_

#include "ui/views/basic_view_layout.h"

/*
+--------------------------------------------------------------------------------------+
|                                                                                      |
|                               Current parameters                                     |
|                                                                                      |
+--------------------------------------------------------------------------------------+
|                                                                                      |
|                      200                                220                          |
|      +<-------------------------------->+<----------------------------------->+      |
|      |                                  |                                     |      |
|      +-----------------------------------------------------------------------------+ |
|      |(30, 110)                         |                                     |    ^ |
|      |            Value name            |        Value parameter              | 60 | |
|      |                                  |                                     |    v |
|      +----------------------------------+-------------------------------------+----+ |
|                                                                                      |
|                                     420                                              |
|      +<---------------------------------------------------------------------->+      |
|      |                                                                        |      |
|      +-----------------------------------------------------------------------------+ |
|      |(30, 220)                                                               |    ^ |
|      |                        Progress-bar                                    | 40 | |
|      |                                                                        |    v |
|      +------------------------------------------------------------------------+----+ |
|                                                                                      |
+--------------------------------------------------------------------------------------+
|                                                                                      |
|                                 Buttons                                              |
|                                                                                      |
+--------------------------------------------------------------------------------------+
*/

#define UI_PARAM_TABLE_X_POS 30
#define UI_PARAM_TABLE_Y_POS 110
#define UI_PARAM_TABLE_HEIGHT 60
#define UI_PARAM_TABLE_COL_COUNT 2
#define UI_PARAM_TABLE_ROW_COUNT 1
#define UI_PARAM_TABLE_FIRST_COL_WIDTH 200
#define UI_PARAM_TABLE_SECOND_COL_WIDTH 220

#define UI_PARAM_BAR_X_POS 30
#define UI_PARAM_BAR_Y_POS 220
#define UI_PARAM_BAR_HEIGHT 40
#define UI_PARAM_BAR_WIDTH 420

#endif /* SRC_UI_VIEWS_PARAM_SETUP_VIEW_LAYOUT_H_ */
