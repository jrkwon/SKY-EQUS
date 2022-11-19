#include "clnt.h"
#include "wap_mime.h"

extern void* WIPAlloc( unsigned long size );
extern void	WIPFree( void* block );

#define MW_FAILURE			0
#define MW_SUCCESS			1
#define MAX_TEMP_CERT		2
#define MAX_TCA_CERT		10
#define MAX_MY_CERT			5

#define MAX_TCA_CERT_SIZE	1500

//SKTelecom's so called, secure mime will be added here for processing
//According to AU mannual wap core bypass contents which is not processed by WAE
#define CERT_RESPONSE_MIME "application/vnd.wap.skt-mc-cert-response"
#define HASHED_CA_CERT_RESPONSE_MIME "application/vnd.wap.skt-mc-hashed-certificate"
#define SIGNED_CA_CERT_RESPONSE_MIME "application/vnd.wap.skt-mc-signed-certificate"

int clnt_check_mime(const char *mime);
int clnt_write_user_cert(CERT_RESPONSE *cert, int efs_pos,int log_pos);
CERT_RESPONSE* clnt_process_user_cert(const char *data, int length, int* error);
int clnt_search_skt_user_cert(char *akid, int* index);
int clnt_search_my_cert(char *skid, int* index);
int clnt_search_temp_cert(char *skid, int* index);
int clnt_search_tca_cert(char *skid);
char *clnt_get_tca_cert(char *skid);
int clnt_get_cert_index(int keyIDType, const unsigned char *keyID);
int clnt_search_user_cert_dn(char *subject_dn, int* index);

//I can not help adding this func prototype, because there is no header file
//including this prototype supported by SecureSoft.
void free_cert_response(CERT_RESPONSE * res);





