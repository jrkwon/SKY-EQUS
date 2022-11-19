/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_mo.h
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
** 2000-12-25 reo      Created.
**
** ============================================================================
*/

#ifndef _SKA_LOCA_H_
#define _SKA_LOCA_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FEATURE_SKTT_EQUS_LOCATION
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define LOCATION_INFORM				0x01	//��ġ �뺸
#define LOCATION_REMOTE_SETTING		0x02	//��ġ��Ȯ���ڴܸ� ���� ����
#define LOCATION_DELETE_SETTING		0x03	//���� ������ɷ�
#define LOCATION_INFORM_OFF			0x04	//��ġ �뺸 OFF �˸� 
#define LOCATION_INFORM_IN_CALL		0x05	//��ȭ�� �˸� 
#define LOCATION_INFORM_REQUEST		0x06	//��ġ�뺸 �䱸 


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
**  Variable Declarations
**---------------------------------------------------------------------------
*/
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/* APPLET */
extern VOID SKY_API SKA_LocationView(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_LocationNotif(SKY_EVENT_T* pEvent);      

/* UTILIRY */
extern VOID SaveLocationReport(LOCATION_INFO_T* pGetLocation);
extern BYTE ConvertDtmf(BYTE ch);
extern VOID RptLocationData(LOCATION_INFO_T* pGetLocation);
extern VOID ReportCfrmOff(LOCATION_INFO_T* pGetLocation);
extern VOID SaveTrafficLocationData(LOCATION_INFO_T* pGetLocation);
extern BYTE CompLocation(LOCATION_INFO_T* pGetLocation);
extern BYTE CheckCurrentTime(VOID);

#endif //FEATURE_SKTT_EQUS_LOCATION
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_MO_H_ */


