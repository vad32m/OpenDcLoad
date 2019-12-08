/*
 * basic_view_layout.h
 *
 *  Created on: Dec 8, 2019
 *      Author: vadym
 */

#ifndef SRC_UI_VIEWS_BASIC_VIEW_LAYOUT_H_
#define SRC_UI_VIEWS_BASIC_VIEW_LAYOUT_H_

 /*
          +-----+--------------------------------------------------------------------------+
          ^     | (0,0)                                                                    |
    35 px |     |              Top panel with current parameters                           |
          v     |                                                                          |
          +-----+--------------------------------------------------------------------------+
                |                                                                          |
                |                                                                          |
                |                                                                          |
                |                                                                          |
                |                                                                          |
                |                                                                          |
                |                                                                          |
                |                                                                          |
                |                                                                          |
          +-----+--------------------------------------------------------------------------+
          ^     | (0,285)                                                                  |
    35 px |     |                           Buttons                                        |
          v     |                                                                          |
          +-----+--------------------------------------------------------------------------+

 */

#define UI_VIEW_WIDTH 480
#define UI_VIEW_HEIGHT 320

#define UI_TOP_RIBBON_X_POS 0
#define UI_TOP_RIBBON_Y_POS 0
#define UI_TOP_RIBBON_HEIGHT 35

#define UI_BUTTONS_X_POS 0
#define UI_BUTTONS_Y_POS 285
#define UI_BUTTONS_HEIGHT 35

#endif /* SRC_UI_VIEWS_BASIC_VIEW_LAYOUT_H_ */
