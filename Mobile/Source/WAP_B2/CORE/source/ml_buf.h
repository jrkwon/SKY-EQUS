/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Headerfile : ML_BUF.H                                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Copyright by Telelogic AB 1993 - 1998                                       |
|  Copyright by S&P Media GmbH Germany 1993 - 1998                             |
|                                                                              |
|  This Program is owned by Telelogic and is protected by national             |
|  copyright laws and international copyright treaties. Telelogic              |
|  grants you the right to use this Program on one computer or in              |
|  one local computer network at any one time.                                 |
|  Under this License you may only modify the source code for the purpose      |
|  of adapting it to your environment. You must reproduce and include          |
|  any copyright and trademark notices on all copies of the source code.       |
|  You may not use, copy, merge, modify or transfer the Program except as      |
|  provided in this License.                                                   |
|  Telelogic does not warrant that the Program will meet your                  |
|  requirements or that the operation of the Program will be                   |
|  uninterrupted and error free. You are solely responsible that the           |
|  selection of the Program and the modification of the source code            |
|  will achieve your intended results and that the results are actually        |
|  obtained.                                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-MHDR*/




#ifndef __ML_BUF_H_
#define __ML_BUF_H_

extern void   xmk_RingInit  XPP((xmk_T_RING xmk_RAM_ptr, char xmk_RAM_ptr, int, int, unsigned char));
extern int    xmk_RingWrite XPP((xmk_T_RING xmk_RAM_ptr, char xmk_RAM_ptr, int));
extern int    xmk_RingRead  XPP((xmk_T_RING xmk_RAM_ptr, char xmk_RAM_ptr, int));
extern int    xmk_RingQuery XPP((xmk_T_RING xmk_RAM_ptr));

#endif /* ... __ML_BUF_H_ */
