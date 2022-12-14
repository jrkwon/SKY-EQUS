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

/* Program generated by SDT Cmicro, version 1.33 */

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif

#ifndef XX_Global_Declarations_H
#define XX_Global_Declarations_H

/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

/*****
* SYNONYM MAXTRANS
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(3),197(20,40),4)
******/
#define MAXTRANS SDL_INTEGER_LIT(5)

/*****
* SYNONYM MAXRESTRANS
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(3),197(20,40),5)
******/
#define z_Global_Declarations_5_MAXRESTRANS SDL_INTEGER_LIT(1)

/*****
* SYNONYM MAXSESSIONS
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(3),197(20,40),21)
******/
#define MAXSESSIONS SDL_INTEGER_LIT(15)
                /* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun,4,1) */

#include "wipprdct.h"

#ifndef MAXSESSIONS
  #error  The_SYNONYM_MAXSESSIONS_must_be_defined_at_this_point
#endif

#if  defined CONFIG_PUSH && defined CONFIG_WTA
#elif  defined CONFIG_PUSH || defined CONFIG_WTA
#endif


/*****
* NEWTYPE DeviceAddress  (TYPE section)
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),200(45,45),1)
******/
                /* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),200(45,45),10) */
#include "tapicmmn.h"

#define MAX_DEVICE_ADDR_LEN   16

typedef struct {
  UINT8 length;
  BYTE  data[MAX_DEVICE_ADDR_LEN];
} DeviceAddress;



/*****
* NEWTYPE AuxiliaryAddress  (TYPE section)
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),203(45,65),1)
******/
                /* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),203(45,65),10) */

#define MAX_AUX_ADDR_LEN       16

typedef struct {
  UINT8 length;
  BYTE  data[MAX_AUX_ADDR_LEN];
} AuxiliaryAddress;



/*****
* NEWTYPE AddressType  (TYPE section)
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),206(25,90),1)
******/
typedef struct AddressType_s {
    SDL_Octet  Bearer;
    SDL_Octet  SecondAddressFormat;
    SDL_Octet  ViewID;
    SDL_Octet  ChannelID;
    SDL_Integer  PortNumber;
    DeviceAddress  Address;
    AuxiliaryAddress  SecondAddress;
} AddressType;

/*****
* NEWTYPE DeviceAddress  (HEADING section)
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),200(45,45),1)
******/
                /* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),200(45,45),10) */
DeviceAddress
devaddr_null (void);

SDL_Integer
devaddr_getlen (DeviceAddress *paddr);

DeviceAddress
devaddr_fromOS (SDL_Octet_String os);

SDL_Octet_String
devaddr_toOS (DeviceAddress *paddr);

void
devaddr_store (DeviceAddress *paddr, BYTE *buf, UINT8 len);

void
devaddr_get (DeviceAddress *paddr, BYTE *buf, UINT8 *len);



#define yAssF_DeviceAddress(V,E,A)  V = E
extern void yDef_DeviceAddress XPP(( DeviceAddress * ));
extern SDL_Boolean yEq_DeviceAddress XPP(( DeviceAddress, DeviceAddress ));
#define yEqF_DeviceAddress(Expr1,Expr2)  yEq_DeviceAddress(Expr1,Expr2)
#define yNEqF_DeviceAddress(Expr1,Expr2)  ( ! yEq_DeviceAddress(Expr1,Expr2) )
#ifndef ANY_DeviceAddress
#define ANY_DeviceAddress
#endif
#define z_Global_Declarations_17_copy(P)  P

/*****
* NEWTYPE AuxiliaryAddress  (HEADING section)
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),203(45,65),1)
******/
                /* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),203(45,65),10) */
AuxiliaryAddress
auxaddr_null (void);

SDL_Integer
auxaddr_getlen (AuxiliaryAddress *paddr);

AuxiliaryAddress
auxaddr_fromOS (SDL_Octet_String os);

SDL_Octet_String
auxaddr_toOS (AuxiliaryAddress *paddr);

void
auxaddr_store (AuxiliaryAddress *paddr, BYTE *buf, UINT8 len);

void
auxaddr_get (AuxiliaryAddress *paddr, BYTE *buf, UINT8 *len);



#define yAssF_AuxiliaryAddress(V,E,A)  V = E
extern void yDef_AuxiliaryAddress XPP(( AuxiliaryAddress * ));
extern SDL_Boolean yEq_AuxiliaryAddress XPP(( AuxiliaryAddress,
  AuxiliaryAddress ));
#define yEqF_AuxiliaryAddress(Expr1,Expr2)  yEq_AuxiliaryAddress(Expr1,Expr2)
#define yNEqF_AuxiliaryAddress(Expr1,Expr2)  ( ! yEq_AuxiliaryAddress(Expr1,Expr2) )
#ifndef ANY_AuxiliaryAddress
#define ANY_AuxiliaryAddress
#endif
#define z_Global_Declarations_27_copy(P)  P

/*****
* NEWTYPE AddressType  (HEADING section)
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(2),206(25,90),1)
******/

#define yAssF_AddressType(V,E,A)  V = E
extern void yDef_AddressType XPP(( AddressType * ));
extern SDL_Boolean yEq_AddressType XPP(( AddressType, AddressType ));
#define yEqF_AddressType(Expr1,Expr2)  yEq_AddressType(Expr1,Expr2)
#define yNEqF_AddressType(Expr1,Expr2)  ( ! yEq_AddressType(Expr1,Expr2) )
extern AddressType yMake_AddressType XPP(( SDL_Octet, SDL_Octet, SDL_Octet,
  SDL_Octet, SDL_Integer, DeviceAddress, AuxiliaryAddress ));
#ifndef ANY_AddressType
#define ANY_AddressType  yMake_AddressType(ANY_SDL_Octet, ANY_SDL_Octet, \
  ANY_SDL_Octet, ANY_SDL_Octet, ANY_SDL_Integer, ANY_DeviceAddress, \
  ANY_AuxiliaryAddress)
#endif
#define z_Global_Declarations_33_copy(P)  P

/*****
* SIGNAL ClosePort
* <<PACKAGE Global_Declarations>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\global_declarations.sun(3),209(20,195),2)
******/
#define z_Global_Declarations_0_ClosePort 13
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
    SDL_Natural  Param2;
} yPDef_z_Global_Declarations_0_ClosePort;

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif
