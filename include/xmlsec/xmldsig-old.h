typedef struct _xmlSecDSigOldCtx 			xmlSecDSigOldCtx, *xmlSecDSigOldCtxPtr; 
typedef struct _xmlSecDSigResult xmlSecDSigResult, *xmlSecDSigResultPtr;
typedef struct _xmlSecReferenceResult xmlSecReferenceResult, *xmlSecReferenceResultPtr;

/**
 * xmlSecDSigOldCtx:
 * @keysMngr: the keys manager #xmlSecKeysMngr.
 * @processManifests: if 0 then <dsig:Manifests> nodes are not processed.
 * @storeSignatures: store the signed content just (<dsig:SignedInfo> element)
 *	before applying signature.
 * @storeReferences: store the result of processing <dsig:Reference> nodes in 
 *      <dsig:SignedInfo> nodes just before digesting.
 * @storeManifests: store the result of processing <dsig:Reference> nodes in 
 *	<dsig:Manifest> nodes just before digesting (ignored if @processManifest is 0).
 * @fakeSignatures: for performance testing only.
 *
 * XML DSig context. 
 */
struct _xmlSecDSigOldCtx {
    int				processManifests;
    int				storeSignatures;
    int				storeReferences;
    int				storeManifests;	
    int				fakeSignatures;

    xmlSecKeyInfoCtx		keyInfoCtx;
    xmlSecTransformCtx		signatureTransformCtx;
};

/**
 * xmlSecDSigResult:
 * @ctx: the DSig context #xmlSecDSigOldCtx.
 * @context: the pointer to application specific data.
 * @self: the pointer to <dsig:Signature> node.
 * @sign: the sign/verify flag.
 * @result: the signature verification/generation status.
 * @signMethod: the signature algorithm .
 * @key: the pointer to signature key.
 * @firstSignRef: the pointer to the first <dsig:SignedInfo> reference result.
 * @lastSignRef: the pointer to the last  <dsig:SignedInfo> reference result.
 * @firstManifestRef: the pointer to the first <dsig:Manifest> reference result
 *    (valid only if the #processManifests flag in @ctx is set to 1).  
 * @lastManifestRef: the pointer to the last  <dsig:Manifest> reference result
 *    (valid only if the #processManifests flag in @ctx is set to 1).  
 * @buffer: the pointer to the signed content - the canonicalization of 
 *    <dsig:SignedInfo> node  (valid only if the #storeSignatures flag 
 *    in #ctx structure is set to 1).
 *
 * XML DSig Result.
 */
struct _xmlSecDSigResult {
    xmlSecDSigOldCtxPtr		ctx;
    void			*context;
    xmlNodePtr			self;
    int				sign;
    xmlSecTransformStatus	result;
    xmlSecTransformId		signMethod;
    xmlSecKeyPtr		key;
    xmlSecReferenceResultPtr	firstSignRef;
    xmlSecReferenceResultPtr	lastSignRef;
    xmlSecReferenceResultPtr	firstManifestRef;
    xmlSecReferenceResultPtr	lastManifestRef; 
    xmlSecBufferPtr		buffer;
};

/**
 * xmlSecReferenceType:
 * @xmlSecSignedInfoReference: reference in <dsig:SignedInfo> node.
 * @xmlSecManifestReference: reference <dsig:Manifest> node.
 * 
 * The possible <dsig:Reference> node locations: 
 * in the <dsig:SignedInfo> node or in the <dsig:Manifest> node.
 */
typedef enum  {
    xmlSecSignedInfoReference,
    xmlSecManifestReference
} xmlSecReferenceType;

/**
 * xmlSecReferenceResult:
 * @ctx: the pointer to DSig context.
 * @self: the pointer to <dsig:Reference> node.
 * @refType: the <dsig:Reference> node location.
 * @result: the verification/generation result.
 * @next: the next reference result.
 * @prev: the prev reference result.
 * @uri: the <dsig:Reference> node URI attribute.
 * @id: the <dsig:Reference> node Id attribute.
 * @type: the <dsig:Reference> node Type attribute.
 * @digestMethod: the used digest algorithm id.
 * @buffer: the pointer to digested content (valid only if 
 * 	the #storeReferences or #storeManifests flags in #xmlSecDSigOldCtx).
 *
 * The result of <dsig:Reference> processing.
 */
struct _xmlSecReferenceResult {
    xmlSecDSigOldCtxPtr		ctx;
    xmlNodePtr			self;
    xmlSecReferenceType		refType;
    xmlSecTransformStatus	result;
    xmlSecReferenceResultPtr	next;
    xmlSecReferenceResultPtr	prev;
    xmlChar			*uri;
    xmlChar			*id;
    xmlChar			*type;
    xmlSecTransformId		digestMethod;
    xmlSecBufferPtr		buffer;
}; 


/**
 * DSig context methods
 */
XMLSEC_EXPORT xmlSecDSigOldCtxPtr	xmlSecDSigOldCtxCreate		(xmlSecKeysMngrPtr keysMngr);
XMLSEC_EXPORT void 		xmlSecDSigOldCtxDestroy		(xmlSecDSigOldCtxPtr ctx);


/**
 * DSig generation/validation
 */
XMLSEC_EXPORT int		xmlSecDSigValidate		(xmlSecDSigOldCtxPtr ctx,
								 void *context,
								 xmlSecKeyPtr key,
								 xmlNodePtr signNode,
								 xmlSecDSigResultPtr *result);
XMLSEC_EXPORT int		xmlSecDSigGenerate		(xmlSecDSigOldCtxPtr ctx,
								 void *context,
								 xmlSecKeyPtr key,								 
								 xmlNodePtr signNode,
								 xmlSecDSigResultPtr *result);
/**
 * DSig results methods
 */
XMLSEC_EXPORT xmlSecDSigResultPtr xmlSecDSigResultCreate	(xmlSecDSigOldCtxPtr ctx,
								 void *context,
								 xmlNodePtr signNode,
								 int sign);
XMLSEC_EXPORT void		xmlSecDSigResultDestroy		(xmlSecDSigResultPtr result);
XMLSEC_EXPORT void		xmlSecDSigResultDebugDump	(xmlSecDSigResultPtr result,
								 FILE *output);
XMLSEC_EXPORT void		xmlSecDSigResultDebugXmlDump	(xmlSecDSigResultPtr result,
								 FILE *output);

