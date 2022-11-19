


#if !defined(__pirlmpias_h__)
#define		__pirlmpias_h__


#if !defined(IrDANOINCLUDEDCVER)
extern	const char	DeepCoreVersion[] ;
#endif	


#if !defined(IASATTRNAMEMAX)
#define	IASATTRNAMEMAX		32
#endif	
#if !defined(IASATTRVALUEMAX)
#define	IASATTRVALUEMAX		32
#endif	
#if !defined(IASANSWERBUFSZ)
#define	IASANSWERBUFSZ		(6+IASATTRVALUEMAX)
#endif	


typedef	struct	_IASATTR {
	uint	AttributeSize ;				
	byte	Attribute[IASATTRNAMEMAX] ;	
	uint	ValueSize ;					
	byte	Value[IASATTRVALUEMAX] ;	
	int		ClassId ;					
#if !defined(IASNOHIDDENCONTROL)
	IrDABOOL	hidden ;				
#endif	
}	IASATTR ;


#define	IAM_Used	0x0001


#if !defined(IASCLASSNAMEMAX)
#define	IASCLASSNAMEMAX		32
#endif	


typedef	struct	_IASCLASS	{
	uint	NameSize ;				
	byte	Name[IASCLASSNAMEMAX] ;	
#if !defined(IASNOHIDDENCONTROL)
	IrDABOOL	hidden ;				
#endif	
}	IASCLASS ;


#define	ICM_Used	0x0001


static	IASCLASS	IASClass[IASCLASSMAX] ;		
static	IASATTR		IASAttr[IASATTRMAX] ;		
static	byte		AnswerBuf[IASANSWERBUFSZ] ;	


typedef	struct	_IASCLIENT	{
	uint		Mode ;				
	dword		Address ;			
	int			LSAP ;				
	IASCALLBACK	icb ;				
}	IASCLIENT ;


#define	ISCM_Used			0x0001
#define	ISCM_WAITCONNECT	0x0002


static	IASCLIENT	IASClient[IASCLIENTMAX] ;	


static	void	lmpIAS_Open( void ) ;
static	int		IrDACB	lmpCbIAS_ConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB	lmpCbIAS_DataInd( int lsap, const void IrDAFAR *buf, uint size ) ;
static	void	IrDACB	lmpCbIAS_DisconnectInd( int lsap, int status, const void IrDAFAR *data, int datasz ) ;
#if defined(IrDAPRIMARY)
static	void	IrDACB	lmpCbIAS_C_ConnectCnf( int ssel, dword addr, int dsel, LMCONNDAT IrDAFAR *lcd, int status ) ;
static	void	IrDACB	lmpCbIAS_C_DataInd( int lsap, const void IrDAFAR *buf, uint size ) ;
static	void	IrDACB	lmpCbIAS_C_DisconnectInd( int lsap, int status, const void IrDAFAR *data, int datasz ) ;
#endif	
static	void	lmpGetValueByClass( int lsap, int size, byte IrDAFAR *ptr ) ;
static	uint	lmpGetEncodedValueSize( const byte IrDAFAR *value ) ;
static	int		lmpFindClass( const byte IrDAFAR *classnm, uint classsz ) ;
static	int		lmpFindAttribute( int classid, const byte IrDAFAR *attrnm, uint attrsz ) ;


int		IrDAAPI	IrLMP_AddClass( const void IrDAFAR *classnm, uint classsz )
{
	int		i ;
	int		classid ;


#if	defined(DMSG)
{	int		j, c ;
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_AddClass():" ) ;
	_DPUTW( DL_LMPINFO, " classsz:", classsz ) ;
	_DPUTS( DL_LMPINFO, " class:" ) ;
	for( j=0 ; j<classsz ; j++ ) {
		c		= ((byte IrDAFAR *)classnm)[j] ;
		if( c < ' ' || 0x7F < c ) {
			_DPUTB( DL_LMPINFO, "[", c ) ;
			_DPUTC( DL_LMPINFO, ']' ) ;
		} else {
			_DPUTC( DL_LMPINFO, c ) ;
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	


	if( classsz == 0 )
		return( IrDAR_INVALID ) ;
	classid	= lmpFindClass((const byte IrDAFAR *)classnm, classsz ) ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_AddClass():" ) ;
	if( classid >= 0 )
		_DPUTS( DL_LMPINFO, " found" ) ;
	else
		_DPUTS( DL_LMPINFO, " not found" ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( classid >= 0 )
		return( classid ) ;


	
	for( i=0 ; i<IASCLASSMAX ; i++ )
		if( IASClass[i].NameSize == 0 )
			break ;
#if	defined(DMSG)
	if( i == IASCLASSMAX ) {
		_DPUTS( DL_LMPINFO, " LMP::IrLMP_AddClass():" ) ;
		_DPUTS( DL_LMPINFO, " resource not found" ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
	}
#endif	
	if( i == IASCLASSMAX )
		return( IrDAR_NORESOURCE ) ;


	IrDA_memcpy( IASClass[i].Name, classnm, IASCLASSNAMEMAX ) ;
#if !defined(IASNOHIDDENCONTROL)
	IASClass[i].hidden		= FALSE ;
#endif	
	if( classsz < IASCLASSNAMEMAX )
		IASClass[i].NameSize	= classsz ;
	else
		IASClass[i].NameSize	= IASCLASSNAMEMAX ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_AddClass():" ) ;
	_DPUTS( DL_LMPINFO, " accept" ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	return( i ) ;
}


int		IrDAAPI	IrLMP_DeleteClass( int classid )
{
	int		i ;


	if( classid < 0 || IASCLASSMAX <= classid )
		return( IrDAR_INVALID ) ;
	if( IASClass[classid].NameSize == 0 )
		return( IrDAR_INVALID ) ;


	for( i=0 ; i<IASATTRMAX ; i++ ) {
		if( IASAttr[i].AttributeSize == 0 || IASAttr[i].ClassId != classid )
			continue ;
		IASAttr[i].AttributeSize	= 0 ;
		IASAttr[i].ClassId			= -1 ;
	}


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_AddAttribute( int classid, const void IrDAFAR *attrnm, uint attrsz )
{
	int		i ;
	int		attrid ;


	attrid	= lmpFindAttribute( classid, (const byte IrDAFAR*)attrnm, attrsz ) ;
	if( attrid >= 0 )
		return( attrid ) ;


	if( classid < 0 || IASCLASSMAX <= classid )
		return( IrDAR_INVALID ) ;
	if( IASClass[classid].NameSize == 0 )
		return( IrDAR_INVALID ) ;


	for( i=0 ; i<IASATTRMAX ; i++ )
		if( IASAttr[i].AttributeSize == 0 )
			break ;
	if( i >= IASATTRMAX )
		return( IrDAR_NORESOURCE ) ;


	if( attrsz >= IASATTRNAMEMAX )
		attrsz	= IASATTRNAMEMAX ;


	IASAttr[i].ClassId			= classid ;
#if !defined(IASNOHIDDENCONTROL)
	IASAttr[i].hidden		= FALSE ;
#endif	
	IrDA_memcpy( IASAttr[i].Attribute, attrnm, attrsz ) ;
	IASAttr[i].ValueSize		= 0 ;
	IASAttr[i].AttributeSize	= attrsz ;
#if	defined(DMSG)
{	int		j, c ;
	if( IASAttr[i].AttributeSize > 0 ) {
		_DPUTS( DL_LMPINFO, " LMP::IrLMP_AddAttribute():" ) ;
		_DPUTW( DL_LMPINFO, " attrsz:", IASAttr[i].AttributeSize ) ;
		_DPUTS( DL_LMPINFO, " attr:" ) ;
		for( j=0 ; j<IASAttr[i].AttributeSize ; j++ ) {
			c		= IASAttr[i].Attribute[j] ;
			if( c < ' ' || 0x7F < c ) {
				_DPUTB( DL_LMPINFO, "[", c ) ;
				_DPUTC( DL_LMPINFO, ']' ) ;
			} else {
				_DPUTC( DL_LMPINFO, c ) ;
			}
		}
		_DPUTS( DL_LMPINFO, "\n" ) ;
	}
}
#endif	


	return( i ) ;
}


int		IrDAAPI	IrLMP_DeleteAttribute( int attrid )
{
	if( attrid < 0 || IASATTRMAX <= attrid )
		return( IrDAR_INVALID ) ;
	if( IASAttr[attrid].AttributeSize == 0 )
		return( IrDAR_INVALID ) ;


	IASAttr[attrid].AttributeSize	= 0 ;
	IASAttr[attrid].ValueSize		= 0 ;
	IASAttr[attrid].ClassId			= -1 ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_SetAttribute( int attrid, const void IrDAFAR *value, uint valuesz )
{
	if( attrid < 0 || IASATTRMAX <= attrid )
		return( IrDAR_INVALID ) ;
	if( IASAttr[attrid].AttributeSize == 0 )
		return( IrDAR_INVALID ) ;


	if( valuesz >= IASATTRVALUEMAX )
		valuesz	= IASATTRVALUEMAX ;


	IrDA_memcpy( IASAttr[attrid].Value, value, valuesz ) ;
	IASAttr[attrid].ValueSize	= valuesz ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_SetIntAttribute( int attrid, int val )
{
	if( attrid < 0 || IASATTRMAX <= attrid )
		return( IrDAR_INVALID ) ;
	if( IASAttr[attrid].AttributeSize == 0 )
		return( IrDAR_INVALID ) ;


	IASAttr[attrid].ValueSize	= lmpSetInteger( IASAttr[attrid].Value, val ) ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_SetOctetAttribute( int attrid, const void IrDAFAR *value, uint valuesz )
{
	if( attrid < 0 || IASATTRMAX <= attrid )
		return( IrDAR_INVALID ) ;
	if( IASAttr[attrid].AttributeSize == 0 )
		return( IrDAR_INVALID ) ;


	if( valuesz >= IASATTRVALUEMAX-3 )
		valuesz	= IASATTRVALUEMAX-3 ;


#if defined(DMSG)
{	int	i ;
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_SetOctetAttribute():" ) ;
	_DPUTB( DL_LMPINFO, " value:", ((byte IrDAFAR*)value)[0] ) ;
	for( i=1 ; i<valuesz ; i++ )
		_DPUTB( DL_LMPINFO, ".", ((byte IrDAFAR*)value)[i] ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	IASAttr[attrid].ValueSize	= lmpSetOctetSequence( IASAttr[attrid].Value, value, valuesz ) ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_SetUserStringAttribute( int attrid, uint charset, const void IrDAFAR *value, uint valuesz )
{
	if( attrid < 0 || IASATTRMAX <= attrid )
		return( IrDAR_INVALID ) ;
	if( IASAttr[attrid].AttributeSize == 0 )
		return( IrDAR_INVALID ) ;


	if( valuesz >= IASATTRVALUEMAX-3 )
		valuesz	= IASATTRVALUEMAX-3 ;


#if	defined(DMSG)
{	int		j, c ;
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_SetUserStringAttribute():" ) ;
	_DPUTW( DL_LMPINFO, " attrid:", attrid ) ;
	_DPUTW( DL_LMPINFO, " valuesz:", valuesz ) ;
	_DPUTS( DL_LMPINFO, " value:" ) ;
	for( j=0 ; j<valuesz ; j++ ) {
		c		= ((byte IrDAFAR*)value)[j] ;
		if( c < ' ' || 0x7F < c ) {
			_DPUTB( DL_LMPINFO, "[", c ) ;
			_DPUTC( DL_LMPINFO, ']' ) ;
		} else {
			_DPUTC( DL_LMPINFO, c ) ;
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	IASAttr[attrid].ValueSize		= valuesz+3 ;
	lmpSetUserString( IASAttr[attrid].Value, charset, value, valuesz ) ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_SetLSAPAttribute( int attrid, int lsap )
{
	return( IrLMP_SetIntAttribute( attrid, lsap+1 )) ;
}


#if !defined(IASNOHIDDENCONTROL)


int		IrDAAPI	IrLMP_ShowAttribute( int attrid )
{
	IrDABOOL		oHidden ;


	if( attrid < 0 || IASATTRMAX <= attrid )
		return( IrDAR_INVALID ) ;
	if( IASAttr[attrid].AttributeSize == 0 )
		return( IrDAR_INVALID ) ;
	oHidden						= IASAttr[attrid].hidden ;
	IASAttr[attrid].hidden		= FALSE ;


	if( oHidden )
		return( 0 ) ;
	return( 1 ) ;
}
#endif	


#if !defined(IASNOHIDDENCONTROL)


int		IrDAAPI	IrLMP_HiddenAttribute( int attrid )
{
	IrDABOOL		oHidden ;


	if( attrid < 0 || IASATTRMAX <= attrid )
		return( IrDAR_INVALID ) ;
	if( IASAttr[attrid].AttributeSize == 0 )
		return( IrDAR_INVALID ) ;
	oHidden						= IASAttr[attrid].hidden ;
	IASAttr[attrid].hidden		= TRUE ;


	if( oHidden )
		return( 0 ) ;
	return( 1 ) ;
}
#endif	


#if !defined(IASNOHIDDENCONTROL)


int		IrDAAPI	IrLMP_ShowClass( int classid )
{
	IrDABOOL		oHidden ;


	if( classid < 0 || IASCLASSMAX <= classid )
		return( IrDAR_INVALID ) ;
	if( IASClass[classid].NameSize == 0 )
		return( IrDAR_INVALID ) ;


	oHidden						= IASClass[classid].hidden ;
	IASClass[classid].hidden	= FALSE ;


	if( oHidden )
		return( 0 ) ;
	return( 1 ) ;
}
#endif	


#if !defined(IASNOHIDDENCONTROL)


int		IrDAAPI	IrLMP_HiddenClass( int classid )
{
	IrDABOOL		oHidden ;


	if( classid < 0 || IASCLASSMAX <= classid )
		return( IrDAR_INVALID ) ;
	if( IASClass[classid].NameSize == 0 )
		return( IrDAR_INVALID ) ;


	oHidden						= IASClass[classid].hidden ;
	IASClass[classid].hidden	= TRUE ;


	if( oHidden )
		return( 0 ) ;
	return( 1 ) ;
}
#endif	


#if defined(IrDAPRIMARY)


int		IrDAAPI	IrLMP_OpenIAS( dword addr, IASCALLBACK IrDAFAR *icb )
{
	int			lsap ;
	int			ret ;
	int			i ;
	LMCALLBACK	lmc ;


#if	defined(DMSG)
	for( i=0 ; i<IASCLIENTMAX ; i++ ) {
		_DPUTS( DL_LMPINFO, " IAS::IrLMP_OpenIAS():" ) ;
		_DPUTW( DL_LMPINFO, " IASClinet[", i ) ;
		_DPUTW( DL_LMPINFO, "].Mode:", IASClient[i].Mode ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
	}
#endif	
	for( i=0 ; i<IASCLIENTMAX ; i++ )
		if(( IASClient[i].Mode & ISCM_Used ) == 0 )
			break ;
	if( i >= IASCLIENTMAX )
		return( IrDAR_NORESOURCE ) ;


	lsap				= IrLMP_RequestLSAP() ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " IAS::IrLMP_OpenIAS():" ) ;
	_DPUTW( DL_LMPINFO, " IrLMP_RequestLSAP():", lsap ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( lsap < 0 )
		return( lsap ) ;


	IASClient[i].Mode		|= ISCM_Used ;
	IASClient[i].Address	= addr ;
	IASClient[i].LSAP		= lsap ;
	IrDA_memcpy( &IASClient[i].icb, icb, sizeof( IASCALLBACK )) ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " IAS::IrLMP_OpenIAS()" ) ;
	_DPUTB( DL_LMPINFO, " IASClient[", i ) ;
	_DPUTD( DL_LMPINFO, "].icb.ConnectCnf:", (dword)IASClient[i].icb.ConnectCnf ) ;
	_DPUTD( DL_LMPINFO, " icb->ConnectCnf:", (dword)icb->ConnectCnf ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	


	IrDA_memset( &lmc, 0, sizeof( LMCALLBACK )) ;
	lmc.ConnectInd		= 0 ;
	lmc.ConnectCnf		= lmpCbIAS_C_ConnectCnf ;
	lmc.DataInd			= lmpCbIAS_C_DataInd ;
	lmc.DisconnectInd	= lmpCbIAS_C_DisconnectInd ;
	IrLMP_BindLSAP( IASClient[i].LSAP, &lmc ) ;


	ret		= IrLMP_ConnectReq( addr, IASClient[i].LSAP, 0, 0, 0 ) ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " IAS::IrLMP_OpenIAS():" ) ;
	_DPUTW( DL_LMPINFO, " IrLMP_ConnectReq()=", (word)ret ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( ret != IrDAR_ACCEPT ) {
		IrLMP_UnbindLSAP( IASClient[i].LSAP ) ;
		IASClient[i].Mode	&= ~ISCM_Used ;
		return( ret ) ;
	}


	IASClient[i].Mode		|= ISCM_WAITCONNECT ;
	return( i ) ;
}
#endif	


int		IrDAAPI	IrLMP_CloseIAS( int lsap )
{
#if 0
	int		ret ;
#endif


	if(( IASClient[lsap].Mode & ISCM_Used ) == 0 )
		return( IrDAR_INVALID ) ;


#if 0
	if(( ret = IrLMP_DisconnectReq( IASClient[lsap].LSAP, 0, 0, 0 )) != IrDAR_ACCEPT )
		return( ret ) ;
#endif


	IrLMP_UnbindLSAP( IASClient[lsap].LSAP ) ;
	IASClient[lsap].Mode	&= ~(ISCM_Used|ISCM_WAITCONNECT) ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_GetValueByClass( int lsap, const void IrDAFAR *classnm, uint classsz, const void IrDAFAR *attr, uint attrsz )
{
	byte	buf[64] ;
	int		i ;


	if( classsz == 0 || attrsz == 0 )
		return( IrDAR_EPARM ) ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_GetValueByClass()" ) ;
	_DPUTB( DL_LMPINFO, " IASClient[", lsap ) ;
	_DPUTW( DL_LMPINFO, "].Mode:", IASClient[lsap].Mode ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
	_DPUTS( DL_LMPINFO, "                             " ) ;
	{	int		c, p ;
		_DPUTB( DL_LMPINFO, " class=", ((byte IrDAFAR*)classnm)[0] ) ;
		for( p=1 ; p<classsz ; p++ ) {
			if(( c = ((byte IrDAFAR*)classnm)[p] ) < ' ' ) {
				_DPUTB( DL_LMPINFO, "[", c ) ;
				_DPUTC( DL_LMPINFO, ']' ) ;
			} else {
				_DPUTC( DL_LMPINFO, c ) ;
			}
		}
		_DPUTB( DL_LMPINFO, " attr=", ((byte IrDAFAR*)attr)[0] ) ;
		for( p=1 ; p<attrsz ; p++ ) {
			if(( c = ((byte IrDAFAR*)attr)[p] ) < ' ' ) {
				_DPUTB( DL_LMPINFO, "[", c ) ;
				_DPUTC( DL_LMPINFO, ']' ) ;
			} else {
				_DPUTC( DL_LMPINFO, c ) ;
			}
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if(( IASClient[lsap].Mode & ISCM_Used ) == 0 )
		return( IrDAR_INVALID ) ;


	i			= 0 ;
	buf[i++]	= 0x04 | 0x80 ;						
	buf[i++]	= classsz ;
	IrDA_memcpy( &buf[i], classnm, classsz ) ;
	i			+= classsz ;
	buf[i++]	= attrsz ;
	IrDA_memcpy( &buf[i], attr, attrsz ) ;
	i			+= attrsz ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_GetValueByClass()" ) ;
	{	int		p ;
		_DPUTB( DL_LMPINFO, " buf=", buf[0] ) ;
		for( p=1 ; p<i ; p++ )
			_DPUTB( DL_LMPINFO, " ", buf[p] ) ;
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	return( IrLMP_DataReq( IASClient[lsap].LSAP, buf, i )) ;
}


int		IrDAAPI	IrLMP_QueryReturnCode( byte IrDAFAR *value, int valuesz )
{
#if	defined(DMSG)
	int		i ;
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_QueryReturnCode()" ) ;
	_DPUTW( DL_LMPINFO, " valuesz=", valuesz ) ;
	_DPUTB( DL_LMPINFO, " dat=", value[0] ) ;
	for( i=1 ; i<valuesz ; i++ )
		_DPUTB( DL_LMPINFO, ".", value[i] ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( valuesz < 2 )
		return( IrDAR_INVALID ) ;


	return( value[1] ) ;
}


int		IrDAAPI	IrLMP_QueryValueByClass( byte IrDAFAR *value, int valuesz, IASVALUEBYCLASS IrDAFAR *listp, uint listsz )
{
	word	tmpw ;
	uint	i ;


	if( valuesz < 4 )
		return( IrDAR_INVALID ) ;


	if(( value[0] & 0x3F ) != 4 )
		return( IrDAR_INVALID ) ;


	if( value[1] != 0 )
		return( IrDAR_INVALID ) ;


	MiscGetBigEndian16( &value[2], &tmpw ) ;			
	value	+= 4 ;
	valuesz	-= 4 ;
	if( listsz > tmpw )
		listsz	= tmpw ;


	for( i=0 ; i<listsz ; i++ ) {
		MiscGetBigEndian16( value, &tmpw ) ;
		listp[i].ObjectIdentifier	= tmpw ;
		listp[i].EncodedValue		= &value[2] ;
		tmpw	= lmpGetEncodedValueSize( listp[i].EncodedValue ) ;
		value	+= 2 + tmpw ;
		valuesz	-= 2 + tmpw ;
	}


	return( listsz ) ;
}


int		IrDAAPI	IrLMP_GetIntValue( byte IrDAFAR *value, dword IrDAFAR *valp )
{
	if( value[0] != 1 )
		return( IrDAR_INVALID ) ;


	MiscGetBigEndian32( &value[1], valp ) ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_GetOctetValue( byte IrDAFAR *value, word IrDAFAR *valsz, byte IrDAFAR * IrDAFAR *valp )
{
	if( value[0] != 2 )
		return( IrDAR_INVALID ) ;


	MiscGetBigEndian16( &value[1], valsz ) ;
	*valp	= &value[3] ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_GetUserStringValue( byte IrDAFAR *value, int IrDAFAR *charset, byte IrDAFAR * IrDAFAR *valp )
{
	int		len ;


#if	defined(DMSG2)
{	int		i ;
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_GetUserStringValue(): value=" ) ;
	for( i=0 ; i<3 ; i++ )
		_DPUTS( DL_LMPINFO, "%02X ", value[i] ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	if( value[0] != 3 )
		return( IrDAR_INVALID ) ;


	*charset	= value[1] ;
	len			= value[2] ;
	*valp		= &value[3] ;
	return( len ) ;
}


#if !defined(IrDANOSTATIC)


static	void	lmpIAS_Open( void )
{
	int				i ;
	int				classid, attrid ;
	LMCALLBACK		lmc ;
	byte			buf[3] ;


	
	for( i=0 ; i<IASCLASSMAX ; i++ )
		IASClass[i].NameSize	= 0 ;


	for( i=0 ; i<IASATTRMAX ; i++ ) {
		IASAttr[i].AttributeSize	= 0 ;
		IASAttr[i].ValueSize		= 0 ;
		IASAttr[i].ClassId			= -1 ;
	}


	
	for( i=0 ; i<IASCLIENTMAX ; i++ )
		IASClient[i].Mode	= 0 ;


	
	classid		= IrLMP_AddClass( "Device", 6 ) ;
	attrid		= IrLMP_AddAttribute( classid, "DeviceName", 10 ) ;
	IrLMP_SetUserStringAttribute( attrid, IrLMP_CS_ASCII, "LEGASIC", 7+1 ) ;
	attrid		= IrLMP_AddAttribute( classid, "IrLMPSupport", 12 ) ;
	buf[0]		= 1 ;								
	buf[1]		= 0 ;								
	buf[2]		= 0 ;								
#if	defined(DMSG2)
	_DPUTS( DL_LMPDAT, " LMP::lmpIAS_Open(): true1\n" ) ;
#endif	
	IrLMP_SetOctetAttribute( attrid, buf, 3 ) ;


	
	IrDA_memset( &lmc, 0, sizeof( LMCALLBACK )) ;
	lmc.ConnectInd		= lmpCbIAS_ConnectInd ;
	lmc.ConnectCnf		= 0 ;
	lmc.DataInd			= lmpCbIAS_DataInd ;
	lmc.DisconnectInd	= lmpCbIAS_DisconnectInd ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " IAS::lmpIAS_Open():" ) ;
	_DPUTD( DL_LMPINFO, " lmc.ConnectInd:", (dword)lmc.ConnectInd ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	lmpLSAP_Bind( 0, &lmc ) ;


#if !defined(IrDANOINCLUDEDCVER)
	for( i=0 ; DeepCoreVersion[i] ; i++ ) ;
	classid	= IrLMP_AddClass((const void IrDAFAR*)"IrDA:OSW", 8 ) ;
	attrid	= IrLMP_AddAttribute( classid, (const void IrDAFAR*)"DeepCoreVersion", 15 ) ;
	IrLMP_SetUserStringAttribute( attrid, IrLMP_CS_ASCII, DeepCoreVersion, i+1 ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		IrDACB lmpCbIAS_ConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd )
{
#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_LMPINFO, " IAS::lmpCbIAS_ConnectInd():" ) ;
	_DPUTD( DL_LMPINFO, " from ", addr  ) ;
	_DPUTB( DL_LMPINFO, "/", dlsap ) ;
	_DPUTW( DL_LMPINFO, " size:", lcd->lmpDataSize ) ;


	if( lcd->lmpDataSize > 0 ) {
		_DPUTB( DL_LMPINFO, " data=", ((byte IrDAFAR*)lcd->Data)[0] ) ;
		for( i=0 ; i<lcd->lmpDataSize ; i++ )
			_DPUTB( DL_LMPINFO, ".", ((byte IrDAFAR*)lcd->Data)[i] ) ;
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	UNREFERENCE( slsap ) ;
	UNREFERENCE( addr ) ;
	UNREFERENCE( dlsap ) ;
	UNREFERENCE( lcd ) ;
	return( IrDA_CONNECTEN ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpCbIAS_DataInd( int lsap, const void IrDAFAR *buf, uint size )
{
	int		lst, ack, opc ;
	byte	tmp[2] ;
	byte IrDAFAR	*ptr ;


	ptr		= (byte IrDAFAR *)buf ;
#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_LMPDAT, " IAS::lmpCbIAS_DataInd():" ) ;
	_DPUTW( DL_LMPDAT, " size:", size ) ;
	if( size > 0 ) {
		_DPUTB( DL_LMPDAT, " data:", ptr[0] ) ;
		for( i=1 ; i<size ; i++ )
			_DPUTB( DL_LMPDAT, ".", ptr[i] ) ;
	}
	_DPUTS( DL_LMPDAT, "\n" ) ;
}
#endif	
	lst		= ptr[0] & 0x80 ;
	ack		= ptr[0] & 0x40 ;
	opc		= ptr[0] & 0x3F ;


	lsap	++ ;
#if	defined(DMSG)
{	static	IrDAFAR char IrDAFAR *opnm[]	= {
					"GetInfoBaseDetails", "GetObjects","GetValue",
					"GetValueByClass","GetObjectInfo","GetAttributeNames" } ;
	_DPUTS( DL_LMPINFO, " IAS::lmpCbIAS_DataInd():" ) ;
	_DPUTS( DL_LMPINFO, " accept " ) ;
	_DPUTS( DL_LMPINFO, opnm[opc-1] ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	if( ack )										
		return ;


	switch( opc ) {
	case	4 :										
		lmpGetValueByClass( lsap, size-1, &ptr[1] ) ;
		return ;
#if 0	
	case	1 :										
	case	2 :										
	case	3 :										
	case	5 :										
	case	6 :										
#endif	
	}


	
	tmp[0]		= 0x80 | opc ;
	tmp[1]		= 0xFF ;							
	lmpLSAP_DataReq( lsap, tmp, 2 ) ;


	lst		= lst ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpCbIAS_DisconnectInd( int lsap, int status, const void IrDAFAR *data, int datasz )
{
	UNREFERENCE( lsap ) ;
	UNREFERENCE( status ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	IrDACB	lmpCbIAS_C_ConnectCnf( int ssel, dword addr, int dsel, LMCONNDAT IrDAFAR *lcd, int status )
{
	int		i ;


	UNREFERENCE( addr ) ;
	UNREFERENCE( dsel ) ;
	UNREFERENCE( lcd ) ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbIAS_C_ConnectCnf():" ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	for( i=0 ; i<IASCLIENTMAX ; i++ ) {
		if(( IASClient[i].Mode & ISCM_Used ) &&
			 IASClient[i].LSAP == ssel )
			break ;
	}
	if( i >= IASCLIENTMAX )
		return ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbIAS_C_ConnectCnf():" ) ;
	_DPUTB( DL_LMPINFO, "IASClient[", i ) ;
	_DPUTD( DL_LMPINFO, "].icb.ConnectCnf", (dword)IASClient[i].icb.ConnectCnf ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;


#endif	
	IASClient[i].Mode	&= ~ISCM_WAITCONNECT ;
	if( status != IrDAR_DONE && status != IrDAR_CONNECTING ) {
		IrLMP_UnbindLSAP( IASClient[i].LSAP ) ;
		IASClient[i].Mode	&= ~ISCM_Used ;
	}
	if( IASClient[i].icb.ConnectCnf )
		(*IASClient[i].icb.ConnectCnf)( i, status ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	IrDACB	lmpCbIAS_C_DataInd( int lsap, const void IrDAFAR *buf, uint size )
{
	int		i ;
	int		opcode ;


	for( i=0 ; i<IASCLIENTMAX ; i++ ) {
		if(( IASClient[i].Mode & ISCM_Used ) &&
			 IASClient[i].LSAP == lsap )
			break ;
	}
	if( i >= IASCLIENTMAX )
		return ;


	opcode	= ((byte IrDAFAR*)buf)[0] & 0x3F ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " IAS::lmpCbIAS_C_DataInd():" ) ;
	_DPUTW( DL_LMPINFO, " i:", i ) ;
	_DPUTB( DL_LMPINFO, " opcode:", opcode ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( opcode < 1 || 6 < opcode )
		return ;


	if( IASClient[i].icb.ValueCnf )
		(*IASClient[i].icb.ValueCnf)( i, (byte IrDAFAR*)buf, size ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	IrDACB	lmpCbIAS_C_DisconnectInd( int lsap, int status, const void IrDAFAR *data, int datasz )
{
	int		i ;
	int		ret ;
	byte	buf ;


	UNREFERENCE( status ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;


	for( i=0 ; i<IASCLIENTMAX ; i++ ) {
		if(( IASClient[i].Mode & ISCM_Used ) &&
			 IASClient[i].LSAP == lsap )
			break ;
	}
	if( i >= IASCLIENTMAX )
		return ;


	if( IASClient[i].Mode & ISCM_WAITCONNECT ) {
		ret		= lmpSetMissing( &buf ) ;
		ret		= IrLMP_ConnectReq( IASClient[i].Address, IASClient[i].LSAP, 0, &buf, ret ) ;
		if( ret == IrDAR_ACCEPT )
			return ;
	}


	IrLMP_UnbindLSAP( IASClient[i].LSAP ) ;
	IASClient[i].Mode	= 0 ;
	if( IASClient[i].icb.DisconnectInd )
		(*IASClient[i].icb.DisconnectInd)( i ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpGetValueByClass( int lsap, int size, byte IrDAFAR *ptr )
{
	int		classsz, attrsz ;
	byte IrDAFAR	*classp ;
	byte IrDAFAR	*attrp ;
	int		classid, attrid ;
	int		len ;


	size	= size ;


	classsz	= ptr[0] ;
	classp	= &ptr[1] ;
	attrsz	= ptr[1+classsz] ;
	attrp	= &ptr[1+classsz+1] ;
#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_LMPDAT, " IAS::lmpGetValueByClass():" ) ;
	_DPUTW( DL_LMPDAT, " size:", size ) ;
	_DPUTS( DL_LMPDAT, " data=" ) ;
	if( size > 0 ) {
		_DPUTB( DL_LMPDAT, "", ptr[0] ) ;
		for( i=1 ; i<size ; i++ )
			_DPUTB( DL_LMPDAT, ".", ptr[i] ) ;
	}
	_DPUTS( DL_LMPDAT, "\n" ) ;


	_DPUTS( DL_LMPINFO, " IAS::lmpGetValueByClass():" ) ;
	_DPUTW( DL_LMPINFO, " classsz:", classsz ) ;
	_DPUTS( DL_LMPINFO, " class=" ) ;
	for( i=0 ; i<classsz ; i++ ) {
		if( classp[i] < ' ' ) {
			_DPUTB( DL_LMPINFO, "[", classp[i] ) ;
			_DPUTC( DL_LMPINFO, ']' ) ;
		} else {
			_DPUTC( DL_LMPINFO, classp[i] ) ;
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;


	_DPUTS( DL_LMPINFO, "                       :" ) ;
	_DPUTW( DL_LMPINFO, " attrsz:", attrsz ) ;
	_DPUTS( DL_LMPINFO, " attr=" ) ;
	for( i=0 ; i<attrsz ; i++ ) {
		if( attrp[i] < ' ' ) {
			_DPUTB( DL_LMPINFO, "[", attrp[i] ) ;
			_DPUTC( DL_LMPINFO, ']' ) ;
		} else {
			_DPUTC( DL_LMPINFO, attrp[i] ) ;
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;


}
#endif	
	len					= 0 ;
	AnswerBuf[len++]	= 0x80 | 0x04 ;


	classid		= lmpFindClass( classp, classsz ) ;
	if( classid < 0 ) {
#if	defined(DMSG)
_DPUTS( DL_LMPINFO, " IAS::lmpGetValueByClass(): not found class\n" ) ;
#endif	
		AnswerBuf[len++]	= 0x01 ;				
		lmpLSAP_DataReq( lsap, AnswerBuf, len ) ;
		return ;
	}
#if !defined(IASNOHIDDENCONTROL)
	if( IASClass[classid].hidden ) {
#if	defined(DMSG)
_DPUTS( DL_LMPINFO, " IAS::lmpGetValueByClass(): class is hidden\n" ) ;
#endif	
		AnswerBuf[len++]	= 0x01 ;				
		lmpLSAP_DataReq( lsap, AnswerBuf, len ) ;
		return ;
	}
#endif	


	attrid		= lmpFindAttribute( classid, attrp, attrsz ) ;
	if( attrid < 0 ) {
#if	defined(DMSG)
_DPUTS( DL_LMPINFO, " IAS::lmpGetValueByClass(): not found attribute\n" ) ;
#endif	
		AnswerBuf[len++]	= 0x02 ;				
		lmpLSAP_DataReq( lsap, AnswerBuf, len ) ;
		return ;
	}
#if !defined(IASNOHIDDENCONTROL)
	if( IASAttr[attrid].hidden ) {
#if	defined(DMSG)
_DPUTS( DL_LMPINFO, " IAS::lmpGetValueByClass(): attribute is hidden\n" ) ;
#endif	
		AnswerBuf[len++]	= 0x02 ;				
		lmpLSAP_DataReq( lsap, AnswerBuf, len ) ;
		return ;
	}
#endif	


#if	defined(DMSG)
_DPUTS( DL_LMPINFO, " IAS::lmpGetValueByClass(): found!!\n" ) ;
#endif	
	AnswerBuf[len++]	= 0x00 ;					
	len	+= MiscSetBigEndian16( &AnswerBuf[len], 1 ) ;	


	len	+= MiscSetBigEndian16( &AnswerBuf[len], (word)classid ) ;	
													
	IrDA_memcpy( &AnswerBuf[len], IASAttr[attrid].Value, IASAttr[attrid].ValueSize ) ;
	len		+= IASAttr[attrid].ValueSize ;
	lmpLSAP_DataReq( lsap, AnswerBuf, len ) ;
	return ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint	lmpGetEncodedValueSize( const byte IrDAFAR *value )
{
	word	tmpw ;


#if defined(IrDABROKENCOMPILER01)
	int		cmd ;
	cmd		= ((int)value[0] ) & 0x00FF ;
	switch( cmd ) {
#else	
	switch( value[0] ) {
#endif	
	case	0 :										
		return( 1 ) ;
	case	1 :										
		return( 1+4 ) ;
	case	2 :										
		MiscGetBigEndian16((void*)&value[1], &tmpw ) ;
		return( 1 + 2 + tmpw ) ;
	case	3 :										
		return( 1 + 1 + 1 + value[2] ) ;
	}


	return( 1 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpFindClass( const byte IrDAFAR *classnm, uint classsz )
{
	int		i ;


#if	defined(DMSG)
{	int		j, c ;
	_DPUTS( DL_LMPINFO, " LMP::lmpFindClass():" ) ;
	_DPUTW( DL_LMPINFO, " classsz:", classsz ) ;
	_DPUTS( DL_LMPINFO, " class:" ) ;
	for( j=0 ; j<classsz ; j++ ) {
		c		= classnm[j] ;
		if( c < ' ' || 0x7F < c ) {
			_DPUTB( DL_LMPINFO, "[", c ) ;
			_DPUTC( DL_LMPINFO, ']' ) ;
		} else {
			_DPUTC( DL_LMPINFO, c ) ;
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	for( i=0 ; i<IASCLASSMAX ; i++ ) {
#if	defined(DMSG)
{	int		j, c ;
		if( IASClass[i].NameSize > 0 ) {
			_DPUTS( DL_LMPINFO, " LMP::lmpFindClass():" ) ;
			_DPUTW( DL_LMPINFO, " #", i ) ;
			_DPUTW( DL_LMPINFO, " size", IASClass[i].NameSize ) ;
			_DPUTS( DL_LMPINFO, " name:" ) ;
			for( j=0 ; j<IASClass[i].NameSize ; j++ ) {
				c		= IASClass[i].Name[j] ;
				if( c < ' ' || 0x7F < c ) {
					_DPUTB( DL_LMPINFO, "[", c ) ;
					_DPUTC( DL_LMPINFO, ']' ) ;
				} else {
					_DPUTC( DL_LMPINFO, c ) ;
				}
			}
			_DPUTS( DL_LMPINFO, "\n" ) ;
		}
}
#endif	
		if( IASClass[i].NameSize != classsz )
			continue ;
		if( !IrDA_memcmp( IASClass[i].Name, classnm, classsz ))
			return( i ) ;
	}


	return( IrDAR_INVALID ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpFindAttribute( int classid, const byte IrDAFAR *attrnm, uint attrsz )
{
	int		i ;


#if	defined(DMSG)
{	int		j, c ;
	_DPUTS( DL_LMPINFO, " LMP::lmpFindAttribute():" ) ;
	_DPUTW( DL_LMPINFO, " classid:", classid ) ;
	_DPUTW( DL_LMPINFO, " attrsz:", attrsz ) ;
	_DPUTS( DL_LMPINFO, " attr:" ) ;
	for( j=0 ; j<attrsz ; j++ ) {
		c		= attrnm[j] ;
		if( c < ' ' || 0x7F < c ) {
			_DPUTB( DL_LMPINFO, "[", c ) ;
			_DPUTC( DL_LMPINFO, ']' ) ;
		} else {
			_DPUTC( DL_LMPINFO, c ) ;
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	for( i=0 ; i<IASATTRMAX ; i++ ) {
#if	defined(DMSG)
{	int		j, c ;
		if( IASAttr[i].AttributeSize > 0 ) {
			_DPUTS( DL_LMPINFO, " LMP::lmpFindAttribute():" ) ;
			_DPUTW( DL_LMPINFO, " #", i ) ;
			_DPUTW( DL_LMPINFO, " size", IASAttr[i].AttributeSize ) ;
			_DPUTS( DL_LMPINFO, " name:" ) ;
			for( j=0 ; j<IASAttr[i].AttributeSize ; j++ ) {
				c		= IASAttr[i].Attribute[j] ;
				if( c < ' ' || 0x7F < c ) {
					_DPUTB( DL_LMPINFO, "[", c ) ;
					_DPUTC( DL_LMPINFO, ']' ) ;
				} else {
					_DPUTC( DL_LMPINFO, c ) ;
				}
			}
			_DPUTS( DL_LMPINFO, "\n" ) ;
		}
}
#endif	
		if( IASAttr[i].AttributeSize != attrsz || IASAttr[i].ClassId != classid )
			continue ;
		if( !IrDA_memcmp( IASAttr[i].Attribute, attrnm, attrsz ))
			return( i ) ;
	}


	return( IrDAR_INVALID ) ;
}
#endif	


#endif	
