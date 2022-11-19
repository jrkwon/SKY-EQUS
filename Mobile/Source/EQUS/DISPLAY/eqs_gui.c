/*
** ===========================================================================
**
** Project: 
**     Graphics
**
** File:
**     eqs_gui.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-09-19 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

/* ------------------------------------------------------------------------- *

  EQS Theme ....

  1. GUI_THEME
  2. Clock (digital or analog)
  3. Bell sound
  4. Picture setting
  5. Sound effect setting

 * ------------------------------------------------------------------------- */

#define SB_MIN_SIZE        5

ROM COLOR_SCHEME_T  g_aColorScheme[] = {
    /* 
    **  3D color defintion
    **
    **  +-----------------------++  <-- WHITE
    **  |                  <----||----- LIGHT GRAY  
    **  +=======================++  <-- GRAY
    */

    /* text background - WHITE */
    /* CS_STANDARD     */
    { 
        { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(86,120,218) }, // caption color
        { BLACK, WHITE }, // text color
        { WHITE, BLACK }, // selected item color
        { BLACK, WHITE }, // draw color 
        { // list color - popup
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { BLACK, WHITE, BLACK }, // 3D border
                { BLACK, WHITE } // text
            },
            { // disable
                { BLACK, WHITE, BLACK }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), BLACK, EQS_RGB(96,96,96) }, // 3D border
                { EQS_RGB(233,106,0), BLACK } // text
            },
            { // active
                { BLACK, BLACK, EQS_RGB(94,93,93) }, // 3D border
                { WHITE, BLACK } // text
            }
        }
    },
    /* CS_RED        */
    { 
        { EQS_RGB(255,134,129), EQS_RGB(255,87,48), EQS_RGB(217,40,0) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(255,87,48) }, // caption color
        { BLACK, WHITE }, // text color
        { WHITE, BLACK }, // selected item color
        { BLACK, WHITE }, // draw color 
        { // list color - popup
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { BLACK, WHITE, BLACK }, // 3D border
                { BLACK, WHITE } // text
            },
            { // disable
                { BLACK, WHITE, BLACK }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), BLACK, EQS_RGB(96,96,96) }, // 3D border
                { EQS_RGB(233,106,0), BLACK } // text
            },
            { // active
                { BLACK, BLACK, EQS_RGB(94,93,93) }, // 3D border
                { WHITE, BLACK } // text
            }
        }
    },
    /* CS_GREEN    */
    { 
        { EQS_RGB(122,233,117), EQS_RGB(63,209,56), EQS_RGB(8,175,0) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(63,209,56) }, // caption color
        { BLACK, WHITE }, // text color
        { WHITE, BLACK }, // selected item color
        { BLACK, WHITE }, // draw color 
        { // list color - popup
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { BLACK, WHITE, BLACK }, // 3D border
                { BLACK, WHITE } // text
            },
            { // disable
                { BLACK, WHITE, BLACK }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), BLACK, EQS_RGB(96,96,96) }, // 3D border
                { EQS_RGB(233,106,0), BLACK } // text
            },
            { // active
                { BLACK, BLACK, EQS_RGB(94,93,93) }, // 3D border
                { WHITE, BLACK } // text
            }
        }
    },
    /* CS_PURPLE       */
    { 
        { EQS_RGB(243,136,226), EQS_RGB(252,20,215), EQS_RGB(194,2,165) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(252,20,215) }, // caption color
        { BLACK, WHITE }, // text color
        { WHITE, BLACK }, // selected item color
        { BLACK, WHITE }, // draw color 
        { // list color - popup
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { BLACK, WHITE, BLACK }, // 3D border
                { BLACK, WHITE } // text
            },
            { // disable
                { BLACK, WHITE, BLACK }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), BLACK, EQS_RGB(96,96,96) }, // 3D border
                { EQS_RGB(233,106,0), BLACK } // text
            },
            { // active
                { BLACK, BLACK, EQS_RGB(94,93,93) }, // 3D border
                { WHITE, BLACK } // text
            }
        }
    },
    /* CS_MARIN      */
    { 
        { EQS_RGB(139,81,175), EQS_RGB(97,39,134), EQS_RGB(64,25,88) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(97,39,134) }, // caption color
        { BLACK, WHITE }, // text color
        { WHITE, BLACK }, // selected item color
        { BLACK, WHITE }, // draw color 
        { // list color - popup
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { BLACK, WHITE, BLACK }, // 3D border
                { BLACK, WHITE } // text
            },
            { // disable
                { BLACK, WHITE, BLACK }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), BLACK, EQS_RGB(96,96,96) }, // 3D border
                { EQS_RGB(233,106,0), BLACK } // text
            },
            { // active
                { BLACK, BLACK, EQS_RGB(94,93,93) }, // 3D border
                { WHITE, BLACK } // text
            }
        }
    }, 
    /* CS_ORANGE       */
    { 
        { EQS_RGB(255,178,106), EQS_RGB(255,144,37), EQS_RGB(232,112,0) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(255,144,37) }, // caption color
        { BLACK, WHITE }, // text color
        { WHITE, BLACK }, // selected item color
        { BLACK, WHITE }, // draw color 
        { // list color - popup
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { BLACK, BLACK, BLACK }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, WHITE }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { BLACK, WHITE, BLACK }, // 3D border
                { BLACK, WHITE } // text
            },
            { // disable
                { BLACK, WHITE, BLACK }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), BLACK, EQS_RGB(96,96,96) }, // 3D border
                { EQS_RGB(233,106,0), BLACK } // text
            },
            { // active
                { BLACK, BLACK, EQS_RGB(94,93,93) }, // 3D border
                { WHITE, BLACK } // text
            }
        }
    },
    /* text background - BLACK */
    /* CS_STANDARD     */
    { 
        { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(86,120,218) }, // caption color
        { WHITE, BLACK }, // text color
        { WHITE, EQS_RGB(233,106,0) }, // selected item color
        { WHITE, BLACK }, // draw color 
        { // list color - popup
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(141,165,235), EQS_RGB(86,120,218), EQS_RGB(19,25,217) }, // border of caption
                { WHITE, EQS_RGB(86,120,218) } // text
            },
            { BLACK, EQS_RGB(255,195,29) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(255,178,106), EQS_RGB(255,144,37), EQS_RGB(232,112,0) }, // border of caption
                { WHITE, EQS_RGB(255,144,37) } // text
            },
            { BLACK, EQS_RGB(255,195,29) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(255,144,37), BLACK } // text
            },
            { // disable
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), EQS_RGB(255,144,37), EQS_RGB(96,96,96) }, // 3D border
                { WHITE, EQS_RGB(255,144,37) } // text
            },
            { // active
                { WHITE, EQS_RGB(255,144,37), EQS_RGB(94,93,93) }, // 3D border
                { WHITE, EQS_RGB(255,144,37) } // text
            }
        }
    },
    /* CS_RED        */
    { 
        { EQS_RGB(255,134,129), EQS_RGB(255,87,48), EQS_RGB(217,40,0) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(255,87,48) }, // caption color
        { WHITE, BLACK }, // text color
        { WHITE, EQS_RGB(233,106,0) }, // selected item color
        { WHITE, BLACK }, // draw color 
        { // list color - popup
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(255,134,129), EQS_RGB(255,87,48), EQS_RGB(217,40,0) }, // border of caption
                { WHITE, EQS_RGB(255,87,48) } // text
            },
            { BLACK, EQS_RGB(255,195,29) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(255,178,106), EQS_RGB(255,144,37), EQS_RGB(232,112,0) }, // border of caption
                { WHITE, EQS_RGB(255,144,37) } // text
            },
            { BLACK, EQS_RGB(255,195,29) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(255,144,37), BLACK } // text
            },
            { // disable
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), EQS_RGB(255,144,37), EQS_RGB(96,96,96) }, // 3D border
                { WHITE, EQS_RGB(255,144,37) } // text
            },
            { // active
                { WHITE, EQS_RGB(255,144,37), EQS_RGB(94,93,93) }, // 3D border
                { WHITE, EQS_RGB(255,144,37) } // text
            }
        }
    },
    /* CS_GREEN    */
    { 
        { EQS_RGB(122,233,117), EQS_RGB(63,209,56), EQS_RGB(8,175,0) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(63,209,56) }, // caption color
        { WHITE, BLACK }, // text color
        { WHITE, EQS_RGB(233,106,0) }, // selected item color
        { WHITE, BLACK }, // draw color 
        { // list color - popup
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(122,233,117), EQS_RGB(63,209,56), EQS_RGB(8,175,0) }, // border of caption
                { WHITE, EQS_RGB(63,209,56) } // text
            },
            { BLACK, EQS_RGB(255,195,29) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(255,178,106), EQS_RGB(255,144,37), EQS_RGB(232,112,0) }, // border of caption
                { WHITE, EQS_RGB(255,144,37) } // text
            },
            { BLACK, EQS_RGB(255,195,29) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(255,144,37), BLACK } // text
            },
            { // disable
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), EQS_RGB(255,144,37), EQS_RGB(96,96,96) }, // 3D border
                { WHITE, EQS_RGB(255,144,37) } // text
            },
            { // active
                { WHITE, EQS_RGB(255,144,37), EQS_RGB(94,93,93) }, // 3D border
                { WHITE, EQS_RGB(255,144,37) } // text
            }
        }
    },
    /* CS_PURPLE       */
    { 
        { EQS_RGB(243,136,226), EQS_RGB(252,20,215), EQS_RGB(194,2,165) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(252,20,215) }, // caption color
        { WHITE, BLACK }, // text color
        { WHITE, EQS_RGB(119,25,217) }, // selected item color
        { WHITE, BLACK }, // draw color 
        { // list color - popup
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(243,136,226), EQS_RGB(252,20,215), EQS_RGB(194,2,165) }, // border of caption
                { WHITE, EQS_RGB(252,20,215) } // text
            },
            { BLACK, EQS_RGB(183,179,255) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(202,165,240), EQS_RGB(119,25,217), EQS_RGB(64,25,88) }, // border of caption
                { WHITE, EQS_RGB(119,25,217) } // text
            },
            { BLACK, EQS_RGB(183,179,255) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(207,86,255), BLACK } // text
            },
            { // disable
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), EQS_RGB(119,25,217), EQS_RGB(96,96,96) }, // 3D border
                { WHITE, EQS_RGB(119,25,217) } // text
            },
            { // active
                { WHITE, EQS_RGB(119,25,217), EQS_RGB(94,93,93) }, // 3D border
                { WHITE, EQS_RGB(119,25,217) } // text
            }
        }
    },
    /* CS_MARIN      */
    { 
        { EQS_RGB(139,81,175), EQS_RGB(97,39,134), EQS_RGB(64,25,88) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(97,39,134) }, // caption color
        { WHITE, BLACK }, // text color
        { WHITE, EQS_RGB(119,25,217) }, // selected item color
        { WHITE, BLACK }, // draw color 
        { // list color - popup
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(139,81,175), EQS_RGB(97,39,134), EQS_RGB(64,25,88) }, // border of caption
                { WHITE, EQS_RGB(97,39,134) } // text
            },
            { BLACK, EQS_RGB(183,179,255) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(202,165,240), EQS_RGB(119,25,217), EQS_RGB(64,25,88) }, // border of caption
                { WHITE, EQS_RGB(119,25,217) } // text
            },
            { BLACK, EQS_RGB(183,179,255) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(207,86,255), BLACK } // text
            },
            { // disable
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), EQS_RGB(119,25,217), EQS_RGB(96,96,96) }, // 3D border
                { WHITE, EQS_RGB(119,25,217) } // text
            },
            { // active
                { WHITE, EQS_RGB(119,25,217), EQS_RGB(94,93,93) }, // 3D border
                { WHITE, EQS_RGB(119,25,217) } // text
            }
        }
    }, 
    /* CS_ORANGE       */
    { 
        { EQS_RGB(255,178,106), EQS_RGB(255,144,37), EQS_RGB(232,112,0) }, // 3D color : WHITE, LIGHTGRAY, GRAY
        { WHITE, EQS_RGB(255,144,37) }, // caption color
        { WHITE, BLACK }, // text color
        { WHITE, EQS_RGB(119,25,217) }, // selected item color
        { WHITE, BLACK }, // draw color 
        { // list color - popup
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(255,178,106), EQS_RGB(255,144,37), EQS_RGB(232,112,0) }, // border of caption
                { WHITE, EQS_RGB(255,144,37) } // text
            },
            { BLACK, EQS_RGB(183,179,255) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // list color - scroll list
            { WHITE, WHITE, WHITE }, // border of list box
            { // caption
                { EQS_RGB(202,165,240), EQS_RGB(119,25,217), EQS_RGB(64,25,88) }, // border of caption
                { WHITE, EQS_RGB(119,25,217) } // text
            },
            { BLACK, EQS_RGB(183,179,255) }, // normal
            { WHITE, BLACK }  // select
        }, 
        { // control color
            { // normal
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(207,86,255), BLACK } // text
            },
            { // disable
                { EQS_RGB(142,141,141), BLACK, EQS_RGB(142,141,141) }, // 3D border
                { EQS_RGB(94,93,93), WHITE } // text
            },
            { // select
                { EQS_RGB(96,96,96), EQS_RGB(119,25,217), EQS_RGB(96,96,96) }, // 3D border
                { WHITE, EQS_RGB(119,25,217) } // text
            },
            { // active
                { WHITE, EQS_RGB(119,25,217), EQS_RGB(94,93,93) }, // 3D border
                { WHITE, EQS_RGB(119,25,217) } // text
            }
        }
    }
};

GUI_THEME_T     g_GUI_Theme;

void EQS_API EQS_SetTheme(COLOR_SCHEME_E ColorScheme, FONT_TYPE FontType,
                         INT8 nScrollBarSize)
{
    g_GUI_Theme.ColorScheme = ColorScheme;
    g_GUI_Theme.FontType = FontType;
    
    SetForeColor(TEXT_FORE_COLOR); /* use text color as the foreground color */
    SetBackColor(BACK_COLOR);//R_GRAY);//LIGHTGRAY);
    SetTextColor(TEXT_FORE_COLOR);
    SetTextBackColor(TEXT_BACK_COLOR);
    SetFontType(FontType);
    
    EQS_SetForeColor(TEXT_FORE_COLOR);
    EQS_SetBackColor(BACK_COLOR);//R_GRAY);//R_LIGHTGRAY);
    EQS_SetTextColor(TEXT_FORE_COLOR);
    EQS_SetTextBackColor(TEXT_BACK_COLOR);
    EQS_SetFontType(FontType);

    g_GUI_Theme.nScrollBarSize 
        = nScrollBarSize < SB_MIN_SIZE ? SB_MIN_SIZE : nScrollBarSize;
}

COLOR_SCHEME_E EQS_API EQS_GetCurrentColorScheme(void)
{
    return g_GUI_Theme.ColorScheme;
}

void EQS_API EQS_SetCurrentColorScheme(COLOR_SCHEME_E ColorScheme)
{
    g_GUI_Theme.ColorScheme = ColorScheme;
}

FONT_TYPE EQS_API EQS_GetCurrentFontType(void)
{
    return g_GUI_Theme.FontType;
}

void EQS_API EQS_SetCurrentFontType(FONT_TYPE FontType)
{
    g_GUI_Theme.FontType = FontType;
}

INT8 EQS_API EQS_GetCurrentScrollSize(void)
{
    return g_GUI_Theme.nScrollBarSize;
}

void EQS_API EQS_SetCurrentScrollSize(INT8 nScrollBarSize)
{
    g_GUI_Theme.nScrollBarSize 
        = nScrollBarSize < SB_MIN_SIZE ? SB_MIN_SIZE : nScrollBarSize;
}

/*
COLOR EQS_API EQS_GetCurrentTextColor(void)
{
    return g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Fore;
}

void EQS_API EQS_SetCurrentTextColor(COLOR TextColor)
{
    g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Fore = TextColor;
}

COLOR EQS_API EQS_GetCurrentBackColor(void)
{
    return g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Back;
}

void EQS_API EQS_SetCurrentBackColor(COLOR BackColor)
{
    g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Back = BackColor;
}
*/

//#define R_WHITE      g_aColorScheme[g_GUI_Theme.ColorScheme].ThreeDColor.White
//#define R_LIGHTGRAY  g_aColorScheme[g_GUI_Theme.ColorScheme].ThreeDColor.LightGray
//#define R_GRAY       g_aColorScheme[g_GUI_Theme.ColorScheme].ThreeDColor.Gray
//#define R_BLACK      BLACK

//#define CAPTION_FORE_COLOR    g_aColorScheme[g_GUI_Theme.ColorScheme].CaptionColor.Fore
//#define CAPTION_BACK_COLOR    g_aColorScheme[g_GUI_Theme.ColorScheme].CaptionColor.Back

//#define TEXT_FORE_COLOR       g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Fore
//#define TEXT_BACK_COLOR       g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Back

/*
#define R_LIGHTGRAY       LIGHTGRAY
#define R_WHITE           WHITE    
#define R_GRAY        GRAY 
#define R_BLACK           BLACK    
*/

void EQS_API EQS_PreOpenWindow(WINDOW *pWin, WINDOW_STYLE Style, 
//                               WINDOW_CAPTION *pCaption)
                               BYTE *pszCaption)
//                             BYTE* pszTitle, COLOR CaptionBarColor,
//                               COLOR CaptionTextColor)
{
    pWin->Style = Style;
//    if(Style == WS_CAPTION && pCaption)
    if(Style & WS_CAPTION && pszCaption)
    {
        //pWin->Caption = *pCaption;
        pWin->pszCaption = pszCaption;
    }
    //pWin->Caption.pszTitle = pszTitle;
    //pWin->Caption.BarColor = CaptionBarColor;
    //pWin->Caption.TextColor = CaptionTextColor;
}

#define sBORDER_WIDTH     1

LOCAL INT8 l_nCaptionHeight = 0;

/* must call before calling EQS_OpenWindow */
void EQS_API EQS_SetCaptionHeight(WINDOW *pWin, INT8 nCaptionHeight)
{
    l_nCaptionHeight = nCaptionHeight;
}

INT8 EQS_API EQS_GetCaptionHeight(WINDOW *pWin)
{
    return l_nCaptionHeight;
}

EQS_RESULT EQS_API EQS_OpenWindow(WINDOW *pWin, RECT *pRect, BOOL fBackSave)
{
    VIEW_PORT OldViewPort;
    COLOR OldForeColor, OldTextColor, OldBackColor;
    DRAW_MODE OldDrawMode;
    TEXT_ALIGN OldTextAlign;
    INT8 nCaptionHeight = 0;
    RECT FullScreenRect;
    FONT_TYPE OldFontType;

    FullScreenRect.xLeft = 0;
    FullScreenRect.yTop = 0;
    FullScreenRect.xRight = DevGetMaxX();
    FullScreenRect.yBottom = DevGetMaxY();

    /* save the old viewport rect */
    GetViewPort(&OldViewPort);
    OldForeColor = GetForeColor();
    OldTextColor = GetTextColor();
    OldBackColor = GetBackColor();
    OldDrawMode = GetDrawMode();
    GetTextAlign(&OldTextAlign);
    OldFontType = GetFontType();

    /* set the viewport size as a full screen */
    ResetViewPort();
    SetDrawMode(DM_COPY);
    SetFontType(EQS_GetFontType());

    /* if full screen window */
    if(pWin->Style & WS_FULLSCREEN)
    {
        pWin->Frame = FullScreenRect;

        if(pWin->Style & WS_CAPTION)
        {
//            nCaptionHeight = TextHeight(pWin->Caption.pszTitle)
            if(l_nCaptionHeight == 0)
                nCaptionHeight = TextHeight(pWin->pszCaption)
                                 + TEXT_HEIGHT_UPPER_SPACE
                                 + TEXT_HEIGHT_LOWER_SPACE;
            else
                nCaptionHeight = l_nCaptionHeight;

            FullScreenRect.yTop += nCaptionHeight + 2;
        }
        if(pWin->Style & WS_BORDER)
        {
            FullScreenRect.xLeft += sBORDER_WIDTH;
            FullScreenRect.yTop += sBORDER_WIDTH; 
            FullScreenRect.xRight -= sBORDER_WIDTH;
            FullScreenRect.yBottom -= sBORDER_WIDTH;
        }
        /* ignore the pRect argument */
        pRect = &FullScreenRect;
    }

    if(pWin->Style & WS_CAPTION)
    {
//        nCaptionHeight = TextHeight(pWin->Caption.pszTitle)
        if(l_nCaptionHeight == 0)
            nCaptionHeight = TextHeight(pWin->pszCaption)
                             + TEXT_HEIGHT_UPPER_SPACE
                             + TEXT_HEIGHT_LOWER_SPACE;
        else
            nCaptionHeight = l_nCaptionHeight;

        pWin->Frame.yTop = pRect->yTop
                           - nCaptionHeight - sBORDER_WIDTH; /* caption border */
    }
    if(pWin->Style & WS_BORDER)
    {
        pWin->Frame.xLeft = pRect->xLeft - sBORDER_WIDTH;
        if(pWin->Style & WS_CAPTION)
            pWin->Frame.yTop -= sBORDER_WIDTH;
        else
            pWin->Frame.yTop = pRect->yTop - sBORDER_WIDTH;
        pWin->Frame.xRight = pRect->xRight + sBORDER_WIDTH;
        pWin->Frame.yBottom = pRect->yBottom + sBORDER_WIDTH;
    }
    //else if(pWin->Style == WS_NOBORDER)
    else if(!(pWin->Style & WS_FULLSCREEN))
        pWin->Frame = *pRect;

    if(fBackSave)
    {
        /* momory allocation for the area background is saved */
        GetRegionImageBufferRect(&(pWin->Frame), &(pWin->pbBackImage));
        /* check if the memory allocation is successful */
        if(pWin->pbBackImage == NULL)
            return RESULT_OUT_OF_MEMORY;
        /* get the background image to the buffer allocated */
        GetRegionImageRect(&(pWin->Frame), pWin->pbBackImage);
    }
    else
        pWin->pbBackImage = NULL; /* clear the pointer */

    pWin->Header.Type = OT_WINDOW;
    pWin->Header.pNext = NULL;

    /* fill the area of the window */
    FillRectangle(pRect->xLeft, pRect->yTop,
    //FillRectangle((COORD)(pRect->xLeft-1), (COORD)(pRect->yTop-1),
                  (COORD)(pRect->xRight), (COORD)(pRect->yBottom), POPUP_NORMAL_BACK_COLOR);//(COLOR)R_LIGHTGRAY);
    /* draw border */
    //if(pWin->Style == WS_BORDER || pWin->Style == WS_CAPTION)
    if(pWin->Style & WS_BORDER)
    {
        //SetForeColor(R_LIGHTGRAY);
        SetForeColor(WHITE);//(COLOR)255);
        Line(pWin->Frame.xLeft, pWin->Frame.yTop, pWin->Frame.xRight, pWin->Frame.yTop);
        Line(pWin->Frame.xLeft, pWin->Frame.yTop, pWin->Frame.xLeft, pWin->Frame.yBottom);
#if sBORDER_WIDTH == 2
        SetForeColor(R_WHITE);
        Line((COORD)(pWin->Frame.xLeft+1), (COORD)(pWin->Frame.yTop+1), 
             (COORD)(pWin->Frame.xRight-1), (COORD)(pWin->Frame.yTop+1));
        Line((COORD)(pWin->Frame.xLeft+1), (COORD)(pWin->Frame.yTop+1), 
             (COORD)(pWin->Frame.xLeft+1), (COORD)(pWin->Frame.yBottom-1));
#endif
        SetForeColor(BLACK);//(COLOR)0);//R_BLACK);
        Line(pWin->Frame.xLeft, pWin->Frame.yBottom, 
             pWin->Frame.xRight, pWin->Frame.yBottom);
        Line(pWin->Frame.xRight, pWin->Frame.yTop, 
             pWin->Frame.xRight, pWin->Frame.yBottom);
#if sBORDER_WIDTH == 2
        SetForeColor(R_GRAY);
        Line((COORD)(pWin->Frame.xLeft+1), (COORD)(pWin->Frame.yBottom-1), 
             (COORD)(pWin->Frame.xRight-1), (COORD)(pWin->Frame.yBottom-1));
        Line((COORD)(pWin->Frame.xRight-1), (COORD)(pWin->Frame.yTop+1), 
             (COORD)(pWin->Frame.xRight-1), (COORD)(pWin->Frame.yBottom-1));
#endif
    }

    /* draw caption */
    //if(pWin->Style == WS_CAPTION)
    if(pWin->Style & WS_CAPTION)
    {
        INT8 nBorder = 0; /* caption border */

        if(pWin->Style & WS_BORDER)
            nBorder = sBORDER_WIDTH;//2;
        SetForeColor(R_LIGHTGRAY);
        /*Rectangle((COORD)(pWin->Frame.xLeft+nBorder), (COORD)(pWin->Frame.yTop+nBorder),
                  (COORD)(pWin->Frame.xRight-nBorder), 
                  (COORD)(pWin->Frame.yTop+nBorder+nCaptionHeight+1));*/
        FillRectangle((COORD)(pWin->Frame.xLeft+nBorder), (COORD)(pWin->Frame.yTop+nBorder),
                      (COORD)(pWin->Frame.xRight-(nBorder)), 
                      (COORD)(pWin->Frame.yTop+(nBorder)+nCaptionHeight),//-1),
                      POPUP_CAPTION_TEXT_BACK_COLOR);//CAPTION_BACK_COLOR);
//                  pWin->Caption.BarColor);
        SetTextColor(POPUP_CAPTION_TEXT_FORE_COLOR);//CAPTION_FORE_COLOR);
        SetBackColor(POPUP_CAPTION_TEXT_BACK_COLOR);//(COLOR)CAPTION_BACK_COLOR); //179);
//        SetTextColor(pWin->Caption.TextColor);
        SetDrawMode(DM_OR);
        TextOutStr((COORD)(pWin->Frame.xLeft 
                           + (pWin->Frame.xRight - pWin->Frame.xLeft)/2 
//                           - TextWidth(pWin->Caption.pszTitle)/2), 
                           - TextWidth(pWin->pszCaption)/2), 
                   (COORD)(pWin->Frame.yTop+(nBorder)+(nCaptionHeight-CharHeight('A'))/2),//TEXT_HEIGHT_UPPER_SPACE),
//                    pWin->Caption.pszTitle);
                    pWin->pszCaption);
        /*Rectangle((COORD)(pWin->Frame.xLeft+2), (COORD)(pWin->Frame.yTop+2),
                  (COORD)(pWin->Frame.xRight-2), 
                  (COORD)(pWin->Frame.yTop+2+nCaptionHeight+1));
        FillRectangle((COORD)(pWin->Frame.xLeft+3), (COORD)(pWin->Frame.yTop+3),
                  (COORD)(pWin->Frame.xRight-3), 
                  (COORD)(pWin->Frame.yTop+3+nCaptionHeight-1),
                  pWin->Caption.BarColor);
        SetTextColor(pWin->Caption.TextColor);
        SetDrawMode(DM_OR);
        TextOutStr((COORD)(pWin->Frame.xLeft 
                           + (pWin->Frame.xRight - pWin->Frame.xLeft)/2 
                           - TextWidth(pWin->Caption.pszTitle)/2), 
                   (COORD)(pWin->Frame.yTop+3+TEXT_HEIGHT_UPPER_SPACE),
                    pWin->Caption.pszTitle);*/
    }
    //SetViewPortRect(pRect);

    /* just for test...
    SetTextAlignHoriz(TAH_CENTER);
    SetTextWrap(TRUE);
    SetDrawMode(DM_OR);
    SetTextColor(GRAY);
    TextOutStr((COORD)((pRect->xRight - pRect->xLeft)/2)+1, 1, "메시지 박스");// 예제입니다. 하하 멋있지");
    SetTextColor(WHITE);
    TextOutStr((COORD)((pRect->xRight - pRect->xLeft)/2), 0, "메시지 박스");// 예제입니다. 하하 멋있지");
    SetTextColor(BLUE);
    SetTextWrap(TRUE);
    TextOutStr(0, 0, "메시지 박스 예제입니다. 하하 멋있지");
    */

    /* restore the old viewport rect */
    SetViewPort(&OldViewPort);
    SetForeColor(OldForeColor);
    SetDrawMode(OldDrawMode);
    SetTextColor(OldTextColor);
    SetBackColor(OldBackColor);
    SetTextAlign(&OldTextAlign);
    SetFontType(OldFontType);

    return RESULT_OK;
}

void EQS_API EQS_CloseWindow(WINDOW *pWin)
{
    VIEW_PORT OldViewPort;

    /* save the old viewport rect */
    GetViewPort(&OldViewPort);
    ResetViewPort();

    if(pWin->pbBackImage != NULL)
    {
        PutRegionImage(pWin->Frame.xLeft, pWin->Frame.yTop, pWin->pbBackImage);
        EQS_Free((void**)&(pWin->pbBackImage));
    }

    /* restore the old viewport rect */
    SetViewPort(&OldViewPort);

    /* reset caption height */
    l_nCaptionHeight = 0;

    //EQS_ReleaseDC();
}

/* ***************************************************************************
** ScrollBar
*/ 
#define SB_WIDTH                g_GUI_Theme.nScrollBarSize //6
#define SB_HEIGHT               (SB_WIDTH+2)//   6
#define SB_COLOR                R_LIGHTGRAY
#define SB_BORDER_COLOR         R_GRAY
#define SB_THUMB_MIN_SIZE       5

//void EQS_API EQS_AddScrollBar(WINDOW* pWin, SCROLL_BAR* pScrollBar)
void EQS_API EQS_AddScrollBar(WINDOW* pWin, SCROLL_BAR* pScrollBar,
                              SCROLL_BAR_TYPE Type,
                              COORD x, COORD y, INT16 nLength,
                              INT16 nMin, INT16 nMax, INT16 nPageSize, 
                              INT16 nPos)
{
    OBJECT_HEADER* pHeader;

    if(pWin)
    {
        //ASSERT(pWin);
        pHeader = &(pWin->Header);
        while(pHeader->pNext != NULL)
            pHeader = (OBJECT_HEADER*)pHeader->pNext;

        pHeader->pNext = pScrollBar;
    }
    pScrollBar->Header.pNext = NULL;

    pScrollBar->Info.nMin = nMin;
    pScrollBar->Info.nMax = nMax;
    pScrollBar->Info.nPos = nPos;
    pScrollBar->Info.nPageSize = nPageSize;
    pScrollBar->Frame.xLeft = x;
    pScrollBar->Frame.yTop = y;
    
    if(Type == SBT_VERT)
    {
        pScrollBar->Header.Type = OT_VSCROLL;
        pScrollBar->Frame.xRight = x + SB_WIDTH - 1;
        pScrollBar->Frame.yBottom = y + nLength - 1;
    }
    else
    {
        pScrollBar->Header.Type = OT_HSCROLL;
        pScrollBar->Frame.xRight = x + nLength - 1;
        pScrollBar->Frame.yBottom = y + SB_HEIGHT - 1;
    }
}

//void EQS_API sDrawScrollButton(COORD xLeft, COORD yTop, 
//                               COORD xRight, COORD yBottom)
/*void EQS_API EQS_Draw3DBox(COORD xLeft, COORD yTop, 
                            COORD xRight, COORD yBottom)
{
    EQS_SetForeColor(R_LIGHTGRAY); 
    EQS_Line(xLeft, yTop, xRight, yTop);
    EQS_Line(xLeft, yTop, xLeft, yBottom);
    EQS_SetForeColor(R_WHITE);
    EQS_Line((COORD)(xLeft+1), (COORD)(yTop+1),
             (COORD)(xRight-1), (COORD)(yTop+1));
    EQS_Line((COORD)(xLeft+1), (COORD)(yTop+1),
             (COORD)(xLeft+1), (COORD)(yBottom-1));
    EQS_SetForeColor(R_BLACK);
    EQS_Line(xLeft, yBottom, xRight, yBottom);
    EQS_Line(xRight, yTop, xRight, yBottom);
    EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(xLeft+1), (COORD)(yBottom-1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));
    EQS_Line((COORD)(xRight-1), (COORD)(yTop+1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));
//    EQS_Rectangle((COORD)(xLeft+2), (COORD)(yTop+2), 
//                  (COORD)(xRight-2), (COORD)(yBottom-2));
    EQS_FillRectangle((COORD)(xLeft+2), (COORD)(yTop+2), 
                      (COORD)(xRight-2), (COORD)(yBottom-2), R_LIGHTGRAY);
}*/

LOCAL void EQS_API sDraw3DBox(COORD xLeft, COORD yTop, 
                            COORD xRight, COORD yBottom, COLOR Color)
{
    EQS_SetForeColor((COLOR)EQS_RGB(160,160,128));//182);//R_LIGHTGRAY);//R_WHITE);
    EQS_Line(xLeft, yTop, xRight, yTop);
    EQS_Line(xLeft, yTop, xLeft, yBottom);

    EQS_SetForeColor(BLACK);//(COLOR)0);//R_GRAY);//R_BLACK);
    EQS_Line(xLeft, yBottom, xRight, yBottom);
    EQS_Line(xRight, yTop+1, xRight, yBottom);
    /*EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(xLeft+1), (COORD)(yBottom-1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));
    EQS_Line((COORD)(xRight-1), (COORD)(yTop+1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));*/
    EQS_FillRectangle((COORD)(xLeft+1), (COORD)(yTop+1), 
                      (COORD)(xRight-1), (COORD)(yBottom-1), Color ); //R_WHITE);//R_LIGHTGRAY);
}

void EQS_API EQS_Draw3DBox(COORD xLeft, COORD yTop, 
                            COORD xRight, COORD yBottom)
{
    COLOR OldColor;

    OldColor = EQS_GetForeColor();

    //EQS_FillRectangle((COORD)(xLeft-5), (COORD)(yTop-5), 
    //                  (COORD)(xRight+5), (COORD)(yBottom+5), R_LIGHTGRAY);

    /*EQS_SetForeColor(R_LIGHTGRAY);//R_WHITE);
    EQS_Line(xLeft, yTop, xRight, yTop);
    EQS_Line(xLeft, yTop, xLeft, yBottom);*/

    EQS_SetForeColor((COLOR)EQS_RGB(160,160,128));//182);//R_WHITE);//R_BLACK);
    EQS_Line((COORD)(xLeft), (COORD)(yTop), (COORD)(xRight), (COORD)(yTop));
    EQS_Line((COORD)(xLeft), (COORD)(yTop), (COORD)(xLeft), (COORD)(yBottom));

    EQS_SetForeColor(BLACK);//(COLOR)0);//R_BLACK);
    EQS_Line(xLeft+1, yBottom, xRight, yBottom);
    EQS_Line(xRight, yTop+1, xRight, yBottom);

    /*EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(xLeft+1), (COORD)(yBottom-1), 
         (COORD)(xRight-1), (COORD)(yBottom-1));
    EQS_Line((COORD)(xRight-1), (COORD)(yTop+1), 
         (COORD)(xRight-1), (COORD)(yBottom-1));*/

    //EQS_SetForeColor(R_BLACK);
    //EQS_Rectangle(xLeft, yTop, xRight, yBottom);
    
    EQS_SetForeColor(OldColor);

    /*EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(xLeft+1), (COORD)(yBottom-1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));
    EQS_Line((COORD)(xRight-1), (COORD)(yTop+1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));*/
}


void EQS_API EQS_DrawScrollBar(SCROLL_BAR* pScrollBar)
{
//    /* 6 x 6 .. 4 gray bitmap */
//    STATIC ROM BYTE abUpArrow[] = { /* up and right */
//        0xAA, 0x8F, 0xF4, 0xB2, 0x4B, 0x04, 0x95, 0x40, 0x00 
//   };
//    STATIC ROM BYTE abDownArrow[] = { /* down and left */
//        0xAA, 0x8F, 0xF4, 0xB0, 0x4B, 0x84, 0x95, 0x40, 0x00 
//    };
//    ROM BITMAP UpArrowImage = {
//        { BT_IMAGE, { 6, 6 }, EQS_NA, 2 }, abUpArrow
//    };
//    ROM BITMAP DownArrowImage = {
//        { BT_IMAGE, { 6, 6 }, EQS_NA, 2 }, abDownArrow
//    };
    COLOR OldForeColor;
    DRAW_MODE OldDrawMode;
    INT16 nThumbSize;
    INT16 nScrollSize;
    INT16 nRange;
    INT16 nPos;
    COORD yTop, yBottom;
    COORD xLeft, xRight;

    OldDrawMode = EQS_GetDrawMode();
    EQS_SetDrawMode(DM_COPY);

    /* draw frame */
/*    EQS_SetForeColor(R_LIGHTGRAY); 
    EQS_Line(pScrollBar->Frame.xLeft, yTop, pScrollBar->Frame.xRight, yTop);
    EQS_Line(pScrollBar->Frame.xLeft, yTop, pScrollBar->Frame.xLeft, yBottom);
    EQS_SetForeColor(R_WHITE);
    EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yTop+1),
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yTop+1));
    EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yTop+1),
             (COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yBottom-1));
    EQS_SetForeColor(R_BLACK);
    EQS_Line(pScrollBar->Frame.xLeft, yBottom, pScrollBar->Frame.xRight, yBottom);
    EQS_Line(pScrollBar->Frame.xRight, yTop, pScrollBar->Frame.xRight, yBottom);
    EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yBottom-1), 
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yBottom-1));
    EQS_Line((COORD)(pScrollBar->Frame.xRight-1), (COORD)(yTop+1), 
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yBottom-1));

    yTop = pScrollBar->Frame.yBottom - SB_HEIGHT + 1;
    yBottom = pScrollBar->Frame.yBottom;

    EQS_SetForeColor(R_LIGHTGRAY); 
    EQS_Line(pScrollBar->Frame.xLeft, yTop, pScrollBar->Frame.xRight, yTop);
    EQS_Line(pScrollBar->Frame.xLeft, yTop, pScrollBar->Frame.xLeft, yBottom);
    EQS_SetForeColor(R_WHITE);
    EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yTop+1),
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yTop+1));
    EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yTop+1),
             (COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yBottom-1));
    EQS_SetForeColor(R_BLACK);
    EQS_Line(pScrollBar->Frame.xLeft, yBottom, pScrollBar->Frame.xRight, yBottom);
    EQS_Line(pScrollBar->Frame.xRight, yTop, pScrollBar->Frame.xRight, yBottom);
    EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yBottom-1), 
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yBottom-1));
    EQS_Line((COORD)(pScrollBar->Frame.xRight-1), (COORD)(yTop+1), 
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yBottom-1));
*/
    OldForeColor = EQS_GetForeColor();

    /* draw arrow button and fill the scrollbar area */
	//pScrollBar->Frame.xRight -= 2;
    if(pScrollBar->Header.Type == SBT_VERT)
    {	
        sDraw3DBox(pScrollBar->Frame.xLeft, pScrollBar->Frame.yTop,
                       pScrollBar->Frame.xRight + 1, 
                       (COORD)(pScrollBar->Frame.yTop + SB_HEIGHT - 1), R_WHITE);
        sDraw3DBox(pScrollBar->Frame.xLeft, 
                       (COORD)(pScrollBar->Frame.yBottom - SB_HEIGHT + 1),
                       pScrollBar->Frame.xRight + 1, 
                       pScrollBar->Frame.yBottom, R_WHITE);
        /* draw arrow */
        EQS_SetForeColor(SB_BORDER_COLOR); 
        /* up */
		//EQS_Line(pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yTop+1, pScrollBar->Frame.xRight-3, pScrollBar->Frame.yTop+1);
        EQS_Line(pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yTop+2, pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yTop+2);
        EQS_Line(pScrollBar->Frame.xLeft+2, pScrollBar->Frame.yTop+3, pScrollBar->Frame.xLeft+4, pScrollBar->Frame.yTop+3);
		EQS_Line(pScrollBar->Frame.xLeft+1, pScrollBar->Frame.yTop+4, pScrollBar->Frame.xLeft+5, pScrollBar->Frame.yTop+4);
        /* down */
		EQS_Line(pScrollBar->Frame.xLeft+1, pScrollBar->Frame.yBottom-4, pScrollBar->Frame.xLeft+5, pScrollBar->Frame.yBottom-4);
        EQS_Line(pScrollBar->Frame.xLeft+2, pScrollBar->Frame.yBottom-3, pScrollBar->Frame.xLeft+4, pScrollBar->Frame.yBottom-3);
        EQS_Line(pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yBottom-2, pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yBottom-2);
		//EQS_Line(pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yBottom-1, pScrollBar->Frame.xRight-3, pScrollBar->Frame.yBottom-1);

        //EQS_PutBitmap(pScrollBar->Frame.xLeft, pScrollBar->Frame.yTop, 
        //          &UpArrowImage);
        //EQS_PutBitmap(pScrollBar->Frame.xLeft, (COORD)(pScrollBar->Frame.yBottom
        //          - SB_HEIGHT + 1), &DownArrowImage);
        
        EQS_FillRectangle(pScrollBar->Frame.xLeft,
                          (COORD)(pScrollBar->Frame.yTop + SB_HEIGHT),
                          pScrollBar->Frame.xRight+1, 
                          (COORD)(pScrollBar->Frame.yBottom - SB_HEIGHT),
                          GRAY);//CAPTION_BACK_COLOR);//SB_COLOR);

    }
    else /* SBT_HORIZ */
    {
        /* ---------------------------------------
        ** SHOULD BE MODIFIED ... AS LIKE SBT_VERT 
        */

        sDraw3DBox(pScrollBar->Frame.xLeft, pScrollBar->Frame.yTop,
                       (COORD)(pScrollBar->Frame.xLeft + SB_WIDTH - 1), 
                       pScrollBar->Frame.yBottom, R_GRAY);
        sDraw3DBox((COORD)(pScrollBar->Frame.xRight - SB_WIDTH + 1), 
                       pScrollBar->Frame.yTop,
                       pScrollBar->Frame.xRight, 
                       pScrollBar->Frame.yBottom, R_GRAY);
        /* draw arrow */
        EQS_SetForeColor(SB_BORDER_COLOR); 
        /* left */
        EQS_Line(pScrollBar->Frame.xLeft+2, pScrollBar->Frame.yTop+2, pScrollBar->Frame.xLeft+2, pScrollBar->Frame.yBottom-2);
        EQS_Line(pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yTop+1, pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yBottom-1);
        /* right */
        EQS_Line(pScrollBar->Frame.xLeft+2, pScrollBar->Frame.yTop+1, pScrollBar->Frame.xLeft+2, pScrollBar->Frame.yBottom-1);
        EQS_Line(pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yTop+2, pScrollBar->Frame.xLeft+3, pScrollBar->Frame.yBottom-2);
        //EQS_PutBitmap(pScrollBar->Frame.xLeft, pScrollBar->Frame.yTop, 
        //          &DownArrowImage);
        //EQS_PutBitmap((COORD)(pScrollBar->Frame.xRight - SB_WIDTH + 1), 
        //          pScrollBar->Frame.yTop, &UpArrowImage);
        EQS_FillRectangle((COORD)(pScrollBar->Frame.xLeft + SB_WIDTH),
                          pScrollBar->Frame.yTop,
                          (COORD)(pScrollBar->Frame.xRight - SB_WIDTH), 
                          pScrollBar->Frame.yBottom,
                          SB_COLOR);
    }
    EQS_SetForeColor(SB_BORDER_COLOR);
//    if(pScrollBar->Header.Type == SBT_VERT)
//       EQS_Rectangle(pScrollBar->Frame.xLeft,
//                      (COORD)(pScrollBar->Frame.yTop + SB_HEIGHT),
//                      pScrollBar->Frame.xRight, 
//                      (COORD)(pScrollBar->Frame.yBottom - SB_HEIGHT));
//    else /* SBT_HORIZ */
//        EQS_Rectangle((COORD)(pScrollBar->Frame.xLeft + SB_WIDTH),
//                  pScrollBar->Frame.yTop,
//                  (COORD)(pScrollBar->Frame.xRight - SB_WIDTH), 
//                  pScrollBar->Frame.yBottom);

    /* get thumb size */
    if(pScrollBar->Header.Type == SBT_VERT)
        nScrollSize = (pScrollBar->Frame.yBottom - pScrollBar->Frame.yTop + 1
                       - SB_HEIGHT * 2);
    else /* SBT_HORIZ */
        nScrollSize = (pScrollBar->Frame.xRight - pScrollBar->Frame.xLeft + 1
                       - SB_WIDTH * 2);

    nRange = pScrollBar->Info.nMax - pScrollBar->Info.nMin + 1;

	//nThumbSize = (INT16)(nScrollSize * ((FLOAT)pScrollBar->Info.nPageSize/(pScrollBar->Info.nPageSize+nRange-1)));
	nThumbSize = (INT16)((nScrollSize * pScrollBar->Info.nPageSize)/(pScrollBar->Info.nPageSize+nRange-1));

    if(nThumbSize < SB_THUMB_MIN_SIZE)
        nThumbSize = SB_THUMB_MIN_SIZE;

    /* get the thumb position */
    nScrollSize -= nThumbSize;
    //nPos = (INT16)(nScrollSize * ((FLOAT)(pScrollBar->Info.nPos)/(pScrollBar->Info.nMax - pScrollBar->Info.nMin)));
	if( nRange > 1 )
	{
		nPos = (INT16)(nScrollSize * ((FLOAT)(pScrollBar->Info.nPos)/(nRange-1)));
	}
	else
	{
		nPos = (INT16)(nScrollSize * ((FLOAT)(pScrollBar->Info.nPos)/(nRange)));
	}

    /* draw thumb */
    yTop = pScrollBar->Frame.yTop + SB_HEIGHT + nPos;
    yBottom = yTop + nThumbSize - 1;
    
    xLeft = pScrollBar->Frame.xLeft + SB_WIDTH + nPos;
    xRight = xLeft + nThumbSize - 1;
    
    if(pScrollBar->Header.Type == SBT_VERT)
    {
        sDraw3DBox(pScrollBar->Frame.xLeft, yTop, pScrollBar->Frame.xRight+1, yBottom, WHITE);
		//EQS_FillRectangle(pScrollBar->Frame.xLeft+1, yTop, pScrollBar->Frame.xRight-1, yBottom, YELLOW);
        /*EQS_SetForeColor(R_LIGHTGRAY); 
        EQS_Line(pScrollBar->Frame.xLeft, yTop, pScrollBar->Frame.xRight, yTop);
        EQS_Line(pScrollBar->Frame.xLeft, yTop, pScrollBar->Frame.xLeft, yBottom);
        EQS_SetForeColor(R_WHITE);
        EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yTop+1),
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yTop+1));
        EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yTop+1),
             (COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yBottom-1));
        EQS_SetForeColor(R_BLACK);
        EQS_Line(pScrollBar->Frame.xLeft, yBottom, pScrollBar->Frame.xRight, yBottom);
        EQS_Line(pScrollBar->Frame.xRight, yTop, pScrollBar->Frame.xRight, yBottom);
        EQS_SetForeColor(R_GRAY);
        EQS_Line((COORD)(pScrollBar->Frame.xLeft+1), (COORD)(yBottom-1), 
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yBottom-1));
        EQS_Line((COORD)(pScrollBar->Frame.xRight-1), (COORD)(yTop+1), 
             (COORD)(pScrollBar->Frame.xRight-1), (COORD)(yBottom-1));*/
    }
    else /* SBT_HORIZ */
    {
        sDraw3DBox(xLeft, pScrollBar->Frame.yTop, xRight, pScrollBar->Frame.yBottom, WHITE);
        /*EQS_SetForeColor(R_LIGHTGRAY); 
        EQS_Line(xLeft, pScrollBar->Frame.yTop, xRight, pScrollBar->Frame.yTop);
        EQS_Line(xLeft, pScrollBar->Frame.yTop, xLeft, pScrollBar->Frame.yBottom);
        EQS_SetForeColor(R_WHITE);
        EQS_Line((COORD)(xLeft+1), (COORD)(pScrollBar->Frame.yTop+1),
             (COORD)(xRight-1), (COORD)(pScrollBar->Frame.yTop+1));
        EQS_Line((COORD)(xLeft+1), (COORD)(pScrollBar->Frame.yTop+1),
             (COORD)(xLeft+1), (COORD)(pScrollBar->Frame.yBottom-1));
        EQS_SetForeColor(R_BLACK);
        EQS_Line(xLeft, pScrollBar->Frame.yBottom, xRight, pScrollBar->Frame.yBottom);
        EQS_Line(xRight, pScrollBar->Frame.yTop, xRight, pScrollBar->Frame.yBottom);
        EQS_SetForeColor(R_GRAY);
        EQS_Line((COORD)(xLeft+1), (COORD)(pScrollBar->Frame.yBottom-1), 
             (COORD)(xRight-1), (COORD)(pScrollBar->Frame.yBottom-1));
        EQS_Line((COORD)(xRight-1), (COORD)(pScrollBar->Frame.yTop+1), 
             (COORD)(xRight-1), (COORD)(pScrollBar->Frame.yBottom-1));*/
    }

    /* restore old foreground color */
    EQS_SetForeColor(OldForeColor);
    EQS_SetDrawMode(OldDrawMode);
}

SCROLL_BAR_INFO* EQS_API EQS_GetScrollBarInfo(SCROLL_BAR* pScrollBar, 
                                  SCROLL_BAR_INFO* pInfo)
{
    *pInfo = pScrollBar->Info;
    return pInfo;
}

void EQS_API EQS_SetScrollBarInfo(SCROLL_BAR* pScrollBar, 
                                  SCROLL_BAR_INFO* pInfo, 
                                  BOOL fRedraw)
{
    pScrollBar->Info = *pInfo;
    if(fRedraw)
        EQS_DrawScrollBar(pScrollBar);
}

void EQS_API EQS_AddButton(WINDOW* pWin, BUTTON* pButton,
                           COORD x, COORD y, INT8 nWidth)
{
    OBJECT_HEADER* pHeader;

    if(pWin)
    {
        //ASSERT(pWin);
        pHeader = &(pWin->Header);
        while(pHeader->pNext != NULL)
            pHeader = (OBJECT_HEADER*)pHeader->pNext;

        pHeader->pNext = pButton;
    }
    pButton->Header.Type = OT_BUTTON;
    pButton->Header.pNext = NULL;

    pButton->Frame.xLeft = x;
    pButton->Frame.yTop = y;
    if(nWidth == BUTTON_WIDTH_AUTO)
        nWidth = (INT8)(EQS_TextWidth(pButton->pszText) + (BUTTON_INNER_SPACE << 2));
    pButton->Frame.xRight = x + nWidth - 1;
    pButton->Frame.yBottom = y + BUTTON_HEIGHT(pButton) - 1;
}


void EQS_API EQS_Draw3DButton(COORD xLeft, COORD yTop, 
                              COORD xRight, COORD yBottom, BOOL fFocus)
{
    COLOR ForeColor;
    DRAW_MODE DrawMode;

    DrawMode = EQS_GetDrawMode();
    ForeColor = EQS_GetForeColor();

    EQS_SetDrawMode(DM_COPY);

    EQS_SetForeColor((COLOR)EQS_RGB(160,160,128));//182);//R_WHITE);
    EQS_Line(xLeft, yTop, xRight, yTop);
    EQS_Line(xLeft, yTop, xLeft, yBottom);
    EQS_SetForeColor(BLACK);//(COLOR)0);//R_BLACK);
    EQS_Line(xLeft, yBottom, xRight, yBottom);
    EQS_Line(xRight, yTop, xRight, yBottom);
    /*EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(xLeft+1), (COORD)(yBottom-1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));
    EQS_Line((COORD)(xRight-1), (COORD)(yTop+1), 
             (COORD)(xRight-1), (COORD)(yBottom-1));*/
    if(fFocus)
        EQS_FillRectangle((COORD)(xLeft+1), (COORD)(yTop+1), 
              (COORD)(xRight-1), (COORD)(yBottom-1), CAPTION_BACK_COLOR);
    else
        EQS_FillRectangle((COORD)(xLeft+1), (COORD)(yTop+1), 
                          (COORD)(xRight-1), (COORD)(yBottom-1), R_LIGHTGRAY);
    EQS_SetForeColor(ForeColor);
    EQS_SetDrawMode(DrawMode);
}

void EQS_API EQS_DrawButton(BUTTON *pButton, BOOL fFocus)
{
    DRAW_MODE DrawMode;
    TEXT_ALIGN TextAlign;
    COLOR TextColor;

    /* save the old properties */
    DrawMode = EQS_GetDrawMode();
    EQS_GetTextAlign(&TextAlign);
    TextColor = EQS_GetTextColor();

    EQS_Draw3DButton(pButton->Frame.xLeft, pButton->Frame.yTop,
                     pButton->Frame.xRight, pButton->Frame.yBottom, fFocus);

    /* set the new properties */
    EQS_SetDrawMode(DM_OR);
    EQS_SetTextAlignHoriz(TAH_CENTER);
    EQS_SetTextAlignVert(TAV_CENTER);
    if(fFocus)
        EQS_SetTextColor(TEXT_BACK_COLOR);
    else
        EQS_SetTextColor(TEXT_FORE_COLOR);

    EQS_TextOutStrTDM((COORD)(pButton->Frame.xLeft
                              + ((pButton->Frame.xRight 
                                 - pButton->Frame.xLeft + 1)>>1)),
                      (COORD)(pButton->Frame.yTop 
                              + ((pButton->Frame.yBottom 
                                 - pButton->Frame.yTop + 1)>>1)),
                      pButton->pszText, 
                      (TEXT_DRAW_MODE)(pButton->fActive ? TDM_NORMAL : TDM_SHADOW));

    /* restore the original properties */
    EQS_SetTextAlign(&TextAlign);
    EQS_SetDrawMode(DrawMode);
    EQS_SetTextColor(TextColor);
}



void EQS_API EQS_Draw3DReverseBorder(COORD x, COORD y, COORD dx, COORD dy)
{
	COLOR OldColor;

	OldColor = EQS_GetForeColor();
    EQS_SetForeColor(BLACK);//(COLOR)0);
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(x-1), (COORD)(dy+1));
	EQS_SetForeColor((COLOR)EQS_RGB(160,160,128));//182);
	EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
	EQS_Line((COORD)(dx+1), (COORD)(y-1), (COORD)(dx+1), (COORD)(dy+1));
	/*
    EQS_SetForeColor(R_BLACK);
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(x-1), (COORD)(dy+1));
	EQS_SetForeColor(R_GRAY);
	EQS_Line((COORD)(x-2), (COORD)(y-2), (COORD)(dx+2), (COORD)(y-2));
	EQS_Line((COORD)(x-2), (COORD)(y-2), (COORD)(x-2), (COORD)(dy+2));
	EQS_SetForeColor(R_LIGHTGRAY);
	EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
	EQS_Line((COORD)(dx+1), (COORD)(y-1), (COORD)(dx+1), (COORD)(dy+1));
	EQS_SetForeColor(R_WHITE);
	EQS_Line((COORD)(x-2), (COORD)(dy+2), (COORD)(dx+2), (COORD)(dy+2));
	EQS_Line((COORD)(dx+2), (COORD)(y-2), (COORD)(dx+2), (COORD)(dy+2));
    */
	EQS_SetForeColor(OldColor);
}
void EQS_API EQS_Draw3DBorder(COORD x, COORD y, COORD dx, COORD dy)
{
	COLOR OldColor;

	OldColor = EQS_GetForeColor();
    EQS_SetForeColor((COLOR)EQS_RGB(160,160,128));
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(x-1), (COORD)(dy+1));
	EQS_SetForeColor(BLACK);//(COLOR)0);
	EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
	EQS_Line((COORD)(dx+1), (COORD)(y-1), (COORD)(dx+1), (COORD)(dy+1));
	/*
    EQS_SetForeColor(R_LIGHTGRAY);
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
	EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(x-1), (COORD)(dy+1));
	EQS_SetForeColor(R_BLACK);
	EQS_Line((COORD)(x-2), (COORD)(y-2), (COORD)(dx+2), (COORD)(y-2));
	EQS_Line((COORD)(x-2), (COORD)(y-2), (COORD)(x-2), (COORD)(dy+2));
	EQS_SetForeColor(R_GRAY);
	EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
	EQS_Line((COORD)(dx+1), (COORD)(y-1), (COORD)(dx+1), (COORD)(dy+1));
	EQS_SetForeColor(R_BLACK);
	EQS_Line((COORD)(x-2), (COORD)(dy+2), (COORD)(dx+2), (COORD)(dy+2));
	EQS_Line((COORD)(dx+2), (COORD)(y-2), (COORD)(dx+2), (COORD)(dy+2));
    */
	EQS_SetForeColor(OldColor);

}

void EQS_API EQS_DrawTitlePannel(void)
{
    BITMAP *pBitmap;

    switch(EQS_GetCurrentColorScheme())
    {
    case CS_STANDARD :
    case CS_STANDARD_BLACK :
        pBitmap = (BITMAP *)&g_BmpTitlePannelBlue;
        break;
    case CS_RED_WHITE :
    case CS_RED_BLACK :
        pBitmap = (BITMAP *)&g_BmpTitlePannelRed;
        break;
    case CS_GREEN_WHITE :
    case CS_GREEN_BLACK :
        pBitmap = (BITMAP *)&g_BmpTitlePannelGreen;
        break;
    case CS_PURPLE_WHITE :
    case CS_PURPLE_BLACK :
        pBitmap = (BITMAP *)&g_BmpTitlePannelViolet;
        break;
    case CS_MARIN_WHITE :
    case CS_MARIN_BLACK :
        pBitmap = (BITMAP *)&g_BmpTitlePannelDeepBlue;
        break;
    case CS_ORANGE_WHITE :
    case CS_ORANGE_BLACK :
        pBitmap = (BITMAP *)&g_BmpTitlePannelBrown;
        break;
    }

	EQS_PutBitmap(EQS_SWINDOW_HEADER_X, EQS_SWINDOW_HEADER_Y, pBitmap);
}