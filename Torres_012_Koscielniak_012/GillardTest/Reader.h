/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@                                                               @@@@
@@          /       ==============================        /          @@
@         ////           @@       @@ #@                  ////         @
@         ////*          @@ @%  @ @@    @@  @ #@#       /////         @
@     ((( ////))))   @@  @% @%  @ @@ #@ @@  @ #@@#   ///////( ///     @
@     ((((,/ )))))    @@@@   @@@  @@ ##  @@@   #@#   ///// ,/////     @
@     (((((/)))((    ------------------------------    ((((./////*    @
@    //*./ /  .///   ---  PROGRAMMING LANGUAGE  ---  ////   / ( (//   @
@    ///// / /////   ==============================  * ////* / ////   @
@     ///// ///// ((                               (( ///// /////     @
@    ((((   / , (((((                             (((((  //   /(((    @
@    (((((((/ . (((((                          (((((* / (((((((       @
@      (((((( //((((/((((                    *((((/((((/( (((((/      @
@       .//,   * /   (((((                   (((((  ///    .//.       @
@     ,////////./(  (((((* ////         (///(((((( ,/(,////////       @
@         //////// ,// ((( /////,     ////// ((( //  /////// *        @
@            (((((((((,// * /////     /////   (//(((((((((            @
@            ((((((((((((/////         //.///  (((((((((((.           @
@                   (///////// //(   (//.//////////                   @
@                  (/////////             //////////                  @
@                                                                     @
@@          A L G O N Q U I N   C O L L E G E  -  2 0 2 3 W          @@
@@@@                                                               @@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'F', /* Fixed mode (constant size) */
	MODE_ADDIT = 'A', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'M'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
/* TO_DO: BIT 7: FUL = Full */
#define READER_FULL_FLAG 0x7F /*0111.1111*/
/* TO_DO: BIT 6: EMP: Empty */
#define READER_EMPTY_FLAG 0x3F /*0011.1111*/
/* TO_DO: BIT 5: REL = Relocation */
#define READER_RELOCATION_FLAG 0x1F /*0001.1111*/
/* TO_DO: BIT 4: END = EndOfBuffer */
#define READER_END_FLAG 0x0F /*0000.1111*/

#define NCHAR				128			/* Chars from 0 to 127 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	gillard_intg mark;			/* the offset to the mark position (in chars) */
	gillard_intg read;			/* the offset to the get a char position (in chars) */
	gillard_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	gillard_char*	content;			/* pointer to the beginning of character array (character buffer) */
	gillard_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	gillard_intg		increment;			/* character array increment factor */
	gillard_intg		mode;				/* operational mode indicator */
	gillard_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	gillard_intg		histogram[NCHAR];	/* Statistics of chars */
	gillard_intg		numReaderErrors;	/* Number of errors from Reader */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate		(gillard_intg, gillard_intg, gillard_intg);
ReaderPointer	readerAddChar		(ReaderPointer const, gillard_char);
gillard_boln		readerClear		    (ReaderPointer const);
gillard_boln		readerFree		    (ReaderPointer const);
gillard_boln		readerIsFull		(ReaderPointer const);
gillard_boln		readerIsEmpty		(ReaderPointer const);
gillard_boln		readerSetMark		(ReaderPointer const, gillard_intg);
gillard_intg		readerPrint		    (ReaderPointer const);
gillard_intg		readerLoad			(ReaderPointer const, FILE* const);
gillard_boln		readerRecover		(ReaderPointer const);
gillard_boln		readerRetract		(ReaderPointer const);
gillard_boln		readerRestore		(ReaderPointer const);
/* Getters */
gillard_char		readerGetChar		(ReaderPointer const);
gillard_char*	readerGetContent	(ReaderPointer const, gillard_intg);
gillard_intg		readerGetPosRead	(ReaderPointer const);
gillard_intg		readerGetPosWrte	(ReaderPointer const);
gillard_intg		readerGetPosMark	(ReaderPointer const);
gillard_intg		readerGetSize		(ReaderPointer const);
gillard_intg		readerGetInc		(ReaderPointer const);
gillard_intg		readerGetMode		(ReaderPointer const);
gillard_byte		readerGetFlags		(ReaderPointer const);
gillard_intg		readerShowStat		(ReaderPointer const);
gillard_intg		readerNumErrors		(ReaderPointer const);

#endif
