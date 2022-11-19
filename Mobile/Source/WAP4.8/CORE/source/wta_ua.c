/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */


/*========================================================================

	WAP WTA Implementation Project

==========================================================================

	File: wta_ua.c

	Revision history:
  	Date    Sign	Comment
  	010307  JPR     Implementation of function int2wchar changed
    010316  ASPN    Function CallDropCalls modified
    010402  ASPN    Member fEndContext removed from WTACONTROL struct
    010502  ASPN    Function 'CallModeIsKeep' added.
    010504  ASPN    CallHandle type changed from INT8 to INT16.
    015004  ASPN    Functions wtaiEventCallAlerting, wtaiEventCallHeld
                    and wtaiEventCallActive modified.
	
=========================================================================*/

#ifndef WTA_UA_C
#define WTA_UA_C
#include "wta_ua.h"

void CallDropCalls(void** ppCallList)
{
   INT8 iCallID;
   void* pTheReqElm;

   if (*ppCallList)
   {
	   pTheReqElm = SDL_GetNextListElement(*ppCallList,NULL);
	   while (pTheReqElm)
	   {
		  iCallID = (INT8)SDL_GetListElementID( pTheReqElm );
		  WTAIa_voiceCallRelease (NULL, NULL, iCallID, FALSE); /* FALSE:disable response */
		  pTheReqElm = SDL_GetNextListElement(*ppCallList, pTheReqElm);
	   } 
	   SDL_DeleteList(ppCallList);
   }
}

BOOL CallModeIsKeep(void** ppCallList, INT16 callHandle)
{
  void* pTheReqElm;

  if (*ppCallList)
  {
    pTheReqElm = SDL_GetNextListElement(*ppCallList,NULL);
    while (pTheReqElm)
    {
      if (callHandle == SDL_GetListElementID( pTheReqElm ))
      {
        return FALSE; /* call handle found in the "drop" list*/
      }
      else
      {
        pTheReqElm = SDL_GetNextListElement(*ppCallList, pTheReqElm);
      }
    }
  }
  return TRUE; /* call handle NOT in the "drop" list*/
}

void* SDL_CreateWTAControl(int iViewID)
{
    WTACONTROL* pNewControl = NULL;
    pNewControl = NEWSTRUCT(WTACONTROL);
	
	if (!pNewControl)
      return NULL;

    pNewControl->fContextProtected= FALSE;
	pNewControl->pCallList= NULL;
	pNewControl->iViewID= (UINT8)iViewID;
	pNewControl->ppWTAIResult=NULL;
    pNewControl->pwchWTAChannelId=NULL;
    return pNewControl;
}

void SDL_DeleteWTAControl(void** ppCallCtrl)
{
  if (*ppCallCtrl)
  {
	  CallDropCalls( & ((WTACONTROL*)(*ppCallCtrl))->pCallList );

	  /* Delete ppWTAIResult */

	  /* TODO */

      DEALLOC(& ((WTACONTROL*)(*ppCallCtrl))->pwchWTAChannelId);
	  DEALLOC(ppCallCtrl);
  }
}


void SDL_TerminateService(void* pWTAControl)
{
   WTACONTROL* pWTAControlStruct = (WTACONTROL*)pWTAControl;
   if (pWTAControl != NULL)
   {
     CallDropCalls(& pWTAControlStruct->pCallList );
     MMIa_status( (UINT8)pWTAControlStruct->iViewID, WTAServiceTerminated, NULL );
	 pWTAControlStruct->fContextProtected= FALSE;
     DEALLOC(& pWTAControlStruct->pwchWTAChannelId);
   }
}


void AddCall(void** ppCallList, INT16 callId)
{ 
   if (*ppCallList == NULL)
   {
      *ppCallList = SDL_NewList(0);
   }

   if (SDL_SearchListElement(*ppCallList, callId, NULL)==NULL)
       SDL_AddElementFirst(*ppCallList, callId, NULL);
}


WCHAR* int2wchar (INT32 iValue)
{
  BYTE tmp[25];
  BYTE* res;
  INT8  i=23;
  tmp[24]=0;  /* Term char */

  while (iValue != 0)
  {
    tmp[i--] = (BYTE)(iValue % 10 + '0');
    iValue /= 10;
  }
  res = tmp + i + 1;
  return wip_byte2wchar(res); 
}

/*------------------------------------------------------------------------------------------
variable and URI parameter handling 
---------------------------------------------------------------------------------------------*/

BOOL GetLibAndFunction(BYTE* pURL, BYTE** ppWTAILib, BYTE** ppWTAIFunction, BYTE** ppWTAIParameters)
{
  if ( pURL && b_GetHost ((BYTE*)pURL, ppWTAILib) && b_GetPath ((BYTE*)pURL, ppWTAIFunction) && b_GetParameters ((BYTE*)pURL, ppWTAIParameters) )
    return TRUE;
  else
    return FALSE;
}


BYTE* GetNextWTAIInParamFromAnURI(BYTE** ppbInStr)
{
   BYTE* pbResult=NULL;
   BYTE* pbTemp=*ppbInStr;
   UINT16 iLen=0;

   if (*ppbInStr==NULL)
	   return NULL;

   /* find next ; */
   while (*pbTemp && *pbTemp != ';' && *pbTemp != '!')  
   {
     pbTemp++;
   }
   
   iLen = (UINT16)(pbTemp-*ppbInStr);
   if (*ppbInStr != pbTemp)
   {
      pbResult = NEWARRAY(BYTE,iLen+1);
      B_COPYSTRINGN( pbResult, *ppbInStr, iLen );
	  pbResult[iLen]=0;
   }

   if (*pbTemp == ';')
      pbTemp++;

   *ppbInStr=pbTemp;
   return pbResult;
}

BYTE* GetNextWTAIOutParamFromAnURI(BYTE** ppbInStr)
{

   if (*ppbInStr==NULL)
	   return NULL;

   if (**ppbInStr == '!')
     (*ppbInStr)++;

   return GetNextWTAIInParamFromAnURI(ppbInStr);

}


BOOL CreateVariableList(UINT8 iSize, variableType*** pppListHandle, BOOL fWithIndex)
{
  variableType **ppNewList = NULL;
  UINT8 i=0;

  ppNewList =  (variableType **)OSConnectorAlloc((iSize + 1) * sizeof(variableType *));
  if (!ppNewList)
    return FALSE;
  i = iSize;
  while (i)
  {
	i--;
	ppNewList[i] = (variableType *) OSConnectorAlloc(sizeof(variableType));
    if (!ppNewList[i])
      return FALSE;
	ppNewList[i]->name = NULL;
    ppNewList[i]->value = NULL;
	if (fWithIndex)
	  ppNewList[i]->name = int2wchar(i+1);

  }
  ppNewList[iSize]=NULL;
  *pppListHandle = ppNewList;
  return TRUE;
}

variableType** AddEventParameter(variableType** ppListHandle, WCHAR* pStrValue)
{
	(*ppListHandle)->value = pStrValue;
    ppListHandle++;
	return ppListHandle;
}


variableType** AddVariable(variableType** ppListHandle, WCHAR* pName, WCHAR* pStrValue)
{
	(*ppListHandle)->value = pStrValue;
	(*ppListHandle)->name = pName;
    ppListHandle++;
	return ppListHandle;
}

/*------------------------------------------------------------------------------------------
wtai conversion calls 
---------------------------------------------------------------------------------------------*/

variableType** wtaiEventIncomingCall(INT16 iId, const BYTE* pbCallerID)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(2, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = int2wchar(iId);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = wip_byte2wchar(pbCallerID);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
  /* DEALLOC( & pbCallerID);*/ /* TODO: not really good */
   return ppParameterList;
}

variableType** wtaiEventCallConnected(INT16 iId, const BYTE* pbCallerID)
{
   return wtaiEventIncomingCall(iId, pbCallerID);  /* since it is the same parameters */
}

variableType** wtaiEventOutgoingCall (INT16 iId, const BYTE* pbCallerID)
{
   return wtaiEventIncomingCall(iId, pbCallerID);  /* since it is the same parameters */
}


variableType** wtaiEventCallCleared(INT16 iId, INT8 iResult)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(2, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = int2wchar(iId);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = int2wchar(iResult);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   return ppParameterList;
}


variableType** wtaiEventMessageSendStatus(INT16 msgHandle, INT16 iId)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(2, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = int2wchar(msgHandle);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = int2wchar(iId);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   return ppParameterList;
}

variableType** wtaiEventCallAlerting (INT16 iId)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(1, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = int2wchar(iId);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   return ppParameterList;
}

variableType** wtaiEventCallHeld(INT16 iId)
{
   return wtaiEventCallAlerting(iId);
}

variableType** wtaiEventCallActive(INT16 iId)
{
   return wtaiEventCallAlerting(iId);
}

variableType** wtaiEventDTMFSent(INT16 callHandle, const WCHAR* pwchResultString)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(2, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = int2wchar(callHandle);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = GenCopyString(2,pwchResultString);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   return ppParameterList;
}

variableType** wtaiEventIncomingMessage (INT16 iId, const WCHAR* pwchSender)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(2, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = int2wchar(iId);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = GenCopyString(2,pwchSender);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   return ppParameterList;
}

variableType** wtaiEventNetworkStatus (BOOL fCamping, const WCHAR* pwchNetworkName, INT8 iNotCampingCause)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(3, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = int2wchar(fCamping);   /*TODO: what? */
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = GenCopyString(2,pwchNetworkName);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = int2wchar(iNotCampingCause);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   return ppParameterList;
}

variableType** wtaiEventUSSD_Received(const WCHAR* pwchUSSDString,const WCHAR* pwchUSSDDataCodingScheme, INT8 iType,const WCHAR* pwchId)
{
   WCHAR* pStrValue=NULL;
   variableType **ppParameterList = NULL;
   variableType **ppTempList = NULL;
   if (!CreateVariableList(4, & ppParameterList,TRUE))
      return NULL;
   ppTempList = ppParameterList;
   pStrValue = GenCopyString(2,pwchUSSDString);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = GenCopyString(2,pwchUSSDDataCodingScheme);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = int2wchar(iType);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   pStrValue = GenCopyString(2,pwchId);
   ppTempList = AddEventParameter(ppTempList, pStrValue);
   return ppParameterList;
}

#endif
