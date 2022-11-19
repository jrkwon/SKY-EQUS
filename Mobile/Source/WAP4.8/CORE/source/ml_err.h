/*+MHDR*/
/*
# clearcase: CmicroPatchRel2.3.3
+------------------------------------------------------------------------------+
|  Headerfile : ML_ERR.H                                                       |
|  Author       : S&P Media GmbH Germany                                       |
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
/*-MHDR*/



#ifndef __ML_ERR_H_
#define __ML_ERR_H_

/*
+------------------------------------------------------------------------------+
|  Constants                                                                   |
+------------------------------------------------------------------------------+
*/


/*---------------------------------------*/
/* E r r o r s  a n d  W a r n i n g s   */
/*---------------------------------------*/

/*
**  Errors in the Cmicro Kernel / Cmicro Library
*/
#define ERR_N_UNDEFINED_ERROR           -40
#define ERR_N_CREATE_NO_MEM             -41
#define ERR_N_CREATE_INSTANCE           -42
#define ERR_N_SDL_DISCARD               -43
#define ERR_N_DIVIDE_BY_ZERO            -44
#define ERR_N_EMPTY_CHARSTRING          -45
#define ERR_N_ERROR_CHARSTRING          -46
#define ERR_N_FATAL_CHARSTRING          -47
#define ERR_N_NO_FREE_SIGNAL            -48
#define ERR_N_PARAMETER_MEM_ALLOC       -49
#define ERR_N_INDEX_SIGNAL              -50
#define ERR_N_SYSTEM_SIGNAL             -51
#define ERR_N_NO_CONT_PRIO              -52
#define ERR_N_NO_COR_PRIO               -53
#define ERR_N_xRouteSignal              -55
#define ERR_N_NO_REC_AVAIL              -56
#define ERR_N_NO_FREE_TIMER             -57
#define ERR_N_PID_INDEX                 -58
#define ERR_N_SEND_TO_NULLPID           -59
#define ERR_N_TRANS_TIME                -60
#define ERR_N_xOutEnv                   -61
#define ERR_N_TRACE_OUTPUT              -62
#define ERR_N_RECORD_OUTPUT             -63
#define ERR_N_RECORD_MAX_EXCEEDED       -64
#define ERR_N_RECORD_STATE              -65
#define ERR_N_RECORD_CANNOT_CONT        -66
#define ERR_N_INIT_SDL_MEM              -67
#define ERR_N_SDL_DECISION_ELSE         -68
#define ERR_N_SDL_RANGE                 -69
#define ERR_N_NO_RCV                    -70
#define ERR_N_SDL_IMPLICIT_CONSUMPTION  -71
#define ERR_N_PREDEFINED_OPERATOR_CALL  -72
#define ERR_N_INIT_QUEUE                -73
#define ERR_N_MEM_PARAM                 -74
#define ERR_N_MEM_NO_FREE               -75
#define ERR_N_MEM_ILLMBLOCK             -76
#define ERR_N_NULL_POINTER_VALUE_USED   -78


/*
**  Data link / Communications interface / Encoder / Decoder
*/
#define ERR_N_LINK_CHKSUM               -80
#define ERR_N_LINK_DEST_BUFFER          -81
#define ERR_N_LINK_ENC_MEMORY           -82
#define ERR_N_LINK_ENC_LENGTH           -83
#define ERR_N_LINK_NOT_IMPL             -84
#define ERR_N_LINK_SYNC                 -85
#define ERR_N_DATA_LINK                 -86
#define ERR_N_DECODE_METHOD             -87
#define ERR_N_CHK_MEMORY                -88
#define ERR_N_CHK_AMOUNT                -89
#define ERR_N_CHK_ORDERING              -90
#define ERR_N_DEC_MEMORY                -91
#define ERR_N_DEC_AMOUNT                -92
#define ERR_N_DEC_ORDERING              -93
#define ERR_N_ENC_MEMORY                -94
#define ERR_N_ENC_AMOUNT                -95
#define ERR_N_ENC_ORDERING              -96


/*
**  Error in utility functions
*/
#define ERR_N_RING_WRITE_LENGTH         -100
#define ERR_N_RING_READ_LENGTH          -101


/*
**  Errors in the communication modules
*/
#define ERR_N_OPEN_TTY                 -111
#define ERR_N_GET_TTY_MODE             -112
#define ERR_N_SET_TTY_MODE             -113
#define ERR_N_RETRIEVE_TTY_MODE        -114
#define ERR_N_CLOSE_TTY                -115
#define ERR_N_TTY_WRITE                -116
#define ERR_N_TTYO_ENABLE              -117
#define ERR_N_TTYO_DISABLE             -118
#define ERR_N_BUF_SIZE_EXCEEDED        -119
#define ERR_N_TTYI_ENABLE              -120
#define ERR_N_TTY_IO_FLUSH             -121
#define ERR_N_TTY_BAUDRATE             -122
#define ERR_N_SOCKET_WRITE             -123
#define ERR_N_SOCKET_READ              -124


/* 
**  Errors in the Cmicro Tester
*/
#define ERR_N_ILLEGAL_CMD               -140
#define ERR_N_TESTER_MESSAGE            -141


#ifdef NO_GLOBAL_VARS
  /* if you use the Epoc OS you must know the number of errors 
  ** if you add new errors do not forget to increase the size
  */ 
  #define  HIGHEST_ERR_N                 64 
#endif


/*
**  Errors in ascii text
*/

#define ERR_T_UNDEFINED_ERROR \
        "ERR: Internal Error (Missing Errortext) ! **\n"

#define ERR_T_CREATE_INSTANCE \
        "ERR: Cannot create another instance (no free instance) !\n"

#define ERR_T_CREATE_NO_MEM \
        "ERR: Cannot create another instance (no free mem for createsig) !\n"

#define ERR_T_SDL_DISCARD \
        "ERR: Unexpected signal was discarded !\n"

#define ERR_T_DIVIDE_BY_ZERO \
        "ERR:Division by 0, catched up by Cmicro Library !\n"

#define ERR_T_EMPTY_CHARSTRING \
        "ERR: Invalid operation on empty SDL Charstring !\n"

#define ERR_T_ERROR_CHARSTRING \
        "ERR: Invalid operation on charstring (either empty or index error) !\n"

#define ERR_T_FATAL_CHARSTRING \
        "ERR: Invalid access to SDL Charstring (NULL pointer) !\n"

#define ERR_T_NO_FREE_SIGNAL \
        "ERR: No free memory to allocate one more signal instance in output !\n"

#define ERR_T_PARAMETER_MEM_ALLOC \
        "ERR: No free memory to allocate parameters of the signal in output !\n"

#define ERR_T_INDEX_SIGNAL \
        "ERR: Unallowed index value in Signal filter function !\n"

#define ERR_T_SYSTEM_SIGNAL \
        "ERR: Systemerror - Unable to work on Signal !\n"

#define ERR_T_NO_CONT_PRIO \
        "ERR: Prio-levels numbered with gaps !\n"

#define ERR_T_NO_COR_PRIO \
        "ERR: MAX_PRIO_LEVELS is not correctly assigned !\n"

#define ERR_T_xRouteSignal \
        "ERR: No receiver for signal (xRouteSignal-function) !\n"

#define ERR_T_NO_REC_AVAIL \
        "ERR: No receiver available for this signal !\n"

#define ERR_T_NO_FREE_TIMER \
        "ERR: No free timer !\n"

#define ERR_T_PID_INDEX \
        "ERR: PID-Index error while sending signal !\n"

#define ERR_T_SEND_TO_NULLPID \
        "ERR: Unallowed to SEND to a NULL-PID !\n"

#define ERR_T_TRANS_TIME \
        "ERR: Maximum time for transitions function exceeded !\n"

#define ERR_T_xOutEnv \
        "ERR: function xOutEnv () has not consumed a signal to send to the environment !\n"

#define ERR_T_TRACE_OUTPUT \
        "ERR: Buffer too small to transfer parameter of SDL signal(trace)\n"

#define ERR_T_RECORD_OUTPUT \
        "ERR: Buffer too small to transfer parameter of SDL signal(record)\n"

#define ERR_T_RECORD_MAX_EXCEEDED \
        "ERR: Cmicro Recorder: Counter value overflow (Record/Play)\n"

#define ERR_T_RECORD_STATE \
        "ERR: System is in a state where a received message is ignored\n"

#define ERR_T_RECORD_CANNOT_CONT \
        "ERR: Recorder cannot continue due to fatal error in the system\n"

#define ERR_T_INIT_SDL_MEM \
        "ERR: Malloc / Free : Missing initialization\n"

#define ERR_T_SDL_DECISION_ELSE \
        "ERR: SDL Decision without else causes fatal errors\n"

#define ERR_T_SDL_RANGE \
        "ERR: SDL Range error in assignment leads to fatal errors !\n"

#define ERR_T_NO_RCV \
        "ERR: No receiver found in function xRouteSignal (module mk_user.c) !\n"

#define ERR_T_SDL_IMPLICIT_CONSUMPTION \
        "ERR: Signal has been implicitely consumed !\n"

#define ERR_T_PREDEFINED_OPERATOR_CALL \
        "ERR: An error occurred in the parameters of an call to an predefined operator !\n"

#define ERR_T_INIT_QUEUE \
        "ERR: The SDL Queue was not initialized correctly !\n"

#define ERR_T_MEM_PARAM \
        "ERR: The SDL Memory allocator was called with zero block size !\n" 

#define ERR_T_MEM_NO_FREE \
        "ERR: There is no more memory available in the SDL memory allocator !\n" 

#define ERR_T_MEM_ILLMBLOCK \
        "ERR: In SDL Memory allocator : Try to free a memory block which was never allocated !\n"

#define ERR_T_NULL_POINTER_VALUE_USED \
        "ERR: Trying to use a NULL pointer value in expression !"

#define ERR_T_ILLEGAL_CMD \
        "ERR: Illegal Cmicro Tester command !\n"

#define ERR_T_TESTER_MESSAGE \
        "ERR: Error in Cmicro Tester Message (communications interface) !\n"

#define ERR_T_LINK_CHKSUM \
        "ERR: Checksum error on communications interface !\n"

#define ERR_T_LINK_DEST_BUFFER \
        "ERR: Not enough mem to decode Message of communications interface !\n"

#define ERR_T_LINK_ENC_LENGTH \
        "ERR: Length error when encoding for communications interface !\n"

#define ERR_T_LINK_ENC_MEMORY \
        "ERR: Not enough mem to encode message for communications interface !\n"

#define ERR_T_LINK_NOT_IMPL \
        "ERR: Unimplented feature on communications interface\n"

#define ERR_T_LINK_SYNC \
        "ERR: Sync on communications interface\n"

#define ERR_T_DATA_LINK \
        "ERR: Error in data link of communications interface !\n"

#define ERR_T_DECODE_METHOD \
        "ERR: No method specified to decode current data link frame !\n"

#define ERR_T_CHK_MEMORY     \
        "ERR: Error in message coder (memorylayout-size) !\n"

#define ERR_T_CHK_AMOUNT     \
        "ERR: Error in message coder (memorylayout-amount) !\n"

#define ERR_T_CHK_ORDERING   \
        "ERR: Error in message coder (memorylayout-ordering) !\n"

#define ERR_T_DEC_MEMORY     \
        "ERR: Error in message decoder (memorylayout-size) !\n"

#define ERR_T_DEC_AMOUNT     \
        "ERR: Error in message decoder (memorylayout-amount) !\n"

#define ERR_T_DEC_ORDERING   \
        "ERR: Error in message decoder (memorylayout-ordering) !\n"

#define ERR_T_ENC_MEMORY     \
        "ERR: Error in message encoder (memorylayout-size) !\n"

#define ERR_T_ENC_AMOUNT     \
        "ERR: Error in message encoder (memorylayout-amount) !\n"

#define ERR_T_ENC_ORDERING   \
        "ERR: Error in message encoder (memorylayout-ordering) !\n"

#define ERR_T_RING_WRITE_LENGTH \
        "ERR: Length Error while writing into ringbuffer !\n"

#define ERR_T_RING_READ_LENGTH \
        "ERR: Length Error while reading from ringbuffer !\n"

#define ERR_T_OPEN_TTY                 \
        "ERR: Cannot open specified tty-device\n"

#define ERR_T_GET_TTY_MODE             \
        "ERR: Cannot get tty-mode\n"

#define ERR_T_SET_TTY_MODE             \
        "ERR: Cannot set tty-mode\n"

#define ERR_T_RETRIEVE_TTY_MODE        \
        "ERR: Cannot retrieve tty-mode\n"

#define ERR_T_CLOSE_TTY                \
        "ERR: Cannot close tty\n"

#define ERR_T_TTY_WRITE                \
        "ERR: Cannot write to tty-device\n"

#define ERR_T_TTYO_ENABLE              \
        "ERR: Cannot disable transmitter - XOFF failed\n"

#define ERR_T_TTYO_DISABLE             \
        "ERR: Cannot enable transmitter - XON failed\n"

#define ERR_T_SOCKET_READ             \
        "ERR: Cannot read from socket\n"

#define ERR_T_SOCKET_WRITE             \
        "ERR: Cannot write into socket\n"

#define ERR_T_BUF_SIZE_EXCEEDED             \
        "ERR: The given buffer is too small to keep all data \n"

#define ERR_T_TTYI_ENABLE              \
        "ERR: Cannot initialize target-transmitter - XON failed\n"

#define ERR_T_TTY_IO_FLUSH              \
        "ERR: Flushing the input and output queues failed\n"

#define ERR_T_TTY_BAUDRATE              \
        "ERR: Non implemented baudrate for communications interface\n"

#ifdef XMK_ADD_STDIO
  extern void xmk_err_text XPP(( FILE *, int ));
  #ifdef SDTMT
    #ifdef __cplusplus
    extern "C" {
    #endif
    extern void  xmk_PrintErrorText XPP(( FILE *, int ));
    extern char* xmk_GetErrorText XPP(( int ));
    #ifdef __cplusplus
    }
    #endif
  #endif
#endif /* ... XMK_ADD_STDIO */

#if defined(XMK_ADD_PRINTF_ERROR)
  #ifdef NO_GLOBAL_VARS
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

   typedef struct
   {
     int nr ;
     int cl ;
     char p_text [MAX_ERR_TEXT +1] ;
   }  T_ERROR ;
  #endif
#endif







#endif /* ... __ML_ERR_H_  */

