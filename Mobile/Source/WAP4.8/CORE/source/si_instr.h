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
#ifndef _SI_INSTR_H
#define _SI_INSTR_H
/***********************
File: si_instr.h + c
Author: Klas Hermodsson (KHN)

Description:
This module contains handling for all the WMLScript
bytecode operations. The ExecBytecodeInstr function
below is the main function. 

Design explanation:
The bytecode that this interpreter must execute
is built up by a defined set of bytecode operations.
Each operation has a bit pattern that uniquely identifies
it.

To quickly go from the bit-pattern in the BP (compiled wmlscript) to
the correct instruction there are three arrays. By performing some
easy bitoperations the instruction bit pattern can be used as an index
into one of the arrays and voila: the array entry holds a pointer to 
the function that carries out the corresponding operation.
There is also a timecost parameter in each array entry. This is the cost
in time units of running the operation. This timecost is used by
the SI (see si_si) to ensure that only a certain number of operations
are carried out before returning control. Currently the time cost is 
the same for all functions but it is possible to change this in the
future and have a better estimate of how much an operation costs in
time consumption in relation to other operations.

The ExecBytecodeInstr function declared below is the main function that 
is called. It does the bitpattern to operation transfer and calls the 
appropriate function to execute the bytecode operation.

************************/


#include "cmmnrsrc.h"
#include "si_si.h"


typedef enum { JUMP_FW_S, JUMP_BW_S, TJUMP_FW_S, LOAD_VAR_S } 
type1XXPPPPP;

typedef enum { STORE_VAR_S, LOAD_CONST_S }
type010XPPPP;

typedef enum { CALL_S, CALL_LIB_S, INCR_VAR_S }
type011XXPPP;

/* this construction is neccessary in order to
being able to compile in the WinCE environment (ONJ) */
#ifdef DEBUG
#define DEFINE_DEBUG
#undef DEBUG
#endif

typedef enum {	JUMP_FW = 0x01,	JUMP_FW_W,			JUMP_BW,				
								JUMP_BW_W,			TJUMP_FW,				TJUMP_FW_W,				
								TJUMP_BW, 			TJUMP_BW_W,			CALL, 					
								CALL_LIB, 			CALL_LIB_W, 		CALL_URL, 			
								CALL_URL_W, 		LOAD_VAR,				STORE_VAR,			
								INCR_VAR, 			DECR_VAR,				LOAD_CONST,			
								LOAD_CONST_W, 	CONST_0,				CONST_1,				
								CONST_M1, 			CONST_ES,				CONST_INVALID,	
								CONST_TRUE, 		CONST_FALSE,		INCR,						
								DECR, 					ADD_ASG,				SUB_ASG,				
								UMINUS, 				ADD,						SUB,						
								MUL, 						DIV,						IDIV,						
								REM,						B_AND,					B_OR,						
								B_XOR, 					B_NOT,					B_LSHIFT,				
								B_RSSHIFT, 			B_RSZSHIFT,			EQ,							
								LE, 						LT,							GE,							
								GT, 						NE,							NOT,						
								SCAND, 					SCOR,						TOBOOL,					
								POP, 						TYPEOF,					ISVALID,				
								RETURN,					RETURN_ES,			DEBUG }
type00XXXXXX;

/* this construction is neccessary in order to
being able to compile in the WinCE environment (ONJ) */
#ifdef DEFINE_DEBUG
#undef DEFINE_DEBUG
#define DEBUG
#endif


enumErrorCode ExecBytecodeInstr( BYTE instr, pstructSI si );


/* Control flow instructions */
enumErrorCode Exec_JUMP_FW_S( BYTE instr, pstructSI si );
enumErrorCode Exec_JUMP_FW( pstructSI si );
enumErrorCode Exec_JUMP_FW_W( pstructSI si );
enumErrorCode Exec_JUMP_BW_S( BYTE instr, pstructSI si );
enumErrorCode Exec_JUMP_BW( pstructSI si );
enumErrorCode Exec_JUMP_BW_W( pstructSI si );
enumErrorCode Exec_TJUMP_FW_S( BYTE instr, pstructSI si );
enumErrorCode Exec_TJUMP_FW( pstructSI si );
enumErrorCode Exec_TJUMP_FW_W( pstructSI si );
enumErrorCode Exec_TJUMP_BW( pstructSI si );
enumErrorCode Exec_TJUMP_BW_W( pstructSI si );

/* Function call instructions */
enumErrorCode Exec_CALL_S( BYTE instr, pstructSI si );
enumErrorCode Exec_CALL( pstructSI si );
enumErrorCode Exec_CALL_LIB_S( BYTE instr, pstructSI si );
enumErrorCode Exec_CALL_LIB( pstructSI si );
enumErrorCode Exec_CALL_LIB_W( pstructSI si );
enumErrorCode Exec_CALL_URL( pstructSI si );
enumErrorCode Exec_CALL_URL_W( pstructSI si );

/* Variable access and manipulation */
enumErrorCode Exec_LOAD_VAR_S( BYTE instr, pstructSI si );
enumErrorCode Exec_LOAD_VAR( pstructSI si );
enumErrorCode Exec_STORE_VAR_S( BYTE instr, pstructSI si );
enumErrorCode Exec_STORE_VAR( pstructSI si );
enumErrorCode Exec_INCR_VAR_S( BYTE instr, pstructSI si );
enumErrorCode Exec_INCR_VAR( pstructSI si );
enumErrorCode Exec_DECR_VAR( pstructSI si );

/* Access to constants */
enumErrorCode Exec_LOAD_CONST_S( BYTE instr, pstructSI si );
enumErrorCode Exec_LOAD_CONST( pstructSI si );
enumErrorCode Exec_LOAD_CONST_W( pstructSI si );
enumErrorCode Exec_CONST_0( pstructSI si );
enumErrorCode Exec_CONST_1( pstructSI si );
enumErrorCode Exec_CONST_M1( pstructSI si );
enumErrorCode Exec_CONST_ES( pstructSI si );
enumErrorCode Exec_CONST_INVALID( pstructSI si );
enumErrorCode Exec_CONST_TRUE( pstructSI si );
enumErrorCode Exec_CONST_FALSE( pstructSI si );

/* Arithmetic instructions */
enumErrorCode Exec_INCR( pstructSI si );
enumErrorCode Exec_DECR( pstructSI si );
enumErrorCode Exec_ADD_ASG( pstructSI si );
enumErrorCode Exec_SUB_ASG( pstructSI si );
enumErrorCode Exec_UMINUS( pstructSI si );
enumErrorCode Exec_ADD( pstructSI si );
enumErrorCode Exec_SUB( pstructSI si );
enumErrorCode Exec_MUL( pstructSI si );
enumErrorCode Exec_DIV( pstructSI si );
enumErrorCode Exec_IDIV( pstructSI si );
enumErrorCode Exec_REM( pstructSI si );

/* Bitwise instructions */
enumErrorCode Exec_B_AND( pstructSI si );
enumErrorCode Exec_B_OR( pstructSI si );
enumErrorCode Exec_B_XOR( pstructSI si );
enumErrorCode Exec_B_NOT( pstructSI si );
enumErrorCode Exec_B_LSHIFT( pstructSI si );
enumErrorCode Exec_B_RSSHIFT( pstructSI si );
enumErrorCode Exec_B_RSZSHIFT( pstructSI si );

/* Comparison instructions */
enumErrorCode Exec_EQ( pstructSI si );
enumErrorCode Exec_LE( pstructSI si );
enumErrorCode Exec_LT( pstructSI si );
enumErrorCode Exec_GE( pstructSI si );
enumErrorCode Exec_GT( pstructSI si );
enumErrorCode Exec_NE( pstructSI si );

/* Logical instructions */
enumErrorCode Exec_NOT( pstructSI si );
enumErrorCode Exec_SCAND( pstructSI si );
enumErrorCode Exec_SCOR( pstructSI si );
enumErrorCode Exec_TOBOOL( pstructSI si );

/* Stack instructions */
enumErrorCode Exec_POP( pstructSI si );

/* Access to operand type */
enumErrorCode Exec_TYPEOF( pstructSI si );
enumErrorCode Exec_ISVALID( pstructSI si );

/* Function return instructions */
enumErrorCode Exec_RETURN( pstructSI si );
enumErrorCode Exec_RETURN_ES( pstructSI si );

/* Miscellaneous instructions */
enumErrorCode Exec_DEBUG( pstructSI si );




#endif
