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
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Sep 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: @) */
	STR_T,		/*  2: String literal token */
	INT_T,		/*  3: Intger literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (semicolon) */
	VAR_T,		/* 10: Variable Identifier */
	CMA_T,		/* 11: Comma token */
	AOP_T,		/* 12: Arithmetic tokens */
	ROP_T,		/* 12: Relational OP tokens */
	FLT_T,		/* 13: Float literal token */
	RTE_T,		/* 14: Run-time error token */
	INL_T,		/* 15: Run-time error token */
	SEOF_T		/* 16: Source end-of-file token */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

typedef enum VariableType { VAR_INT, VAR_FLT, VAR_STRING } VarType;


/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	gillard_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	VarType varType;					/* Variable type */
	gillard_intg intValue;						/* integer literal attribute (value) */
	gillard_intg keywordIndex;					/* keyword index in the keyword table */
	gillard_intg contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	gillard_real floatValue;					/* floating-point literal attribute (value) */
	gillard_char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	gillard_char errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	gillard_byte flags;			/* Flags information */
	union {
		gillard_intg intValue;				/* Integer value */
		gillard_real floatValue;			/* Float value */
		gillard_char* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	gillard_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # , 
 *  .@., .|. , .!. , SEOF.
 */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '@'
#define CHRCOL4 '\"'
#define CHRCOL5 '-'
#define CHRCOL6 '.'
#define CHRCOL7 '%'
#define CHRCOL8 '$'

/* These constants will be used on VID / MID function */
#define MNIDPREFIX '@'
//#define VARSUFFIX '%'

/* TO_DO: Error states and illegal state */
#define FS		100		/* Illegal state */
#define ESNR	11		/* Error state with no retract */
#define ESWR	12		/* Error state with retract */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 11

/* TO_DO: Transition table - type of states defined in separate table */
static gillard_intg transitionTable[][TABLE_COLUMNS] = {
	/*[A-z], [0-9],    _,    @,    ",    -,    .,     %,     $,  SEOF, other
	   L(0),  D(1), S(2), M(3), Q(4), N(5), P(6),  F(7),  I(8),  E(9),  O(10) */
	{     1,     6, ESNR, ESNR,    4,    6, ESNR,  ESNR,  ESNR,  ESWR, ESNR}, // S0: NOAS
	{     1,     1,    8,    3, ESWR, ESWR, ESWR,	  8,     8,  ESWR,    2}, // S1: NOAS
	{    FS,    FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, // S2: ASWR (KEY)
	{    FS,    FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, // S3: ASNR (MVID)
	{     4,     4, ESWR,    4,    5,    4,    4,  ESWR,  ESWR,  ESWR,    4}, // S4: NOAS
	{    FS,    FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, // S5: ASNR (SL)
	{  ESWR,     6, ESWR, ESWR, ESWR, ESWR,    9,  ESWR,  ESWR,  ESWR,	   7}, // S6: NOAS
	{    FS,     7,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, // S7: ASWR (IL)
	{    FS,    FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, // S8: ASNR (VARID)
	{  ESWR,     9, ESWR, ESWR, ESWR, ESWR, ESWR,  ESWR,  ESWR,  ESWR,   10}, // S9: NOAS
	{    FS,    FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, // S9: ASWR (FL)
	{    FS,    FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, //S10: ASNR (ES)
	{    FS,    FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS,   FS}, //S11: ASWR (ER)

};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static gillard_intg stateType[] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSWR, /* 02 (KEY) */
	FSNR, /* 03 (MID) - Methods */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSNR, /* 07 (IL) */
	FSNR, /* 08 (VAR) - Variables */
	NOFS, /* 09 */
	FSWR, /* 10 (FL) */
	FSNR, /* 11 (Err1 - no retract) */
	FSWR  /* 12 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
gillard_intg startScanner(ReaderPointer psc_buf);
Token tokenizer(gillard_void);
static gillard_intg nextClass(gillard_char c);				/* character class function */
static gillard_intg nextState(gillard_intg, gillard_char);		/* state machine function */
gillard_void printToken(Token t);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(gillard_char* lexeme);

/* Declare accepting states functions */
Token funcSL	(gillard_char lexeme[]);
Token funcID	(gillard_char lexeme[]);
Token funcVar	(gillard_char lexeme[]);
Token funcKEY	(gillard_char lexeme[]);
Token funcErr	(gillard_char lexeme[]);
Token funcIL	(gillard_char lexeme[]);
Token funcFL	(gillard_char lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcKEY,	/* KEY  [03] - Keywords */
	funcID,		/* MNID	[02] - Methods */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] - String Literal */
	NULL,		/* -    [06] */
	funcIL,		/* IL   [07] - Integer Literal */
	funcVar,	/* VARID[08] - Variables */
	NULL,		/* -    [09] */
	funcFL,		/* FL	[10] - Float Literal */
	funcErr,	/* ERR1 [11] - No retract */
	funcErr		/* ERR2 [12] - Retract */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 12

/* TO_DO: Define the list of keywords */
static gillard_char* keywordTable[KWT_SIZE] = {
	"data",
	"code",
	"int",
	"real",
	"string",
	"if",
	"then",
	"else",
	"while",
	"do",
	"or",
	"and"
};

/* About indentation (useful for positional languages (ex: Python, Cobol) */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	gillard_char indentationCharType;
	gillard_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
gillard_intg numScannerErrors;

#endif
