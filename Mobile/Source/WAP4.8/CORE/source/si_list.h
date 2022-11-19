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
#ifndef _SI_LIST_H
#define _SI_LIST_H


#include "cmmnrsrc.h"

/******************************************/

typedef enum 
{   
	headkind,
  linkkind

} enumKind;


typedef struct element {

   enumKind kind;

   struct element *pre,
                  *suc;

	VOID *data;

}  t_element,
   *t_link,
   *t_head;

/*******************************************/

VOID f_lh_newhead (t_head *p_h);
VOID f_lh_newlink (t_link *p_l);
VOID f_lh_out (t_link *p_l);
VOID f_lh_precede (t_link *p_l, t_link *p_x);
VOID f_lh_follow (t_link *p_l, t_link *p_x);
VOID f_lh_into (t_link *p_l, t_head *p_h);
VOID f_lh_into_as_first (t_link *p_l, t_head *p_h);
t_link f_lh_lpred (t_link p_l);
t_link f_lh_lsuc (t_link p_l);
BOOL f_lh_empty (t_head p_h);
t_link f_lh_first (t_head p_h);
t_link f_lh_last (t_head p_h);
UINT16 f_lh_cardinal (t_head p_h);
VOID f_lh_disposelink (t_link *p_l);
VOID f_lh_clear (t_head *p_h);
VOID f_lh_disposehead (t_head *p_h);

/**********************************************************************

Funktioner för listhantering:
   f_lh_newhead
   Skapar en ny tom lista p_h

   f_lh_newlink
   skapar ett nytt element p_l

   f_lh_out
   Om elementet p_l tillhör en lista tas den ur listan.
   p_l ändras inte i övrigt

   f_lh_precede
   Sätter in p_l före p_x i listan där p_x finns

   f_lh_follow
   Sätter in p_l efter p_x i listan där p_x finns

   f_lh_into
   Sätter in p_l sist i listan p_h

   f_lh_into_as_first
   Sätter in p_l först i listan p_h

   f_lh_lpred
   Ger en pekare till det element som ligger före p_l i listan,
   om p_l är först returneras NULL

   f_lh_lsuc
   Ger en pekare till det element som ligger efter p_l i listan
   om p_l är sist returneras NULL

   f_lh_empty
   Returnerar 1 om listan är tom, annars 0

   f_lh_first
   Returnerar första elementet i listan, om tom : NULL

   f_lh_last
   Returnerar sista elementet i listan, om tom : NULL

   f_lh_cardinal
   Returnerar antalet element i listan

   f_lh_disposelink
   Raderar ut elementet p_l

   f_lh_clear
   Rensar ut listan utom huvudet p_h

   f_lh_disposehead
   Raderar ut hela listan inklusive p_h


*************************************************************************/


/* _SI_LIST_H */
#endif 
