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
#include "si_list.h"

/**********************************************************************

Funktioner f�r listhantering:
   f_lh_newhead
   Skapar en ny tom lista p_h

   f_lh_newlink
   skapar ett nytt element p_l

   f_lh_out
   Om elementet p_l tillh�r en lista tas den ur listan.
   p_l �ndras inte i �vrigt

   f_lh_precede
   S�tter in p_l f�re p_x i listan d�r p_x finns

   f_lh_follow
   S�tter in p_l efter p_x i listan d�r p_x finns

   f_lh_into
   S�tter in p_l sist i listan p_h

   f_lh_into_as_first
   S�tter in p_l f�rst i listan p_h

   f_lh_lpred
   Ger en pekare till det element som ligger f�re p_l i listan,
   om p_l �r f�rst returneras NULL

   f_lh_lsuc
   Ger en pekare till det element som ligger efter p_l i listan
   om p_l �r sist returneras NULL

   f_lh_empty
   Returnerar 1 om listan �r tom, annars 0

   f_lh_first
   Returnerar f�rsta elementet i listan, om tom : NULL

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




/************************************************************************
VOID f_lh_newhead (t_head *p_h) {};

Purpose:
Skapar en ny tom lista p_h

Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_newhead (t_head *p_h) {

		*p_h = NULL;
		*p_h = (t_head) OSConnectorAlloc( sizeof(t_element) );

	 if (*p_h != NULL) {
		 (*p_h)->kind = headkind;
		 (*p_h)->pre = *p_h;
		 (*p_h)->suc = *p_h;
		 (*p_h)->data = NULL;
	 }
}



/************************************************************************
VOID f_lh_newlink (t_link *p_l)

Purpose:
Skapar ett nytt element p_l

Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_newlink (t_link *p_l) {

	*p_l = NULL;
	*p_l = (t_link) OSConnectorAlloc( sizeof(t_element) );

	if (*p_l != NULL) {
   (*p_l)->kind = linkkind;
   (*p_l)->pre = NULL;
   (*p_l)->suc = NULL;
	 (*p_l)->data = NULL;
	}
}


/************************************************************************
VOID f_lh_out (t_link *p_l)

Purpose:
Om elementet p_l tillh�r en lista tas den ur listan.
p_l �ndras inte i �vrigt

Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_out (t_link *p_l) {

   if ( (*p_l)->pre != NULL) {
   /* D� �r �ven (*p_l).suc != NULL */

      (*p_l)->pre->suc = (*p_l)->suc;
      (*p_l)->suc->pre = (*p_l)->pre;
      (*p_l)->pre = NULL;
      (*p_l)->suc = NULL;

   }
}


/************************************************************************
VOID f_lh_precede (t_link *p_l, t_link *p_x)

Purpose:
S�tter in p_l f�re p_x i listan d�r p_x finns


Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_precede (t_link *p_l, t_link *p_x) {

   f_lh_out( p_l );
   /* tar ut p_l s� att den inte finns i n�gon annan lista ocks� */
   (*p_x)->pre->suc = *p_l;
   (*p_l)->pre = (*p_x)->pre;
   (*p_l)->suc = *p_x;
   (*p_x)->pre = *p_l;
}


/************************************************************************
VOID f_lh_follow (t_link *p_l, t_link *p_x)

Purpose:
S�tter in p_l efter p_x i listan d�r p_x finns

Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_follow (t_link *p_l, t_link *p_x) {

   f_lh_out( p_l );
   /* tar ut p_l s� att den inte finns i n�gon annan lista ocks� */
   (*p_x)->suc->pre = *p_l;
   (*p_l)->pre = *p_x;
   (*p_l)->suc = (*p_x)->suc;
   (*p_x)->suc = *p_l;
}


/************************************************************************
VOID f_lh_into (t_link *p_l, t_head *p_h)

Purpose:
S�tter in p_l sist i listan p_h


Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_into (t_link *p_l, t_head *p_h) {

   f_lh_out( p_l );
   /* tar ut p_l s� att den inte finns i n�gon annan lista ocks� */
   (*p_h)->pre->suc = *p_l;
   (*p_l)->pre = (*p_h)->pre;
   (*p_l)->suc = *p_h;
   (*p_h)->pre = *p_l;
}


/************************************************************************
VOID f_lh_into_as_first (t_link *p_l, t_head *p_h)

Purpose:
S�tter in p_l f�rst i listan p_h


Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_into_as_first (t_link *p_l, t_head *p_h) {

   f_lh_out( p_l );
   /* tar ut p_l s� att den inte finns i n�gon annan lista ocks� */
   (*p_h)->suc->pre = *p_l;
   (*p_l)->pre = *p_h;
   (*p_l)->suc = (*p_h)->suc;
   (*p_h)->suc = *p_l;
}


/************************************************************************
t_link f_lh_lpred (t_link p_l)

Purpose:
Ger en pekare till det element som ligger f�re p_l i listan,
om p_l �r f�rst returneras NULL

Limitations:
-

Return:
pekare till det element som ligger f�re p_l i listan
NULL om elementet �r f�rst i listan

************************************************************************/
t_link f_lh_lpred (t_link p_l) {

   if ( (*p_l).pre->kind == linkkind ) {
      return (*p_l).pre;
   }
   else {
      return NULL;
   }
}


/************************************************************************
t_link f_lh_lsuc (t_link p_l)

Purpose:
Ger en pekare till det element som ligger efter p_l i listan
om p_l �r sist returneras NULL

Limitations:
-

Return:
pekare till elementet efter p_l
NULL om sist i listan

************************************************************************/
t_link f_lh_lsuc (t_link p_l) {

   if ( (*p_l).suc->kind == linkkind ) {
      return (*p_l).suc;
   }
   else {
      return NULL;
   }
}


/************************************************************************
BOOL f_lh_empty (t_head p_h)

Purpose:
Returnerar TRUE om listan �r tom, annars FALSE

Limitations:
-

Return:
TRUE om listan �r tom
NULL om listan inte �r tom

************************************************************************/
BOOL f_lh_empty (t_head p_h) {

   if ( (*p_h).suc->kind == headkind ) {
      return TRUE;
   }
   else {
      return FALSE;
   }
}


/************************************************************************
t_link f_lh_first (t_head p_h)

Purpose:
Returnerar f�rsta elementet i listan, om tom : NULL

Limitations:
-

Return:
pekare till f�rsta elementet i listan
NULL om listan �r tom

************************************************************************/
t_link f_lh_first (t_head p_h) {

   if ( ! f_lh_empty(p_h) ) {
      return (*p_h).suc;
   }
   else {
      return NULL;
   }
}


/************************************************************************
t_link f_lh_last (t_head p_h)

Purpose:
Returnerar sista elementet i listan, om tom : NULL

Limitations:
-

Return:
pekare till det sista elementet i listan
NULL om listan �r tom

************************************************************************/
t_link f_lh_last (t_head p_h) {

   if ( ! f_lh_empty(p_h) ) {
      return (*p_h).pre;
   }
   else {
      return NULL;
   }
}


/************************************************************************
UINT16 f_lh_cardinal (t_head p_h)

Purpose:
Returnerar antalet element i listan

Limitations:
-

Return:
antalet element i listan

************************************************************************/
UINT16 f_lh_cardinal (t_head p_h) {

   UINT16		v_k;
   t_link   v_l;


   v_k = 0;
   v_l = f_lh_first(p_h);

   while ( v_l != NULL ) {
      v_k++;
      v_l = f_lh_lsuc( v_l );
   }

   return v_k;
}


/************************************************************************
VOID f_lh_disposelink (t_link *p_l)

Purpose:
Raderar ut elementet p_l


Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_disposelink (t_link *p_l) {

   f_lh_out(p_l);
   OSConnectorFree(*p_l);
   *p_l = NULL;
}


/************************************************************************
VOID f_lh_clear (t_head *p_h)

Purpose:
Rensar ut listan utom huvudet p_h

Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_clear (t_head *p_h) {

   t_link   v_l;


   while ( ! f_lh_empty(*p_h) ) {
      v_l = f_lh_first(*p_h);
      f_lh_disposelink(&v_l);
   }
}


/************************************************************************
VOID f_lh_disposehead (t_head *p_h)

Purpose:
Raderar ut hela listan inklusive p_h

Limitations:
-

Return:
-

************************************************************************/
VOID f_lh_disposehead (t_head *p_h) {

   f_lh_clear(p_h);
   OSConnectorFree(*p_h);
   *p_h = NULL;
}

