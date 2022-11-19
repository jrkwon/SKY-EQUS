/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_dscf.c
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
** 2000-12-27 htg     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

#ifndef WIN32
#include "dsskt.h"
#include "dsi.h"
#include "dsatcop.h"
#include "sdevmap.h"
extern int1 sdm_diag_port_id;
extern int1 sdm_ds_port_id;
#include "sio.h"
#include "siors232.h"
#include "hw.h"
#include "diag.h"
extern void siors_set_bit_rate
(
  sio_port_id_type     port_id,            /* SIO Port ID                  */  
  sio_stream_mode_type stream_mode,        /* Stream mode                  */
  sio_flow_ctl_type    tx_flow,            /* TX flow control method       */
  sio_bitrate_type     req_bitrate             /* specified baud rate for UART */
);
extern sio_stream_mode_type sio_current_stream_mode[ SIO_PORT_MAX];
extern sio_flow_ctl_type    sio_tx_flow_control_method[ SIO_PORT_MAX];
#endif

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

/*============================================================================= 
Function:    
SKA_MainXxxx

  Description: 
  @Description
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/

void SKY_API SKA_SetVoiceMode(SKY_EVENT_T* pEvent)
{
    BYTE* sIncomingMode[3];
    int nSel=0;
    HCONTROL hControl;
    NV_ITEM_U nv_data;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC HCONTROL s_hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(FALSE);
        EQS_SetWindow(SKY_DEF_WIN_X,
        SKY_DEF_WIN_Y, 
        SKY_DEF_WIN_DX, 
        120,//SKY_DEF_WIN_DY, 
        CL_NONE_SELECT_NONE,//CML_CANCEL_SELECT, 
        SKY_GET_TOKEN(TKN_INCOMMING_MODE), 
        WA_SHOW_HEADER|WA_SHOW_CML);
        sIncomingMode[0] = SKY_GET_TOKEN(TKN_VOICE_INCOMING);
        sIncomingMode[1] = SKY_GET_TOKEN(TKN_MODEM_INCOMING);
        sIncomingMode[2] = SKY_GET_TOKEN(TKN_FAX_INCOMING);
        // 6: # of eng. ch
        // 3 : #of item

#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && MODEL_ID != MODEL_B0
        if(SKY_GetNV(NV_DATA_REV_MODE_I, &nv_data) != NV_DONE_S || 
          nv_data.data_rev_mode > FAX_INCOMMING)
        {
          nv_data.data_rev_mode = dsAtSktDrVal = VOICE_INCOMMING;
          SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
          nSel = 0;
        }
        else
        {
          nSel = nv_data.data_rev_mode;
        }
#endif //FEATURE_SKT_DS_MMI
#if (MODEL_ID == MODEL_B0)
		//SKY_TRACE1(TL_HIGH,"rev_mode = %d",nv_data.data_rev_mode);
#endif
        hControl = EQC_RadioButton(5, 5, 8, 3, (BYTE **)sIncomingMode, nSel);
        EQC_SET_STYLE(hControl,(EQC_GET_STYLE(hControl) & ~CS_RADIO_BUTTON_BORDER)| CS_RADIO_BUTTON_HMAXIMIZE);
        EQS_SetIOCActive(hControl);
        break;

    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
      if(SKY_GET_DTR_DISP() != EQC_GetRadioButton(EQS_GetNthIOC(0)))
          EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
      else
*/
          EQS_END();
      break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
        if(SKY_GET_DTR_DISP() != EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            s_fEndToRoot = TRUE;
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
        {
            s_fEndToRoot = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
*/
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_SOFTKEY_SAVE:
    case SE_IO_RADIO_BUTTON_SELECT:
      hControl = EQS_GetNthIOC(0);
      nSel = EQC_GetRadioButton(hControl);

#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
      nv_data.data_rev_mode = dsAtSktDrVal = nSel;
      SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
      SKY_TRACE1(TL_HIGH,"착신모드 = %d",nSel);
        
      SKY_SET_DTR_DISP(nSel);
      if(nSel == VOICE_INCOMMING)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
        nv_data.data_rev_mode = dsAtSktDrVal = VOICE_INCOMMING;
        SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_INCOMING_VOICE), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else if(nSel == MODEM_INCOMMING)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
          nv_data.data_rev_mode = dsAtSktDrVal = MODEM_INCOMMING;
          SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_INCOMING_MODEM), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else if(nSel == FAX_INCOMMING)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
          nv_data.data_rev_mode = dsAtSktDrVal = FAX_INCOMMING;
          SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_INCOMING_FAX), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else
      {
      }
      //save user data
      break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }else
            EQS_END();
      break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
void SKY_API SKA_SetUmSpeed(SKY_EVENT_T* pEvent)
{
    BYTE* sUmSpeed[2];
    int nSel=0;
    HCONTROL hControl;
	NV_ITEM_U nv_data;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC HCONTROL s_hControl;


    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(FALSE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            120, //SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,//CML_CANCEL_SELECT, 
            SKY_GET_TOKEN(TKN_UM_SPEED_TITLE), 
            WA_SHOW_HEADER|WA_SHOW_CML);

        sUmSpeed[0] = SKY_GET_TOKEN(TKN_MAXIMUM_SPEED);
        sUmSpeed[1] = SKY_GET_TOKEN(TKN_NORMAL_SPEED);
        // 6: # of eng. ch
        // 3 : #of item
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32)
        if(SKY_GetNV(NV_DATA_MDR_MODE_I, &nv_data) != NV_DONE_S || nv_data.data_mdr_mode >= DS_MDR_MODE_MAX)
        {
          nv_data.data_mdr_mode = ds_atcop_qcmdr_val = DS_MDR_MODE_SO33_PREF;
          SKY_PutNV(NV_DATA_MDR_MODE_I, &nv_data);
          nSel = 0;
          ds_atcop_qcmdr_val    = DS_MDR_MODE_SO33_PREF;
        }
        else
        {
          if(nv_data.data_mdr_mode == DS_MDR_MODE_SO33_PREF 
            || nv_data.data_mdr_mode == DS_MDR_MODE_MDR_ONLY)
          {
            nSel = INTERNET_MAX_SPEED;
            ds_atcop_qcmdr_val    = DS_MDR_MODE_SO33_PREF;
          }
          else
          {
            nSel = INTERNET_NORMAL_SPEED;
            ds_atcop_qcmdr_val    = DS_MDR_MODE_NO_MDR;
          }
        }
#endif  //#if defined(FEATURE_INCOMING_MODE) && !defined(WIN32)

        hControl = EQC_RadioButton(5, 5, 8, 2, (BYTE **)sUmSpeed,  nSel); 
        EQC_SET_STYLE(hControl,(EQC_GET_STYLE(hControl) & ~CS_RADIO_BUTTON_BORDER) | CS_RADIO_BUTTON_HMAXIMIZE);
        EQS_SetIOCActive(hControl);
        break;

    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
      nSel = EQC_GetRadioButton(EQS_GetNthIOC(0));
#ifndef WIN32      
      if(nSel == 0)
        bTemp = DS_MDR_MODE_IF_AVAIL;    
      else if(nSel == 1)
        bTemp = DS_MDR_MODE_NO_MDR;

      if(ds_atcop_qcmdr_val != bTemp)
          EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
      else
#endif
*/
          EQS_END();
      break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
      nSel = EQC_GetRadioButton(EQS_GetNthIOC(0));
#ifndef WIN32            
      if(nSel == 0)
        bTemp = DS_MDR_MODE_IF_AVAIL;    
      else if(nSel == 1)
        bTemp = DS_MDR_MODE_NO_MDR;

      if(ds_atcop_qcmdr_val != bTemp)
        {
            s_fEndToRoot = TRUE;
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
        {
            s_fEndToRoot = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
#endif//#ifndef WIN32      
*/
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
      hControl = EQS_GetNthIOC(0);
      nSel = EQC_GetRadioButton(hControl);
#ifndef WIN32
      if(nSel == INTERNET_MAX_SPEED)
      {
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000)	
            ds_atcop_qcmdr_val = DS_MDR_MODE_SO33_PREF;
#else	    //	#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000)
            ds_atcop_qcmdr_val = DS_MDR_MODE_NO_MDR;
#endif	//	#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000)
            nv_data.data_mdr_mode = ds_atcop_qcmdr_val;
      }
      else
      {
          nv_data.data_mdr_mode = DS_MDR_MODE_NO_MDR;
          ds_atcop_qcmdr_val    = DS_MDR_MODE_NO_MDR;
      }

      SKY_PutNV(NV_DATA_MDR_MODE_I, &nv_data);

#endif //#ifndef WIN32

      if(nSel == INTERNET_MAX_SPEED)
      {
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_UM_MAXIMUM), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else if(nSel == INTERNET_NORMAL_SPEED)
      {
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_UM_NORMAL), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else
      {
        SKY_TRACE1(TL_HIGH,"Wrong return value[%d] from Radio",nSel);
      }
      //save user data
      break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }else
            EQS_END();
      break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_SetRmSpeed(SKY_EVENT_T* pEvent)
{
    BYTE* sRmSpeed[3];
    int nSel=0;
    STATIC BYTE bSpace;
    HCONTROL hControl;
    NV_ITEM_U nv_data;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC HCONTROL s_hControl;


    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(FALSE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            120,//SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,//CML_CANCEL_SELECT, 
            SKY_GET_TOKEN(TKN_RM_SPEED_TITLE), 
            WA_SHOW_HEADER|WA_SHOW_CML);
        sRmSpeed[0] = SKY_GET_TOKEN(TKN_RM_115200);
        sRmSpeed[1] =  SKY_GET_TOKEN(TKN_RM_19200);
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
        sRmSpeed[2] = SKY_GET_TOKEN(TKN_RM_230400);
#endif

#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)

        if(SKY_GetNV(NV_DATA_IPR_MODE_I, &nv_data) != NV_DONE_S)
        {
          nv_data.data_ipr_mode = PC_SPEED_115200;
          SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
          nSel = 0;
          bSpace = 1;
        }
        else
        {
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
          if(nv_data.data_ipr_mode == PC_SPEED_230400) 
          {
            nSel = 2;
          }else
#endif
          if(nv_data.data_ipr_mode == PC_SPEED_19200) 
          {
            nSel = 1;
          }
          else 
          {
            nSel = 0;
          }
        }
        // 6: # of eng. ch
        // 3 : #of item
#endif  //#ifndef WIN32
#if (MODEL_ID == MODEL_B0)
//		SKY_TRACE1(TL_HIGH,"ipr_mode = %d",nv_data.data_ipr_mode);
#endif
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
        hControl = EQC_RadioButton(5, 5, 8, 3, (BYTE **)sRmSpeed,  nSel);
#else
        hControl = EQC_RadioButton(5, 5, 8, 2, (BYTE **)sRmSpeed,  nSel);
#endif
        EQC_SET_STYLE(hControl,(EQC_GET_STYLE(hControl) & ~CS_RADIO_BUTTON_BORDER)| CS_RADIO_BUTTON_HMAXIMIZE);
        EQS_SetIOCActive(hControl);
        bSpace = nv_data.data_ipr_mode;
        break;

    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
      nSel = EQC_GetRadioButton(EQS_GetNthIOC(0));
      
      if(nSel == 0)
        bTemp = PC_SPEED_115200;    
      else if(nSel == 1)
        bTemp = PC_SPEED_19200;
      else if(nSel == 2)
        bTemp = PC_SPEED_230400;

      if(bSpace != bTemp)
          EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
      else
          EQS_END();
*/
        EQS_End();
      break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
      nSel = EQC_GetRadioButton(EQS_GetNthIOC(0));
      
      if(nSel == 0)
        bTemp = PC_SPEED_115200;    
      else if(nSel == 1)
        bTemp = PC_SPEED_19200;
      else if(nSel == 2)
        bTemp = PC_SPEED_230400;

        if(bSpace != bTemp)
        {
            s_fEndToRoot = TRUE;
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
        {
            s_fEndToRoot = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
*/
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
      hControl = EQS_GetNthIOC(0);
      nSel = EQC_GetRadioButton(hControl);
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
      if(nSel == 2)
      {
		nv_data.data_ipr_mode = PC_SPEED_230400;
		SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
		siors_set_bit_rate((sio_port_id_type)sdm_ds_port_id,
					   sio_current_stream_mode[sdm_ds_port_id],
				       sio_tx_flow_control_method[sdm_ds_port_id],
					   SIO_BITRATE_230400
					  );
		SKY_TRACE(TL_HIGH,"Baud Rate = 230400, ipr_mode = 2");
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_RM_230400), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }else
#endif
      if(nSel == 0)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
		nv_data.data_ipr_mode = PC_SPEED_115200;
		SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
		siors_set_bit_rate((sio_port_id_type)sdm_ds_port_id,
					   sio_current_stream_mode[sdm_ds_port_id],
				       sio_tx_flow_control_method[sdm_ds_port_id],
					   SIO_BITRATE_115200
					  );
#endif //#ifndef WIN32
		SKY_TRACE(TL_HIGH,"Baud Rate = 115200, ipr_mode = 1");
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_RM_115200), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else if(nSel == 1)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
		nv_data.data_ipr_mode = PC_SPEED_19200;
		SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
		siors_set_bit_rate((sio_port_id_type)sdm_ds_port_id,
					   sio_current_stream_mode[sdm_ds_port_id],
				       sio_tx_flow_control_method[sdm_ds_port_id],
					   SIO_BITRATE_19200
					  );
#endif
		SKY_TRACE(TL_HIGH,"Baud Rate = 19200, ipr_mode = 0");

      //save user data
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_RM_19200), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
	  }

      break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }else
            EQS_END();
      break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_SetDmSpeed(SKY_EVENT_T* pEvent)
{
    BYTE* sDmSpeed[2];
    int nSel;
    HCONTROL hControl;
    NV_ITEM_U nv_data;
//    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC HCONTROL s_hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(FALSE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            120,//SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,//CML_CANCEL_SELECT, 
            SKY_GET_TOKEN(TKN_DM_SPEED_TITLE), 
            WA_SHOW_HEADER|WA_SHOW_CML);

        sDmSpeed[0] = SKY_GET_TOKEN(TKN_DM_SPEED_38400);
        sDmSpeed[1] = SKY_GET_TOKEN(TKN_DM_SPEED_115200);
#ifndef WIN32
#if MODEL_ID == MODEL_B0
        if(SKY_GetNV(NV_DM_BAUD_I, &nv_data) != NV_DONE_S || nv_data.dm_baud >= 2)
        {
          nv_data.dm_baud = 0;
          SKY_PutNV(NV_DM_BAUD_I, &nv_data);
          nSel = 0;
        }
        else
        {
          if(nv_data.dm_baud == 0) 
          {
            nSel = 0;
          }
          else
          {
            nSel = 1;
          }
        }
#elif (MODEL_ID == MODEL_B1 || MODEL_ID == MODEL_B2)
        if(SKY_GetNV(NV_DIAG_DEFAULT_BAUDRATE_I, &nv_data) == NV_DONE_S)
        {
			if(nv_data.diag_default_baudrate == SIO_BITRATE_38400)
				nSel = 0;
			else if(nv_data.diag_default_baudrate == SIO_BITRATE_115200)
				nSel = 1;
			else
			{	// 잘못된 값이 들어 있는 경우에는 38400으로 set
				nv_data.diag_default_baudrate = (nv_sio_baudrate_type)SIO_BITRATE_38400;
		        SKY_PutNV(NV_DIAG_DEFAULT_BAUDRATE_I, &nv_data);
				nSel = 0;
			}
        }
        else
        {	// notactive인 경우에는 38400으로 set
				nv_data.diag_default_baudrate = (nv_sio_baudrate_type)SIO_BITRATE_38400;
		        SKY_PutNV(NV_DIAG_DEFAULT_BAUDRATE_I, &nv_data);
				nSel = 0;
        }
#endif	// #if MODEL_ID == MODEL_B0
#endif  //#ifndef WIN32
        hControl = EQC_RadioButton(5, 5, 8, 2, (BYTE **)sDmSpeed, nSel);
        EQC_SET_STYLE(hControl,(EQC_GET_STYLE(hControl) & ~CS_RADIO_BUTTON_BORDER) | CS_RADIO_BUTTON_HMAXIMIZE);
        EQS_SetIOCActive(hControl);
        break;

    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        EQS_END();
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
      hControl = EQS_GetNthIOC(0);
      nSel = EQC_GetRadioButton(hControl);
      if(nSel == 0)
      {

#ifndef WIN32
#if MODEL_ID == MODEL_B0
		nv_data.dm_baud = 0;
		SKY_PutNV(NV_DM_BAUD_I, &nv_data);
		siors_set_bit_rate(sdm_diag_port_id,
					   sio_current_stream_mode[sdm_diag_port_id],
				       sio_tx_flow_control_method[sdm_diag_port_id],
					   SIO_BITRATE_38400
					  );
#elif (MODEL_ID == MODEL_B1|| MODEL_ID == MODEL_B2)
		diag_change_default_sio_bitrate( SIO_BITRATE_38400 );
#endif	//#if MODEL_ID == MODEL_B0
#endif //#ifndef WIN32
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_DM_38400), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else if(nSel == 1)
      {
#ifndef WIN32
#if MODEL_ID == MODEL_B0
		nv_data.dm_baud = 1;
		SKY_PutNV(NV_DM_BAUD_I, &nv_data);
		siors_set_bit_rate(sdm_diag_port_id,
					   sio_current_stream_mode[sdm_diag_port_id],
				       sio_tx_flow_control_method[sdm_diag_port_id],
					   SIO_BITRATE_115200
					  );
#elif (MODEL_ID == MODEL_B1||MODEL_ID == MODEL_B2)
		diag_change_default_sio_bitrate( SIO_BITRATE_115200 );
#endif	//#if MODEL_ID == MODEL_B0
#endif
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_DM_115200), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else
      {
        SKY_TRACE1(TL_HIGH,"Wrong return value[%d] from Radio",nSel);
      }
      //save user data
      break;

    case SE_IO_MESGBOX_END:
        EQS_END();
      break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


int nPrevSel;
void SKY_API SKA_SetDmPort(SKY_EVENT_T* pEvent)
{
    BYTE* sDmPort[3];
    HCONTROL hControl;
    NV_ITEM_U nv_data;
    int nSel = 0;
//    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC HCONTROL s_hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(FALSE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            120,//SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,//CML_CANCEL_SELECT, 
            SKY_GET_TOKEN(TKN_DM_PORT_TITLE), 
            WA_SHOW_HEADER|WA_SHOW_CML);
#ifndef WIN32
#endif
        sDmPort[0] = SKY_GET_TOKEN(TKN_DM_PORT_UART1);
        sDmPort[1] = SKY_GET_TOKEN(TKN_DM_PORT_UART2);
        sDmPort[2] = SKY_GET_TOKEN(TKN_DM_PORT_OFF);
        // 6: # of eng. ch
        // 3 : #of item
		//Retrieve Saved Data
#ifndef WIN32
        if(SKY_GetNV(NV_SIO_DEV_MAP_MENU_ITEM_I, &nv_data) != NV_DONE_S )
        {
          nv_data.sio_dev_map_menu_item = SDM__U1_DS____U2_DIAG;
          SKY_PutNV(NV_SIO_DEV_MAP_MENU_ITEM_I, &nv_data);
          nSel = 1;
        }
        else
        {
          if(nv_data.sio_dev_map_menu_item ==  SDM__U1_DIAG__U2_NULL) // dm = uart1
          {
            nSel = 0;
          }
          else if(nv_data.sio_dev_map_menu_item ==  SDM__U1_DS____U2_DIAG)	//dm = uart2
          {
            nSel = 1;
          }
		  else if(nv_data.sio_dev_map_menu_item ==  SDM__U1_DS____U2_NULL)	// dm = off
		  {
            nSel = 2;
		  }
        }
#endif //#ifndef WIN32
		nPrevSel = nSel;		// save prev value

        hControl = EQC_RadioButton(5, 5, 8, 3, (BYTE **)sDmPort,  nSel);
        EQC_SET_STYLE(hControl,(EQC_GET_STYLE(hControl) & ~CS_RADIO_BUTTON_BORDER) | CS_RADIO_BUTTON_HMAXIMIZE);
        EQS_SetIOCActive(hControl);
        break;

    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        EQS_END();
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
      hControl = EQS_GetNthIOC(0);
      nSel = EQC_GetRadioButton(hControl);

	  //SAVE 
#ifndef WIN32
      if(nSel == 0)
      {
		nv_data.sio_dev_map_menu_item = SDM__U1_DIAG__U2_NULL;
      }
      else if(nSel == 1)
      {
		nv_data.sio_dev_map_menu_item = SDM__U1_DS____U2_DIAG;
      }
	  else
	  {
		nv_data.sio_dev_map_menu_item = SDM__U1_DS____U2_NULL;
	  }

      SKY_PutNV(NV_SIO_DEV_MAP_MENU_ITEM_I, &nv_data);

#endif //#ifndef WIN32


      if(nSel == 0)
      {
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_DM_UART1), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else if(nSel == 1)
      {
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_DM_UART2), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
      }
      else if(nSel == 2)
      {
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_SET_DM_OFF), 
            MESSAGE_DEFAULT_TIME,   MBT_NONE, MBT_HELP);
      } 

	  if(nPrevSel != nSel)	// 이전 value와 다를 경우에만 hw_reset실행함 
        EQS_StartTimer(EQS_TIMER_1, 1000, FALSE);
      break;

	case EQE_TM_TIMER1:
	  hw_reset();
	  break;

    case SE_IO_MESGBOX_END:
      EQS_END();
      break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;
    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

enum tag_DATA_SERVICE_E {
    DS_INCOMMING_TITLE,
    DS_INCOMMING_RADIO,
    DS_UMSPEED_TITLE,
    DS_UMSPEED_RADIO,
    DS_RMSPEED_TITLE,
    DS_RMSPEED_RADIO,
    DS_DATA_SERVICE_MAX
};

#define sDATA_SERVICE_RADIO_XWIDTH  14

void SKY_API SKA_MainDataServiceSet(SKY_EVENT_T *pEvent)
{
    BYTE i;
    BYTE* sIncomingMode[3];
    BYTE* sUmSpeed[2];
    BYTE* sRmSpeed[3];
    INT8 nSel=0;
    HCONTROL hControl;
    NV_ITEM_U nv_data;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC HCONTROL s_hControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            200, 
            CL_NONE_SELECT_SAVE,
            SKY_GET_TOKEN(TKN_DATA_SERVICE_SET), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        sIncomingMode[0] = SKY_GET_TOKEN(TKN_VOICE_INCOMING);
        sIncomingMode[1] = SKY_GET_TOKEN(TKN_MODEM_INCOMING);
        sIncomingMode[2] = SKY_GET_TOKEN(TKN_FAX_INCOMING);

        sUmSpeed[0] = SKY_GET_TOKEN(TKN_MAXIMUM_SPEED);
        sUmSpeed[1] = SKY_GET_TOKEN(TKN_NORMAL_SPEED);

        sRmSpeed[0] = SKY_GET_TOKEN(TKN_RM_115200);
        sRmSpeed[1] =  SKY_GET_TOKEN(TKN_RM_19200);
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
        sRmSpeed[2] = SKY_GET_TOKEN(TKN_RM_230400);
#endif

#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && MODEL_ID != MODEL_B0
        if(SKY_GetNV(NV_DATA_REV_MODE_I, &nv_data) != NV_DONE_S || 
          nv_data.data_rev_mode > FAX_INCOMMING)
        {
          nv_data.data_rev_mode = dsAtSktDrVal = VOICE_INCOMMING;
          SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
          nSel = 0;
        }
        else
        {
          nSel = nv_data.data_rev_mode;
        }
#endif //FEATURE_SKT_DS_MMI
        EQC_StaticText(EQS_NA, EQS_NA, TKN_INCOMMING_MODE);
        EQC_RadioButton(EQS_NA, EQS_NA, sDATA_SERVICE_RADIO_XWIDTH, 3, (BYTE **)sIncomingMode, nSel);

#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32)
        if(SKY_GetNV(NV_DATA_MDR_MODE_I, &nv_data) != NV_DONE_S || nv_data.data_mdr_mode >= DS_MDR_MODE_MAX)
        {
          nv_data.data_mdr_mode = ds_atcop_qcmdr_val = DS_MDR_MODE_SO33_PREF;
          SKY_PutNV(NV_DATA_MDR_MODE_I, &nv_data);
          nSel = 0;
          ds_atcop_qcmdr_val    = DS_MDR_MODE_SO33_PREF;
        }
        else
        {
          if(nv_data.data_mdr_mode == DS_MDR_MODE_SO33_PREF 
            || nv_data.data_mdr_mode == DS_MDR_MODE_MDR_ONLY)
          {
            nSel = INTERNET_MAX_SPEED;
            ds_atcop_qcmdr_val    = DS_MDR_MODE_SO33_PREF;
          }
          else
          {
            nSel = INTERNET_NORMAL_SPEED;
            ds_atcop_qcmdr_val    = DS_MDR_MODE_NO_MDR;
          }
        }
#endif  //#if defined(FEATURE_INCOMING_MODE) && !defined(WIN32)
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_UM_SPEED_TITLE);
        EQC_RadioButton(EQS_NA, EQS_NA, sDATA_SERVICE_RADIO_XWIDTH, 2, (BYTE **)sUmSpeed, nSel);
     
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)

        if(SKY_GetNV(NV_DATA_IPR_MODE_I, &nv_data) != NV_DONE_S)
        {
          nv_data.data_ipr_mode = PC_SPEED_115200;
          SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
          nSel = 0;
        }
        else
        {
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
          if(nv_data.data_ipr_mode == PC_SPEED_230400) 
          {
            nSel = 2;
          }else
#endif
          if(nv_data.data_ipr_mode == PC_SPEED_19200) 
          {
            nSel = 1;
          }
          else 
          {
            nSel = 0;
          }
        }
        // 6: # of eng. ch
        // 3 : #of item
#endif  //#ifndef WIN32

        EQC_StaticText(EQS_NA, EQS_NA, TKN_RM_SPEED_TITLE);
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
        hControl = EQC_RadioButton(EQS_NA, EQS_NA, sDATA_SERVICE_RADIO_XWIDTH, 3, (BYTE **)sRmSpeed, nSel);
#else
        hControl = EQC_RadioButton(EQS_NA, EQS_NA, sDATA_SERVICE_RADIO_XWIDTH, 2, (BYTE **)sRmSpeed, nSel);
#endif
        
        EQS_StartFormatter();

        for(i = 0; i < DS_DATA_SERVICE_MAX ; i++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(i), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        EQS_FitCanvasSize();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }
        break;

    case SE_SOFTKEY_SAVE:
      hControl = EQS_GetNthIOC(DS_INCOMMING_RADIO);
      nSel = EQC_GetRadioButton(hControl);

#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
      nv_data.data_rev_mode = dsAtSktDrVal = nSel;
      SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
      SKY_TRACE1(TL_HIGH,"착신모드 = %d",nSel);
        
      SKY_SET_DTR_DISP(nSel);
      if(nSel == VOICE_INCOMMING)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
        nv_data.data_rev_mode = dsAtSktDrVal = VOICE_INCOMMING;
        SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
      }
      else if(nSel == MODEM_INCOMMING)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
          nv_data.data_rev_mode = dsAtSktDrVal = MODEM_INCOMMING;
          SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
      }
      else if(nSel == FAX_INCOMMING)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
          nv_data.data_rev_mode = dsAtSktDrVal = FAX_INCOMMING;
          SKY_PutNV(NV_DATA_REV_MODE_I, &nv_data);
#endif
      }
      else
      {
      }

      hControl = EQS_GetNthIOC(DS_UMSPEED_RADIO);
      nSel = EQC_GetRadioButton(hControl);
#ifndef WIN32
      if(nSel == INTERNET_MAX_SPEED)
      {
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000)	
            ds_atcop_qcmdr_val = DS_MDR_MODE_SO33_PREF;
#else	    //	#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000)
            ds_atcop_qcmdr_val = DS_MDR_MODE_NO_MDR;
#endif	//	#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000)
            nv_data.data_mdr_mode = ds_atcop_qcmdr_val;
      }
      else
      {
          nv_data.data_mdr_mode = DS_MDR_MODE_NO_MDR;
          ds_atcop_qcmdr_val    = DS_MDR_MODE_NO_MDR;
      }

      SKY_PutNV(NV_DATA_MDR_MODE_I, &nv_data);

#endif //#ifndef WIN32

      hControl = EQS_GetNthIOC(DS_RMSPEED_RADIO);
      nSel = EQC_GetRadioButton(hControl);
#if defined(FEATURE_SKT_DS) && defined(FEATURE_DS_IS2000) && !defined(WIN32)	//	white 2001.04.13
      if(nSel == 2)
      {
		nv_data.data_ipr_mode = PC_SPEED_230400;
		SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
		siors_set_bit_rate((sio_port_id_type)sdm_ds_port_id,
					   sio_current_stream_mode[sdm_ds_port_id],
				       sio_tx_flow_control_method[sdm_ds_port_id],
					   SIO_BITRATE_230400
					  );
		SKY_TRACE(TL_HIGH,"Baud Rate = 230400, ipr_mode = 2");
      }else
#endif
      if(nSel == 0)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
		nv_data.data_ipr_mode = PC_SPEED_115200;
		SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
		siors_set_bit_rate((sio_port_id_type)sdm_ds_port_id,
					   sio_current_stream_mode[sdm_ds_port_id],
				       sio_tx_flow_control_method[sdm_ds_port_id],
					   SIO_BITRATE_115200
					  );
#endif //#ifndef WIN32
		SKY_TRACE(TL_HIGH,"Baud Rate = 115200, ipr_mode = 1");
      }
      else if(nSel == 1)
      {
#if defined(FEATURE_SKT_DS_MMI) && !defined(WIN32) && (MODEL_ID != MODEL_B0)
		nv_data.data_ipr_mode = PC_SPEED_19200;
		SKY_PutNV(NV_DATA_IPR_MODE_I, &nv_data);
		siors_set_bit_rate((sio_port_id_type)sdm_ds_port_id,
					   sio_current_stream_mode[sdm_ds_port_id],
				       sio_tx_flow_control_method[sdm_ds_port_id],
					   SIO_BITRATE_19200
					  );
#endif
		SKY_TRACE(TL_HIGH,"Baud Rate = 19200, ipr_mode = 0");

      //save user data
	  }
        s_hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
     
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_APP_IO_MODE:
        EQS_SetWindowCML(CL_NONE_CONFIRM_SAVE);
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
    case SE_IO_RADIO_BUTTON_CANCEL:
        EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
       
    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCFocus(EQS_GetNthIOC(0));
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



