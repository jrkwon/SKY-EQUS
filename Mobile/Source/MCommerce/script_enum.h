#ifndef SCRIPT_ENUM__H
#define SCRIPT_ENUM__H

//---------------------------
//
//	script_enum.h
//
//	2000. 12
//
//	by Byung-Jun Yoon
//
//----------------------------

// SignatureAlgorithm

enum {	sigalgo_anonymous	= 0,
		sigalgo_ecdsa_sha	= 1,
		sigalgo_rsa_sha		= 2 };

// IdentifierType

enum {	idtype_null				= 0,
		idtype_text				= 1,
		idtype_binary			= 2,
		idtype_key_hash_sha		= 254,
		idtype_x509_name		= 255 };

// PublicKeyType

enum {	pkeytype_rsa		= 2,
		pkeytype_ecdh		= 3,
		pkeytype_ecdsa		= 4 };
		


// DataSignatureAlgorithm

enum {	algo_null			= 0,
		rsa_sha_pkcs1		= 1,
		ecdsa_sha			= 2,
		ecdsa_sha_p1363		= 2 };

// SignerInfoType

enum { 	implicit			= 0,
		sha_key_hash		= 1,
		wtls_certificate	= 2,
		x509_certificate	= 3,
		x968_certificate	= 4,
		certificate_url		= 5 };

// ContentType

enum {	contenttype_text	= 1,
		contenttype_data	= 2 };

// Boolean
		
enum {	bool_false	= 0,
		bool_true	= 1 };


// AttributeType

enum {	gmt_utc_time		= 1,
		signer_nonce		= 2 };

// CertificateRequestType

enum {	cert_signing_request	= 1,
		cert_revocation_request	= 2 };

// ReasonForRevocation

enum {	reason_anonymous		= 0,
		key_compromise			= 1,
		ca_compromise			= 2,
		affiliation_changed		= 3,
		superseded				= 4,
		cessation_of_operation	= 5 };

// KeyAgreementAlg

enum {	ka_algo_dh		= 1,
		ka_algo_ecdh	= 2,
		ka_algo_rsa		= 3 };

// ContentEncryptAlgorithm

enum {	des_ede3_cbc	= 1,
		rc5_cbc			= 2,
		seed_cbc		= 3 };

//----------------------
// Public Key Algorithm
//----------------------

enum {	PUBKEY_ALGO_RSA		= 0x0001,
		PUBKEY_ALGO_ECC		= 0x0002 };

		

//
//	End of script_enum.h
//
//-------------------------

#endif // SCRIPT_ENUM__H

