#include "clnt.h"
#include "wap_mime.h"

extern void* WIPAlloc( unsigned long size );
extern void	WIPFree( void* block );


//SKTelecom's so called, secure mime will be added here for processing
//According to AU mannual wap core bypass contents which is not processed by WAE
#define CERT_RESPONSE_MIME "application/vnd.wap.skt-mc-cert-response"
#define HASHED_CA_CERT_RESPONSE_MIME "application/vnd.wap.skt-mc-hashed-certificate"
#define SIGNED_CA_CERT_RESPONSE_MIME "application/vnd.wap.skt-mc-signed-certificate"


int clnt_check_mime(const char *mime);
CERT_RESPONSE* clnt_process_user_cert(const char *data, int length, int* error);
TBHTrustedCAInfo* clnt_process_hashed_cert(const char *data, int length, int *error);
SignedTrustedCAInfo* clnt_process_signed_cert(const char *data, int length, int *error);

//I can not help adding this func prototype, because there is no header file
//including this prototype supported by SecureSoft.
void free_cert_response(CERT_RESPONSE * res);
void clnt_write_user_cert(CERT_RESPONSE *cert, int certIndex);
void clnt_free_user_cert(CERT_RESPONSE *cert);
int clnt_search_user_cert(char *skid, int* index);





