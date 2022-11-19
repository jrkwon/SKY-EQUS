/*+MHDR*/
/*
# clearcase: CmicroPatchRel2.3.3
+------------------------------------------------------------------------------+
|  Modulname    : ML_ERR.C                                                     |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  This Module contains Utilityfunctions for the ErrorHandler ().              |
|  Currently, there is only one function exported, which is used to print      |
|  ascii text on a given filedescriptor.                                       |
|                                                                              |
|  M1 Errormessages, which are routed to the ErrorHandler                      |
|  --------------------------------------------------------                    |
|    - none -                                                                  |
|                                                                              |
|  M2 Exported functions of this module :                                      |
|  --------------------------------------------------------                    |
|    ... xmk_err_text           ( .... )                                       |
|                                                                              |
|  M3 Static functions of this module :                                        |
|  --------------------------------------------------------                    |
|    - none -                                                                  |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-MHDR*/

/*
+------------------------------------------------------------------------------+
|                                                                              |
|  Copyright by Telelogic AB 1993 - 1998                                       |
|  Copyright by S&P Media GmbH Germany 1993 - 1998                             |
|                                                                              |
|  This Program is owned by Telelogic and is protected by national             |
|  copyright laws and international copyright treaties. Telelogic              |
|  grants you the right to use this Program on one computer or in              |
|  one local computer network at any one time.                                 |
|  Under this License you may only modify the source code for the purpose      |
|  of adapting it to your environment. You must reproduce and include          |
|  any copyright and trademark notices on all copies of the source code.       |
|  You may not use, copy, merge, modify or transfer the Program except as      |
|  provided in this License.                                                   |
|  Telelogic does not warrant that the Program will meet your                  |
|  requirements or that the operation of the Program will be                   |
|  uninterrupted and error free. You are solely responsible that the           |
|  selection of the Program and the modification of the source code            |
|  will achieve your intended results and that the results are actually        |
|  obtained.                                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/



#ifndef __ML_ERR_C_
#define __ML_ERR_C_


/*+IMPORT*/
/*====================  I M P O R T  =========================================*/

#include "ml_typ.h"


/*============================================================================*/
/*-IMPORT*/



/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/

#if defined(XMK_ADD_PRINTF_ERROR)

/*--------------------  Constants,  Macros  ----------------------------------*/
#ifndef NO_GLOBAL_VARS
#define MAX_ERR_LEN          256
#define MAX_ERR_TEXT         230



/*
**  Numbering of error classes
*/
#define ECINFO        1
#define ECWARNING     2
#define ECERROR       3

#define ERR_USER_T_INFO      "**INFO:"
#define ERR_USER_T_WARNING   "**WARNING:"
#define ERR_USER_T_ERROR     "**ERR:"

#define ERR_MSGLEN           100


/*--------------------  Typedefinitions     ----------------------------------*/

typedef struct
{
   int nr ;
   int cl ;
   char p_text [MAX_ERR_TEXT +1] ;

}  T_ERROR ;

/*--------------------  Functions    -----------------------------------------*/

/*--------------------  Variables    -----------------------------------------*/


static T_ERROR err_text [] =
{
 /* Note : ERR_N_UNDEFINED_ERROR is to be the err_text[0] - element */
 /*        for internal purposes !                          */
 { ERR_N_UNDEFINED_ERROR           , ECERROR         , ERR_T_UNDEFINED_ERROR          },
 { ERR_N_CREATE_NO_MEM             , ECERROR         , ERR_T_CREATE_NO_MEM            },
 { ERR_N_CREATE_INSTANCE           , ECERROR         , ERR_T_CREATE_INSTANCE          },
 { ERR_N_SDL_DISCARD               , ECERROR         , ERR_T_SDL_DISCARD              },
 { ERR_N_DIVIDE_BY_ZERO            , ECERROR         , ERR_T_DIVIDE_BY_ZERO           },
 { ERR_N_EMPTY_CHARSTRING          , ECWARNING       , ERR_T_EMPTY_CHARSTRING         },
 { ERR_N_ERROR_CHARSTRING          , ECWARNING       , ERR_T_ERROR_CHARSTRING         },
 { ERR_N_FATAL_CHARSTRING          , ECERROR         , ERR_T_FATAL_CHARSTRING         },
 { ERR_N_NO_FREE_SIGNAL            , ECERROR         , ERR_T_NO_FREE_SIGNAL           },
 { ERR_N_PARAMETER_MEM_ALLOC       , ECERROR         , ERR_T_PARAMETER_MEM_ALLOC      },
 { ERR_N_INDEX_SIGNAL              , ECERROR         , ERR_T_INDEX_SIGNAL             },
 { ERR_N_SYSTEM_SIGNAL             , ECERROR         , ERR_T_SYSTEM_SIGNAL            },
 { ERR_N_NO_CONT_PRIO              , ECERROR         , ERR_T_NO_CONT_PRIO             },
 { ERR_N_NO_COR_PRIO               , ECERROR         , ERR_T_NO_COR_PRIO              },
 { ERR_N_xRouteSignal              , ECWARNING       , ERR_T_xRouteSignal             },
 { ERR_N_NO_REC_AVAIL              , ECERROR         , ERR_T_NO_REC_AVAIL             },
 { ERR_N_NO_FREE_TIMER             , ECERROR         , ERR_T_NO_FREE_TIMER            },
 { ERR_N_PID_INDEX                 , ECERROR         , ERR_T_PID_INDEX                },
 { ERR_N_SEND_TO_NULLPID           , ECERROR         , ERR_T_SEND_TO_NULLPID          },
 { ERR_N_TRANS_TIME                , ECWARNING       , ERR_T_TRANS_TIME               },
 { ERR_N_xOutEnv                   , ECWARNING       , ERR_T_xOutEnv                  },
 { ERR_N_TRACE_OUTPUT              , ECERROR         , ERR_T_TRACE_OUTPUT             },
 { ERR_N_RECORD_OUTPUT             , ECERROR         , ERR_T_RECORD_OUTPUT            },
 { ERR_N_RECORD_MAX_EXCEEDED       , ECERROR         , ERR_T_RECORD_MAX_EXCEEDED      },
 { ERR_N_RECORD_STATE              , ECWARNING       , ERR_T_RECORD_STATE             },
 { ERR_N_RECORD_CANNOT_CONT        , ECERROR         , ERR_T_RECORD_CANNOT_CONT       },
 { ERR_N_INIT_SDL_MEM              , ECERROR         , ERR_T_INIT_SDL_MEM             },
 { ERR_N_INIT_QUEUE                , ECERROR         , ERR_T_INIT_QUEUE               },
 { ERR_N_MEM_NO_FREE               , ECERROR         , ERR_T_MEM_NO_FREE              },
 { ERR_N_MEM_PARAM                 , ECERROR         , ERR_T_MEM_PARAM                },
 { ERR_N_MEM_ILLMBLOCK             , ECERROR         , ERR_T_MEM_ILLMBLOCK            },
 { ERR_N_SDL_DECISION_ELSE         , ECERROR         , ERR_T_SDL_DECISION_ELSE        },
 { ERR_N_SDL_RANGE                 , ECERROR         , ERR_T_SDL_RANGE                },
 { ERR_N_PREDEFINED_OPERATOR_CALL  , ECERROR         , ERR_T_PREDEFINED_OPERATOR_CALL },
 { ERR_N_NO_RCV                    , ECERROR         , ERR_T_NO_RCV                   },
 { ERR_N_SDL_IMPLICIT_CONSUMPTION  , ECWARNING       , ERR_T_SDL_IMPLICIT_CONSUMPTION },
 { ERR_N_LINK_CHKSUM               , ECWARNING       , ERR_T_LINK_CHKSUM              },
 { ERR_N_LINK_DEST_BUFFER          , ECWARNING       , ERR_T_LINK_DEST_BUFFER         },
 { ERR_N_LINK_ENC_MEMORY           , ECWARNING       , ERR_T_LINK_ENC_MEMORY          },
 { ERR_N_LINK_ENC_LENGTH           , ECWARNING       , ERR_T_LINK_ENC_LENGTH          },
 { ERR_N_LINK_NOT_IMPL             , ECWARNING       , ERR_T_LINK_NOT_IMPL            },
 { ERR_N_LINK_SYNC                 , ECWARNING       , ERR_T_LINK_SYNC                },
 { ERR_N_DATA_LINK                 , ECWARNING       , ERR_T_DATA_LINK                },
 { ERR_N_DECODE_METHOD             , ECWARNING       , ERR_T_DECODE_METHOD            },
 { ERR_N_CHK_MEMORY                , ECWARNING       , ERR_T_CHK_MEMORY               },
 { ERR_N_CHK_AMOUNT                , ECWARNING       , ERR_T_CHK_AMOUNT               },
 { ERR_N_CHK_ORDERING              , ECWARNING       , ERR_T_CHK_ORDERING             },
 { ERR_N_DEC_MEMORY                , ECWARNING       , ERR_T_DEC_MEMORY               },
 { ERR_N_DEC_AMOUNT                , ECWARNING       , ERR_T_DEC_AMOUNT               },
 { ERR_N_DEC_ORDERING              , ECWARNING       , ERR_T_DEC_ORDERING             },
 { ERR_N_ENC_MEMORY                , ECWARNING       , ERR_T_ENC_MEMORY               },
 { ERR_N_ENC_AMOUNT                , ECWARNING       , ERR_T_ENC_AMOUNT               },
 { ERR_N_ENC_ORDERING              , ECWARNING       , ERR_T_ENC_ORDERING             },
 { ERR_N_RING_WRITE_LENGTH         , ECWARNING       , ERR_T_RING_WRITE_LENGTH        },
 { ERR_N_RING_READ_LENGTH          , ECWARNING       , ERR_T_RING_READ_LENGTH         },
 { ERR_N_OPEN_TTY                  , ECERROR         , ERR_T_OPEN_TTY                 },
 { ERR_N_GET_TTY_MODE              , ECERROR         , ERR_T_GET_TTY_MODE             },
 { ERR_N_SET_TTY_MODE              , ECERROR         , ERR_T_SET_TTY_MODE             },
 { ERR_N_RETRIEVE_TTY_MODE         , ECERROR         , ERR_T_RETRIEVE_TTY_MODE        },
 { ERR_N_CLOSE_TTY                 , ECERROR         , ERR_T_CLOSE_TTY                },
 { ERR_N_TTY_WRITE                 , ECERROR         , ERR_T_TTY_WRITE                },
 { ERR_N_TTYO_ENABLE               , ECERROR         , ERR_T_TTYO_ENABLE              },
 { ERR_N_TTYO_DISABLE              , ECERROR         , ERR_T_TTYO_DISABLE             },
 { ERR_N_BUF_SIZE_EXCEEDED         , ECERROR         , ERR_T_BUF_SIZE_EXCEEDED        },
 { ERR_N_TTYI_ENABLE               , ECERROR         , ERR_T_TTYI_ENABLE              },
 { ERR_N_TTY_IO_FLUSH              , ECERROR         , ERR_T_TTY_IO_FLUSH             },
 { ERR_N_TTY_BAUDRATE              , ECERROR         , ERR_T_TTY_BAUDRATE             },
 { ERR_N_SOCKET_WRITE              , ECERROR         , ERR_T_SOCKET_WRITE             },
 { ERR_N_SOCKET_READ               , ECERROR         , ERR_T_SOCKET_READ              },
 { ERR_N_ILLEGAL_CMD               , ECWARNING       , ERR_T_ILLEGAL_CMD              },
 { ERR_N_TESTER_MESSAGE            , ECERROR         , ERR_T_TESTER_MESSAGE           },
 { ERR_N_NULL_POINTER_VALUE_USED   , ECERROR         , ERR_T_NULL_POINTER_VALUE_USED  },
 { 0                               , 0               , " "                        }

} ;
#endif /* NO_GLOBAL_VARS */
/*============================================================================*/
/*-MGG*/


/*+FHDR E*/ /*
+----------------------------------------------------------------------------+
|  Functionname : xmk_err_text                                               |
|  Author       : S&P Media GmbH Germany                                     |
+---------------------------------------------------------------------------+|
|                                                                            |
|   Description :                                                            |
|    This function is used to evaluate an ASCII-Errortext from an error-     |
|    number given by the caller.                                             |
|                                                                            |
|    The first parameter fp must contain a pointer to a valid                |
|    (that is:opened) file descriptor, for example it could be stdin, stdout,|
|    or a file which was opened with fopen.                                  |
|                                                                            |
|    The second parameter nr  contains one  of the possible error numbers    |
|    defined in ml_err.h.                                                    |
|                                                                            |
|  Parameter    : FILE *fp      -   File descriptor                          |
|                 int nr        -   Errornumber                              |
|                                                                            |
|  Return       :                                                            |
|                 -                                                          |
|                                                                            |
+----------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_err_text ( FILE *fp, int nr )
#else
  void xmk_err_text ( fp, nr )
FILE *fp;
int nr ;
#endif


/*-FDEF E*/
{
    int   i        ;
    char *p_help   ;
    char *p_help_2 ;

    XMK_FUNCTION("xmk_err_text");

    for ( i = 0 ; err_text[i].nr != 0 ; i++ )
    {
       if ( err_text[i].nr == nr )
       {
          /*
          **  Found !
          */
          p_help = err_text[i].p_text ;


          switch ( err_text[i].cl )
          {
             case ECINFO   : p_help_2 = ERR_USER_T_INFO   ; break;
             case ECWARNING: p_help_2 = ERR_USER_T_WARNING; break;
             case ECERROR  : p_help_2 = ERR_USER_T_ERROR  ; break;
             default:
               (void)fprintf (fp, "**ERR:ILLEGAL ERROR MESSAGE:%d\n", nr ) ;
               XMK_TRACE_EXIT("xmk_err_text");
               return ;
          }

          (void)fprintf (fp, "%s %d %s\n", p_help_2, nr, p_help ) ;
          XMK_TRACE_EXIT("xmk_err_text");
          return ;
       }
    }
    (void)fprintf (fp, "**ERR:ILLEGAL ERROR MESSAGE:%d\n", nr ) ;
    XMK_TRACE_EXIT("xmk_err_text");
    return ;

} /* end of function */

#endif /* ... XMK_ADD_PRINTF_ERROR */

#endif /* ... __ML_ERR_C */

/*====================  E N D     O F   M O D U L E   ==========================*/
