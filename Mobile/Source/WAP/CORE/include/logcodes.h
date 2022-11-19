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
#ifndef LOGCODES_H
#define LOGCODES_H
/******************************************************************************/
/*  File:           logcodes.h  */
/*  Description:    logCodes, to be used with CLNTa_log() */
/*  Created by:     */
/*  Project:        WIP, (Wap-stack Implementation Project) */
/******************************************************************************/


enum logCodes
{
    /*  to eliminate the message in CLNTa_log() */
    log_Undefined = 0,

    /***************************************************************************
       All log identifiers for a certain layer shall be put between the start
       and end constants for that particular layer.
       No assignment of values shall be performed.
       Ex:

       log_ex_start = 0,
       log_ex_signal1,
       log_ex_end = 99,

    ***************************************************************************/

    log_wae_start = 100,
    log_wae_sunitmethodinvokereq,
    log_wae_abortrequestwsp,
    log_wae_sunitmethodresultind,
    log_wae_suniterrorind,
    log_wae_abortrequestwspcnf,
    log_wae_sconnectreq,
    log_wae_sconnectcnf,
    log_wae_sdisconnectreq,
    log_wae_sdisconnectind,
    log_wae_smethodinvokereq,
    log_wae_smethodinvokecnf,
    log_wae_smethodresultind,
    log_wae_smethodresultres,
    log_wae_smethodabortreq,
    log_wae_smethodabortind,
    log_wae_sunitpushind,
    log_wae_spushind,
    log_wae_spushabortreq,
    log_wae_spushabortind,
    log_wae_sconfirmedpushind,
    log_wae_sconfirmedpushres,
    log_wae_closeport,
	log_wae_sackcnf,
	log_wae_sackres,

    log_wae_wmls_alertDialog,
    log_wae_wmls_confirmDialog,
    log_wae_wmls_promptDialog,
    log_wae_wmls_publicWTAI,
    log_wae_end = 199,


    log_wsp_cl_start = 200,
    log_wsp_TDUnitDataReq,
    log_wsp_TDUnitDataInd,
    log_wsp_TDErrorInd,
    log_wsp_cl_end = 299,


    log_wsp_co_start = 300,
    log_wsp_TRInvokeReq,
    log_wsp_TRInvokeRes,
    log_wsp_TRAbortReq,
    log_wsp_TRResultRes,
    log_wsp_TRInvokeCnf,
    log_wsp_TRResultInd,
    log_wsp_TRAbortInd,
    log_wsp_TRInvokeInd,
    log_wsp_ClosePort,
    log_wsp_co_end = 399,


    log_wtp_start = 400,
    log_wtp_TM_IDLE_TDUnitDataInd,
    log_wtp_TM_IDLE_TDUnitdataReq_0,
    log_wtp_TM_IDLE_TDUnitdataReq_1,
    log_wtp_TM_IDLE_TDUnitdataReq_2,
    log_wtp_procedure_TIDverification_TDUnitdataReq_0,
    log_wtp_Initiator_NULL_TDUnitdataReq_0,
    log_wtp_Initiator_NULL_TDUnitdataReq_1,
    log_wtp_Initiator_RESULTWAIT_TDUnitdataReq_0,
    log_wtp_Initiator_RESULTWAIT_TDUnitdataReq_1,
    log_wtp_Initiator_RESPONSTWAIT_TDUnitdataReq_0,
    log_wtp_Initiator_RESPONSTWAIT_TDUnitdataReq_1,
    log_wtp_Initiator_RESPONSTWAIT_TDUnitdataReq_2,
    log_wtp_Initiator_RESPONSTWAIT_TDUnitdataReq_3,
    log_wtp_Responder_LISTEN_TDUnitdataReq_0,
    log_wtp_Responder_LISTEN_TDUnitdataReq_1,
    log_wtp_Responder_TIDOKWAIT_TDUnitdataReq_0,
    log_wtp_Responder_INVRESPWAIT_TDUnitdataReq_0,
    log_wtp_Responder_INVRESPWAIT_TDUnitdataReq_1,
    log_wtp_Responder_INVRESPWAIT_TDUnitdataReq_2,
    log_wtp_Responder_INVRESPWAIT_TDUnitdataReq_3,
    log_wtp_SendUserAbortPdu,
    log_wtp_TM_CheckForTimeoutINI_TDUnitdataReq,
    log_wtp_TM_CheckForTimeoutRES_TDUnitdataReq,
    log_wtp_TM_ClosePort,
    log_wtp_end = 499,


    log_wtls_start = 500,
    log_wtls_TDUnitdataReq,
    log_wtls_TDUnitdataInd,
    log_wtls_CRYPTa_initialise,
    log_wtls_CRYPTa_getMethods,
    log_wtls_CRYPTa_encrypt,
    log_wtls_CRYPTa_decrypt,
    log_wtls_CRYPTa_hash,
    log_wtls_CRYPTa_hashInit,
    log_wtls_CRYPTa_hashUpdate,
    log_wtls_CRYPTa_hashFinal,
    log_wtls_CRYPTa_keyExchange,
    log_wtls_CRYPTa_PRF,
    log_wtls_CRYPTa_verifyCertificateChain,
    log_wtls_CRYPTa_getClientCertificate,
    log_wtls_CRYPTa_computeSignature,
    log_wtls_CRYPTa_generateRandom,
    log_wtls_CRYPTa_terminate,
    log_wtls_end = 599,


    log_wdp_start = 600,
    log_wdp_sendUDP,
    log_wdp_RecUDPDatagram,
    log_wdp_SendUDCP_PDU,
    log_wdp_RecUDCP_PDU,
    log_wdp_UDPErrorInd,
    log_wdp_sendSMS,
    log_wdp_SMSsentInd,
    log_wdp_SMSErrorInd,
    log_wdp_recSMSDatagram,
    log_wdp_FaultyUDHReceived,
    log_wdp_sendWD,
    log_wdp_RecWDDatagram,
    log_wdp_WDErrorInd,
    log_wdp_end = 699,


    log_udcp_start = 700,
    log_udcp_data_in_datalong,
    log_udcp_datalong_in_data_dialogue,
    log_udcp_Dialogue_kept,
    log_udcp_Error_DataLong_AddressType,
    log_udcp_Error_DCS,
    log_udcp_Error_NEI,
    log_udcp_ErrorConstructDataLpdu,
    log_udcp_ErrorConstructDatapdu,
    log_udcp_ErrorConstructProcUSSD,
    log_udcp_ErrorConstructRDpdu,
    log_udcp_ErrorConstructUSSDReq,
    log_udcp_ErrorErrorCodeToBig,
    log_udcp_ErrorGWAddressToLong,
    log_udcp_ErrorMustKeepDataPDUType,
    log_udcp_ErrorQueueFull,
    log_udcp_ErrorReleaseCodeToBig,
    log_udcp_ErrorTriedToTakeElementFromEmptyQueue,
    log_udcp_ErrorUdcpIEToLong,
    log_udcp_ErrorUDLorUDHLlengthToBig,
    log_udcp_ErrorUnknownSecAddressFormat,
    log_udcp_ErrorUSSDStringToLong,
    log_udcp_IE_UDCP_missed,
    log_udcp_not_RD_in_ProcUSSDReqCnf,
    log_udcp_PDUtype_RR,
    log_udcp_PDUtype_RD,
    log_udcp_PDUtype_Error,
    log_udcp_PDUtype_unknown,
    log_udcp_Portno_missing,
    log_udcp_ProcUSSDReqCnf,
    log_udcp_ProcUSSDReqCnf_in_NetworkInitiated,
    log_udcp_ProcUssdReqReq,
    log_udcp_RD_in_mobileinitiated,
    log_udcp_ResumeUDCP,
    log_udcp_StringDiv,
    log_udcp_StringErr,
    log_udcp_StringError,
    log_udcp_Stringlength_too_small,
    log_udcp_SuspendUDCP,
    log_udcp_TerminateUDCP,
    log_udcp_USSDErrorInd,
    log_udcp_USSDReleaseInd,
    log_udcp_UssdReqInd,
    log_udcp_UssdReqRes,
    log_udcp_VersionNumber_Wrong,
    log_udcp_end = 799
};


#ifndef __H2SDL__

typedef enum logCodes enumLogCode;

#endif


#endif
