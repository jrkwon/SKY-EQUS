/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     sky_cml.c
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
** 2001-08-01 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"

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

/* new command line */

LOCAL ROM CML_T aCMLContents[] = {
    /* CL_NONE,                      */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_SELECT_ADD,           */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_ADD,     RCL_ADD          } },
    /* CL_SCHED_UPDOWN_PBOOK         */  { { SE_SOFTKEY_SMS,     LCL_NONE         }, { SE_SOFTKEY_MENU,    CCL_NONE         }, { SE_SOFTKEY_WAP,     RCL_NONE         } },
    /* CL_NONE_NONE_SAVE,            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
    /* CL_NONE_SELECT_NONE,          */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_NONE_SELECT_BOTTOMUP,      */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_BOTTOMUP,RCL_BOTTOMUP     } },
	/* CL_MENU_MOVEDOWN_BOTTOMUP,    */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_MOVEDOWN,CCL_MOVEDOWN     }, { SE_SOFTKEY_BOTTOMUP,RCL_BOTTOMUP     } },
	/* CL_NONE_START_NONE,           */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_START,   CCL_START        }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_VOLUME_NONE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_VOLUME,  CCL_VOLUME       }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_TOSMS_NONE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_TOSMS,   CCL_TOSMS        }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_MOVE_NONE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVE ,   CCL_MOVE         }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_MOVE_EDIT,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVE ,   CCL_MOVE         }, { SE_SOFTKEY_EDIT,    RCL_EDIT         } },
	/* CL_DELETE_MOVE_MODIFY,		 */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_MOVE ,   CCL_MOVE         }, { SE_SOFTKEY_MODIFY,  RCL_MODIFY       } },
	
	/* CL_NONE_RECEIVESEND_NONE,	 */  { { SE_KEY_NONE,        LCL_NONE         }, {SE_SOFTKEY_RECEIVESEND,CCL_RECEIVESEND}, { SE_KEY_NONE,        RCL_NONE         } },

	/* CL_NONE_MOVEUP_NONE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVEUP,  CCL_MOVEUP      }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_MOVEDOWN_NONE,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVEDOWN,  CCL_MOVEDOWN      }, { SE_KEY_NONE,        RCL_NONE         } },	
	/* CL_MENU_MOVEDOWN_NONE,		 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_MOVEDOWN,CCL_MOVEDOWN    }, { SE_KEY_NONE,        RCL_NONE         } },

	/* CL_NONE_MOVE_DELETE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVE,    CCL_MOVE         }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },		
    /* CL_NONE_MOVE_SEND,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVE,    CCL_MOVE         }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	
	/* CL_MENU_MOVEDOWN_SEND,	     */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_MOVEDOWN,CCL_MOVEDOWN     }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	/* CL_NONE_SAVE_STOP,            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SAVE,    CCL_SAVE         }, { SE_SOFTKEY_STOP,    RCL_STOP         } },
    /* CL_NONE_SAVE_FIND,            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SAVE,    CCL_SAVE         }, { SE_SOFTKEY_FIND,    RCL_FIND         } },
    /* CL_NONE_MOVE_DST,             */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVE,	   CCL_MOVE         }, { SE_SOFTKEY_DST,     RCL_DST          } },
    /* CL_NONE_CONFIRM_DELETE,       */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },
    /* CL_NONE_CONFIRM_FIND,         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_FIND,    RCL_FIND         } },

	/* CL_NONE_CONFIRM_TOSMS,        */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_TOSMS,   RCL_TOSMS        } },
	/* CL_NONE_CONFIRM_ASSIGN,       */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ASSIGN,  RCL_ASSIGN       } },


    /* CL_NONE_SEND_NONE,            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SEND,    CCL_SEND         }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_NONE_NONE_FIND,            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_FIND,    RCL_FIND         } },
    /* CL_NONE_NONE_SET,             */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SET,     RCL_SET          } },
    /* CL_NONE_NONE_MODIFY,          */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_MODIFY,  RCL_MODIFY       } },
    /* CL_NONE_NONE_NEWHOLIDAY,      */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ADD,     RCL_NEWHOLIDAY   } },
    /* CL_NONE_CONFIRM_NONE,         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_CANCEL_CONFIRM_NONE,       */  { { SE_SOFTKEY_CANCEL,  LCL_CANCEL       }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_NONE_NONE_DELETE,          */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },
    /* CL_NONE_NONE_EDIT,            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_EDIT,    RCL_EDIT         } },
    /* CL_NONE_NONE_RELEASE,         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_RELEASE, RCL_RELEASE      } },
    /* CL_NONE_NONE_CANCEL,          */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },
    /* CL_NONE_COMPLETE_NONE,        */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_COMPLETE,CCL_COMPLETE     }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_NONE_NONE_BOTTOMUP,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_BOTTOMUP,RCL_BOTTOMUP     } },
	/* CL_NONE_NONE_ASSIGN,		     */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ASSIGN,  RCL_ASSIGN       } },
	/* CL_NONE_SELECT_ASSIGN,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_ASSIGN,  RCL_ASSIGN       } },
	/* CL_NONE_NONE_NEXT,		     */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_NEXT,    RCL_NEXT         } },
	/* CL_NONE_NONE_TOSMS,		     */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_TOSMS,   RCL_TOSMS        } },
    /* CL_NONE_NONE_NEWMAIL,         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ADD,     RCL_NEWMAIL      } },	
	/* CL_NONE_NONE_PASTE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_PASTE,   RCL_PASTE        } },
	/* CL_NONE_PASTE_NONE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_PASTE,   CCL_PASTE        }, { SE_KEY_NONE,        RCL_NONE         } },

	/* CL_NONE_CONFIRM_PASTE,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_PASTE,   RCL_PASTE        } },
	/* CL_NONE_MOVE_SELECT,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_MOVE,    CCL_MOVE         }, { SE_SOFTKEY_SELECT,  RCL_SELECT       } },
	/* CL_NONE_PLAY_DELETE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_PLAY,    CCL_PLAY         }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },
	/* CL_NONE_OKUPDOWN_SEND,        */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_OKUPDOWN,CCL_OKUPDOWN     }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
    /* CL_NONE_OKUPDOWN_NONE,        */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_OKUPDOWN,CCL_OKUPDOWN     }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_MENU_LIST_PLAY,			 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_LIST,	   CCL_LIST	        }, { SE_SOFTKEY_PLAY,    RCL_PLAY         } },



	/* CL_MENU_NONE_NEXT,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_NEXT,    RCL_NEXT         } },
    /* CL_MENU_NONE_SEND,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
    /* CL_MENU_NONE_MODIFY,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_MODIFY,  RCL_MODIFY       } },
    /* CL_MENU_NONE_DELETE,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },
    /* CL_MENU_NONE_ASSIGN,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ASSIGN,  RCL_ASSIGN       } },
    /* CL_MENU_NONE_SORT,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SORT,    RCL_SORT         } },
    /* CL_MENU_NONE_EDIT,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_EDIT,    RCL_EDIT         } },
	/* CL_MENU_VIEW_EDIT,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_VIEW,    CCL_VIEW         }, { SE_SOFTKEY_EDIT,    RCL_EDIT         } },
    /* CL_MENU_NONE_FIND,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_FIND,    RCL_FIND         } },
    /* CL_MENU_SELECT_FIND,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_FIND,    RCL_FIND         } },
    /* CL_MENU_SELECT_ADD,           */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_ADD,     RCL_ADD          } },
	/* CL_MENU_SELECT_NONE,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_KEY_NONE,        RCL_NONE         } },

    /* CL_MENU_PBOOK_DELETE,         */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_PBOOK,   CCL_PBOOK        }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },
    /* CL_MENU_TOSMS_NONE,           */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_TOSMS,   CCL_TOSMS        }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_MENU_NONE_ADD,             */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ADD,     RCL_ADD          } },
    /* CL_MENU_NONE_VIEW,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_VIEW,    RCL_VIEW         } },
    /* CL_MENU_NONE_SAVE,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
    /* CL_MENU_NONE_REPLY,           */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_REPLY,   RCL_REPLY        } },
    /* CL_MENU_CONFIRM_LUNAR,        */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_LUNAR,   RCL_LUNAR        } },
    /* CL_MENU_CONFIRM_JULIAN,       */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_JULIAN,  RCL_JULIAN       } },
    /* CL_MENU_CONFIRM_NEWSCHED,     */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ADD,     RCL_NEWSCHED     } },
//    /* CL_DELETE_CONFIRM_NEWHOLIDAY,   */  { { SE_SOFTKEY_DELETE,    LCL_DELETE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ADD,     RCL_NEWHOLIDAY   } },
    /* CL_NONE_NONE_NEWALARM,     */     { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE      }, { SE_SOFTKEY_NEWALARM,   RCL_NEWALARM     } },
    /* CL_MENU_CONFIRM_NEWALARM,     */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_NEWALARM,RCL_NEWALARM     } },
	/* CL_MENU_CONFIRM_DELETE,       */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },

//    /* CL_MENU_CONFIRM_NEWMEMO,      */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ADD,     RCL_NEWMEMO      } },
    /* CL_MENU_NONE_NONE,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_MENU_NONE_PLAY,            */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_PLAY,    RCL_PLAY         } },
    /* CL_MENU_VOLUME_PBOOK,         */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_VOLUME,  CCL_VOLUME       }, { SE_SOFTKEY_PBOOK,   RCL_PBOOK        } },
    /* CL_MENU_SELECT_NEWMAIL,         */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_ADD,   RCL_NEWMAIL        } },
    /* CL_MENU_SELECT_WRITE,         */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_WRITE,   RCL_WRITE        } },
	/* CL_NONE_CONTINUE_RESET,       */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONTINUE,CCL_CONTINUE     }, { SE_SOFTKEY_RESET,   RCL_RESET        } },
	/* CL_VIEW_CONTINUE_RESET,       */  { { SE_SOFTKEY_VIEW,    LCL_VIEW         }, { SE_SOFTKEY_CONTINUE,CCL_CONTINUE     }, { SE_SOFTKEY_RESET,   RCL_RESET        } },
    /* CL_VIEW_SAVE_BOTTOMUP,        */  { { SE_SOFTKEY_VIEW,    LCL_VIEW         }, { SE_SOFTKEY_SAVE,    CCL_SAVE         }, { SE_SOFTKEY_BOTTOMUP,RCL_BOTTOMUP     } },

	/* CL_MENU_SORISAM_DELETE,       */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SORISAM, CCL_SORISAM      }, { SE_SOFTKEY_DELETE  ,RCL_DELETE       } },
	/* CL_MENU_SAVE_DELETE,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SAVE,    CCL_SAVE         }, { SE_SOFTKEY_DELETE  ,RCL_DELETE       } },
	/* CL_ALARM_CONFIRM_NEWSCHED,    */  { { SE_SOFTKEY_ALARM,   LCL_ALARM        }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ADD,     RCL_NEWSCHED     } },
	/* CL_DELETE_CONFIRM_NEWSCHED,   */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ADD,     RCL_NEWSCHED     } },
    /* CL_NONE_NONE_NEWSCHED,        */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ADD,     RCL_NEWSCHED     } },
	/* CL_NONE_NONE_NEWMEMO,         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ADD,     RCL_NEWMEMO      } },

	/* CL_MENU_NONE_PLAYMUSIC,		 */	 { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_PLAY,    RCL_PLAYMUSIC    } },
	/* CL_MENU_NONE_STOP			 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_STOP,    RCL_STOP         } },
	/* CL_MENU_SELECT_SAVE			 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },

	/* CL_MENU_CONFIRM_REPLY		 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_REPLY,   RCL_REPLY        } },
	/* CL_MENU_SELECT_SEND			 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_SEND,    RCL_SEND         } },

	/* CL_MENU_INPUT_SEND	 		 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_INPUT,   CCL_INPUT        }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	/* CL_NONE_INPUT_SEND	 		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_INPUT,   CCL_INPUT        }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	/* CL_MENU_OKUPDOWN_SEND		 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_OKUPDOWN,   CCL_OKUPDOWN        }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
    /* CL_MENU_OKUP_SEND             */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_OKUP,   CCL_OKUP       }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
    /* CL_MENU_MOVE_SEND             */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_MOVE,   CCL_MOVE        }, { SE_SOFTKEY_SEND,    RCL_SEND         } },

	/* CL_DELETE_NONE_REPLY			 */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_REPLY,   RCL_REPLY        } },

    /* CL_DELETE_NONE_PROTECT,       */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_PROTECT, RCL_PROTECT      } },
    /* CL_DELETE_NONE_RELEASE,       */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_RELEASE, RCL_RELEASE      } },

    /* CL_FWD_REPLY_DELETE,          */  { { SE_SOFTKEY_FWD,     LCL_FWD          }, { SE_SOFTKEY_REPLY,   CCL_REPLY        }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },

    /* CL_DELETE_NONE_NONE,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_DELETE_NONE_SEND,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
    /* CL_DELETE_SORISAM_SAVE,       */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_SORISAM, CCL_SORISAM      }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
    /* CL_DELETE_NONE_SAVE,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
    /* CL_DELETE_NONE_PLAYMUSIC,     */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_PLAY,    RCL_PLAYMUSIC    } },
    /* CL_DELETE_NONE_STOP,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_STOP,    RCL_STOP         } },
    /* CL_DELETE_NONE_VIEW,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_VIEW,    RCL_VIEW         } },
    /* CL_DELETE_NONE_FWD,           */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_FWD,     RCL_FWD          } },
	/* CL_DELETE_NONE_WRITE,         */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,		   CCL_NONE         }, { SE_SOFTKEY_WRITE,   RCL_WRITE        } },
	/* CL_DELETE_NONE_MODIFY,        */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,		   CCL_NONE         }, { SE_SOFTKEY_MODIFY,  RCL_MODIFY       } },
	/* CL_DELETE_SELECT_MODIFY,      */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_MODIFY,  RCL_MODIFY       } },
    /* CL_DELETE_CONFIRM_NEWMEMO,    */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ADD,     RCL_NEWMEMO      } },
    /* CL_DELETE_CONFIRM_NEWHOLIDAY, */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_ADD,     RCL_NEWHOLIDAY   } },
    /* CL_DELETE_CONFIRM_NONE,       */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_DELETE_MODIFY_SEND,        */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_MODIFY,  CCL_MODIFY       }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
    /* CL_DELETE_SEND_NONE,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_SEND,    CCL_SEND         }, { SE_KEY_NONE,        RCL_NONE         } },
    /* CL_DELETE_SORISAM_NONE,       */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_SORISAM, CCL_SORISAM      }, { SE_KEY_NONE,        RCL_NONE         } },		
    /* CL_DELETE_NONE_ASSIGN,        */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ASSIGN,  RCL_ASSIGN       } },
    /* CL_DELETE_COMPLETE_NONE,      */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_COMPLETE, CCL_COMPLETE    }, { SE_KEY_NONE,        RCL_NONE         } },

	/* CL_DELETE_REC_PLAY,           */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_REC,     CCL_REC          }, { SE_SOFTKEY_PLAY,    RCL_PLAY         } },
	/* CL_SAVE_NONE_ALARM,           */  { { SE_SOFTKEY_SAVE,    LCL_SAVE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_ALARM,   RCL_ALARM        } },
    /* CL_INIT_NONE_SAVE,            */  { { SE_SOFTKEY_INIT,    LCL_INIT         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
	/* CL_INIT_SELECT_SAVE,          */  { { SE_SOFTKEY_INIT,    LCL_INIT         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
    /* CL_FIND_NONE_SEND,			 */  { { SE_SOFTKEY_FIND,    LCL_FIND         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	/* CL_FIND_MOVE_SEND,			 */  { { SE_SOFTKEY_FIND,    LCL_FIND         }, { SE_SOFTKEY_MOVE,    CCL_MOVE         }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	/* CL_FIND_MOVEUP_SEND,  		 */  { { SE_SOFTKEY_FIND,    LCL_FIND         }, { SE_SOFTKEY_MOVEUP,  CCL_MOVEUP       }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	/* CL_FIND_OKUPDOWN_SEND,        */  { { SE_SOFTKEY_FIND,    LCL_FIND         }, { SE_SOFTKEY_OKUPDOWN,CCL_OKUPDOWN     }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
    /* CL_FIND_OKUP_SEND,            */  { { SE_SOFTKEY_FIND,    LCL_FIND         }, { SE_SOFTKEY_OKUP,    CCL_OKUP         }, { SE_SOFTKEY_SEND,    RCL_SEND         } },
	/* CL_NONE_SELECT_VIEW           */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_VIEW,    RCL_VIEW         } },
	/* CL_NONE_SELECT_SAVE           */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },

	/* CL_BEFOREVIEW_SELECT_SAVE     */  { { SE_SOFTKEY_BEFOREVIEW,LCL_BEFOREVIEW }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
	/* CL_RECEIPTBELL_MINUTELY_DELETE*/  { {SE_SOFTKEY_RECEIPTBELL,LCL_RECEIPTBELL}, { SE_SOFTKEY_MINUTELY,CCL_MINUTELY	    }, { SE_SOFTKEY_DELETE,  RCL_DELETE       } },
	/* CL_MENU_BEFOREVIEW_NONE       */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_BEFOREVIEW,CCL_BEFOREVIEW }, { SE_KEY_NONE,          RCL_NONE       } },

	/* CL_NONE_NONE_INIT             */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_INIT,    RCL_INIT         } },


	/* CL_WRITE_SEND_DELETE,         */  { { SE_SOFTKEY_WRITE,   LCL_WRITE        }, { SE_SOFTKEY_SEND,    CCL_SEND         }, { SE_SOFTKEY_DELETE  ,RCL_DELETE       } },
	/* CL_WRITE_SEND_DELETE,         */  { { SE_SOFTKEY_WRITE,   LCL_WRITE        }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_DELETE  ,RCL_DELETE       } },
	/* CL_EDIT_SEND_DELETE,          */  { { SE_SOFTKEY_EDIT,    LCL_EDIT         }, { SE_SOFTKEY_SEND,    CCL_SEND         }, { SE_SOFTKEY_DELETE  ,RCL_DELETE       } },
	/* CL_EDIT_NONE_DELETE,          */  { { SE_SOFTKEY_EDIT,    LCL_EDIT         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_DELETE  ,RCL_DELETE       } },


	/* CL_MENU_CONFIRM_NONE,         */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_MENU_PBOOK_NONE,           */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_PBOOK,   CCL_PBOOK        }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_DELETE_PLAY_NONE,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_PLAY,    CCL_PLAY         }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_DELETE_MOVE_NONE,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_MOVE,    CCL_MOVE         }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_DELETE_REPLY_FWD,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_REPLY,   CCL_REPLY        }, { SE_SOFTKEY_FWD,     RCL_FWD          } },
	/* CL_DELETE_SEND_WRITE,         */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_SEND,    CCL_SEND         }, { SE_SOFTKEY_WRITE,   RCL_WRITE        } },
	/* CL_DELETE_SEND_EDIT,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_SEND,    CCL_SEND         }, { SE_SOFTKEY_EDIT,    RCL_EDIT         } },
	/* CL_DELETE_NONE_EDIT,          */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_EDIT,    RCL_EDIT         } },

	/* CL_NONE_SELECT_DST            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_DST,     RCL_DST          } },
	/* CL_NONE_WRITE_NONE            */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_WRITE,   CCL_WRITE        }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_NEWALARM_NONE         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_NEWALARM,CCL_NEWALARM     }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_NONE_NEWNAME          */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_NEWNAME, RCL_NEWNAME      } },
	/* CL_MENU_SELECT_NEWNAME        */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_NEWNAME, RCL_NEWNAME      } },
	/* CL_NONE_NONE_NEWGROUP         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_NEWGROUP,RCL_NEWGROUP     } },
	/* CL_MENU_SELECT_NEWGROUP       */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_NEWGROUP,RCL_NEWGROUP     } },
	/* CL_NONE_SET_SAVE              */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SETUP        }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
	/* CL_NONE_DISSOLVE_SAVE         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_DISSOLVE     }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },

	/* CL_MENU_SORISAM_DELETE,       */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SORISAM, CCL_SORISAM      }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_MENU_SAVE_DELETE,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SAVE,    CCL_SAVE         }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_MENU_PBOOK_CANCEL,         */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_PBOOK,   CCL_PBOOK        }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },
	/* CL_MENU_NONE_CANCEL,          */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },
	/* CL_NONE_CALL_CANCEL,          */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CALL,    CCL_CALL         }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },
	/* CL_NONE_INPUT_SAVE,           */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_INPUT,   CCL_INPUT        }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
	/* CL_DELETE_REPLY_NONE,         */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_REPLY,   CCL_REPLY        }, { SE_KEY_NONE,        RCL_NONE         } },

	/* CL_NONE_TOSMS_CANCEL,         */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_TOSMS,   CCL_TOSMS        }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },
	/* CL_NONE_CONFIRM_CANCEL,       */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },

	/* CL_DELETE_SELECT_SAVE,        */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } },
	/* CL_NONE_SELECT_INDETAIL,      */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_INDETAIL,RCL_INDETAIL     } },
	/* CL_NONE_SELECT_CANCEL,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },
	/* CL_NONE_CONFIRM_SAVE,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_SAVE,	 RCL_SAVE	      } },
	/* CL_CANCEL_CONFIRM_DOT,		 */  { { SE_SOFTKEY_CANCEL,  LCL_CANCEL       }, { SE_SOFTKEY_CONFIRM, CCL_CONFIRM      }, { SE_SOFTKEY_DOT,     RCL_DOT	      } },

	/* CL_MENU_REPLY_FWD,			 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_REPLY,   CCL_REPLY        }, { SE_SOFTKEY_FWD,     RCL_FWD          } },
	/* CL_MENU_SELECT_NEWARTICLE,	 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_NEWARTICLE,RCL_NEWARTICLE } },
	/* CL_NONE_NONE_NEWARTICLE,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_NEWARTICLE,RCL_NEWARTICLE } },
	/* CL_DELETE_WRITE_NONE,		 */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_WRITE,   CCL_WRITE        }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_RECEIPT_NONE,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_RECEIPT, CCL_RECEIPT      }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_MENU_SAVE_CANCEL,			 */  { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_SAVE,    CCL_SAVE         }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } },
	/* CL_NONE_SETUP_NONE,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_SETUP,   CCL_SETUP        }, { SE_KEY_NONE,        RCL_NONE         } },
	/* CL_NONE_DISSOLVE_NONE,		 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_DISSOLVE,CCL_DISSOLVE     }, { SE_KEY_NONE,        RCL_NONE         } },

	/* CL_DELETE_MODIFY_NONE,		 */  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_SOFTKEY_MODIFY,  CCL_MODIFY       }, { SE_KEY_NONE,        RCL_NONE         } },

	/* CL_NONE_VOLUME_NEXT,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_SOFTKEY_VOLUME,  CCL_VOLUME       }, { SE_SOFTKEY_NEXT,    RCL_NEXT         } },
	/* CL_NONE_NONE_STOP,			 */  { { SE_KEY_NONE,        LCL_NONE         }, { SE_KEY_NONE,		   CCL_NONE         }, { SE_SOFTKEY_STOP,    RCL_STOP         } }
    /* CL_DELETE_NONE_CANCEL,		 */,  { { SE_SOFTKEY_DELETE,  LCL_DELETE       }, { SE_KEY_NONE,	   CCL_NONE         }, { SE_SOFTKEY_CANCEL,  RCL_CANCEL       } }
    /* CL_POINT_NONE_AC              */,  { { SE_SOFTKEY_F1,      LCL_POINT        }, { SE_KEY_NONE,	   CCL_NONE         }, { SE_SOFTKEY_F2,          RCL_AC           } }

#ifdef FEATURE_WAVELET
    /* CL_PLAY_STOP_SAVE,			 */, { { SE_SOFTKEY_PLAY,    LCL_PLAY         }, { SE_SOFTKEY_STOP,    CCL_STOP         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } }	//yoursun 01/06/19
	/* CL_PLAY_NONE_SAVE,			 */, { { SE_SOFTKEY_PLAY,    LCL_PLAY         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } }	//yoursun 01/08/30
	/* CL_PLAY_STOP_END,			 */, { { SE_SOFTKEY_PLAY,    LCL_PLAY         }, { SE_SOFTKEY_STOP,    CCL_STOP         }, { SE_SOFTKEY_END,	 RCL_END          } }	//yoursun 01/08/30
	/* CL_PLAY_NONE_END,			 */, { { SE_SOFTKEY_PLAY,    LCL_PLAY         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_END,	 RCL_END          } }	//yoursun 01/08/30
	/* CL_NONE_NONE_END,			 */, { { SE_KEY_NONE,		 LCL_NONE         }, { SE_KEY_NONE,		   CCL_NONE         }, { SE_SOFTKEY_END,	 RCL_END         } }	//yoursun 01/08/30
    /* CL_REPLY_STOP_SAVE,			 */, { { SE_SOFTKEY_REPLY,   LCL_REPLY        }, { SE_SOFTKEY_STOP,    CCL_STOP         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } }	//yoursun 01/10/16
	/* CL_REPLY_NONE_SAVE,			 */, { { SE_SOFTKEY_REPLY,   LCL_REPLY        }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } }	//yoursun 01/10/16
	/* CL_REPLY_STOP_END,			 */, { { SE_SOFTKEY_REPLY,   LCL_REPLY        }, { SE_SOFTKEY_STOP,    CCL_STOP         }, { SE_SOFTKEY_END,	 RCL_END          } }	//yoursun 01/10/16
	/* CL_REPLY_NONE_END,			 */, { { SE_SOFTKEY_REPLY,   LCL_REPLY        }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_END,	 RCL_END          } }	//yoursun 01/10/16

#endif //FEATURE_WAVELET
#ifdef  FEATURE_CHARACTER_BELL
    /* CL_FIND_SELECT_SAVE,			 */, { { SE_SOFTKEY_FIND,    LCL_FIND         }, { SE_SOFTKEY_SELECT,  CCL_SELECT       }, { SE_SOFTKEY_SAVE,    RCL_SAVE         } }	// bgjung 01/09/15
    /*CL_MENU_BEFOREVIEW_SET,        */, { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_SOFTKEY_BEFOREVIEW,CCL_BEFOREVIEW }, { SE_SOFTKEY_SET,     RCL_SET          } }  // bgjung 01/09/24
    /*CL_MENU_NONE_SET,              */, { { SE_SOFTKEY_MENU,    LCL_MENU         }, { SE_KEY_NONE,        CCL_NONE         }, { SE_SOFTKEY_SET,     RCL_SET          } }  // bgjung 01/09/24
#endif // FEATURE_CHARACTER_BELL
    /* CL_CANCEL_CONFIRM_NEXT,	 */ , { { SE_SOFTKEY_CANCEL,    LCL_CANCEL }, { SE_SOFTKEY_CONFIRM,	CCL_CONFIRM  }, { SE_SOFTKEY_NEXT,    RCL_NEXT   } }	// by yhlee 01/09/25
    /* CL_CANCEL_SIGN_NONE,			*/ , { { SE_SOFTKEY_CANCEL,    LCL_CANCEL }, { SE_SOFTKEY_MENU,		  CCL_SIGN		  },  { SE_KEY_NONE,		  RCL_NONE   } },  // by yhlee 01/09/25
};


ROM CML_INFO_T SKY_CML = {
    aCMLContents,
    CL_MAX,  // The number of CML
    &g_BmpCommandLineLeft,
    &g_BmpCommandLineCenter,
    &g_BmpCommandLineRight
};
