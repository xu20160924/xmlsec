/** 
 * XMLSec library
 *
 * See Copyright for the status of this software.
 * 
 * Author: Aleksey Sanin <aleksey@aleksey.com>
 */
#include "globals.h"

#include <string.h>

#include <gnutls/gnutls.h>
#include <gcrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/transformsInternal.h>
#include <xmlsec/errors.h>

#include <xmlsec/gnutls/crypto.h>

static int		xmlSecGnuTLSKeysInit			(void);
static int		xmlSecGnuTLSTransformsInit		(void);

/**
 * xmlSecGnuTLSInit:
 * 
 * XMLSec library specific crypto engine initialization. 
 *
 * Returns 0 on success or a negative value otherwise.
 */
int 
xmlSecGnuTLSInit (void)  {
    if(xmlSecGnuTLSKeysInit() < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    NULL,
		    "xmlSecGnuTLSKeysInit",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
    if(xmlSecGnuTLSTransformsInit() < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    NULL,
		    "xmlSecGnuTLSTransformsInit",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
    
    return(0);
}

/**
 * xmlSecGnuTLSShutdown:
 * 
 * XMLSec library specific crypto engine shutdown. 
 *
 * Returns 0 on success or a negative value otherwise.
 */
int 
xmlSecGnuTLSShutdown(void) {
    return(0);
}

int
xmlSecGnuTLSGenerateRandom(xmlSecBufferPtr buffer, size_t size) {	
    int ret;
    
    xmlSecAssert2(buffer != NULL, -1);
    xmlSecAssert2(size > 0, -1);

    ret = xmlSecBufferSetSize(buffer, size);
    if(ret < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE, 
		    NULL,
		    "xmlSecBufferSetSize",
		    XMLSEC_ERRORS_R_MALLOC_FAILED,
		    "size=%d", size);
	return(-1);
    }
        
    /* get random data */
    gcry_randomize(xmlSecBufferGetData(buffer), size, GCRY_STRONG_RANDOM);
    return(0);
}


static int		
xmlSecGnuTLSKeysInit(void) {

#ifndef XMLSEC_NO_AES    
    if(xmlSecKeyDataIdsRegister(xmlSecGnuTLSKeyDataAesId) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(xmlSecGnuTLSKeyDataAesId)),
		    "xmlSecKeyDataIdsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES    
    if(xmlSecKeyDataIdsRegister(xmlSecGnuTLSKeyDataDesId) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(xmlSecGnuTLSKeyDataDesId)),
		    "xmlSecKeyDataIdsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_HMAC  
    if(xmlSecKeyDataIdsRegister(xmlSecGnuTLSKeyDataHmacId) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(xmlSecGnuTLSKeyDataHmacId)),
		    "xmlSecKeyDataIdsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
#endif /* XMLSEC_NO_HMAC */    

    return(0);
}

static int 
xmlSecGnuTLSTransformsInit(void) {

#ifndef XMLSEC_NO_HMAC
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformHmacSha1Id) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformHmacSha1Id)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformHmacRipemd160Id) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformHmacRipemd160Id)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformHmacMd5Id) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformHmacMd5Id)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
#endif /* XMLSEC_NO_HMAC */

#ifndef XMLSEC_NO_SHA1    
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformSha1Id) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformSha1Id)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_DES    
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformDes3CbcId) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformDes3CbcId)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES    
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformAes128CbcId) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformAes128CbcId)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformAes192CbcId) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformAes192CbcId)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
    if(xmlSecTransformsRegister(xmlSecGnuTLSTransformAes256CbcId) < 0) {
	xmlSecError(XMLSEC_ERRORS_HERE,
		    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecGnuTLSTransformAes256CbcId)),
		    "xmlSecTransformsRegister",
		    XMLSEC_ERRORS_R_XMLSEC_FAILED,
		    XMLSEC_ERRORS_NO_MESSAGE);
	return(-1);
    }
#endif /* XMLSEC_NO_AES */

    return(0);
}

