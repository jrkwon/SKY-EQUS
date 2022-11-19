#ifndef _ALERT_H
#define _ALERT_H

/* 	store the hash value computed over the record content to send */
void str_shash(SSLS *spSsls, ul ulHash);

/* 	store the hash value computed over the received record content */
void str_rhash(SSLS *spSsls, ul ulHash);

/* 
	check there exists a hash value computed over the sended record
	which equals to the received alert hash value 
*/
int chk_alert(SSLS *spSsls, char *cpAlertHash);

/*
	generate alert record 
	ucLevel: 	alert level(ALRT_FATAL or ALRT_WARNING)
	ucDesc:		alert description
	ucEnc:		encryption flag (ALERT_ENC or ALERT_NO_ENC)
*/
int gen_alert(uc *ucpAlert, uc ucLevel, uc ucDesc, SSLS *spSsls, uc ucEnc);

#endif
