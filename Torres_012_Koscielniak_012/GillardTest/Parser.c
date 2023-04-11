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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

gillard_void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
gillard_void matchToken(gillard_intg tokenCode, gillard_intg tokenAttribute) {
	gillard_intg matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
gillard_void syncErrorHandler(gillard_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
gillard_void printError() {
	Token t = lookahead;
	
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {MNID_T (main@)}.
 ***********************************************************
 */
gillard_void program() {
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
			matchToken(MNID_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			dataSession();
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (KW_data)}.
 ***********************************************************
 */
gillard_void dataSession() {
	matchToken(KW_T, KW_data); //data
	matchToken(LBR_T, NO_ATTR); //bracket
	optVarListDeclarations(); //Variables
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
gillard_void optVarListDeclarations() {
	switch (lookahead.code) {
	case KW_int:
		break;
	case KW_T:
		//printf("code type %d\n",lookahead.attribute.codeType);
		while (lookahead.code != RBR_T) {
			switch (lookahead.attribute.codeType)
			{
			case KW_int:
			case KW_string:
			case KW_real:
				//printf("Real Detected!!!\t%d\n", lookahead.attribute.codeType);
				while (lookahead.code != EOS_T) {
					lookahead = tokenizer();	
					//printf("%d\n", lookahead.code);
					if (lookahead.code == ERR_T) {
						printError();
						lookahead = tokenizer();
						syntaxErrorNumber++;
					}
				}
				//printf("End of line\n");
	
				break;

			default:
				printf("Keyword not for Variable!\n");
				break;
			}
		
			lookahead = tokenizer();
			//printf("%d\n", lookahead.code);
			if (lookahead.code == ERR_T) {
				printError();
				lookahead = tokenizer();
				syntaxErrorNumber++;
			}
			/*if (lookahead.code == RBR_T) {
				printf("End of code block\n");
				break;
			}*/
		}
		break;
	
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
gillard_void codeSession() {
	matchToken(KW_T, KW_code);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	// matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
gillard_void optionalStatements() {
	// printf("in opt statements\n");
	gillard_boln isValid = GILLARD_TRUE;

	while (lookahead.code != RBR_T) {
		//printf("in opt sstatements %d\n", lookahead.code);
		switch (lookahead.code) {
		case MNID_T:
			if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
				(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
				statements();
				break;
			}

		case VAR_T: //variable = <statements>;
			switch (lookahead.attribute.varType)
			{

			case VAR_FLT://float
				lookahead = tokenizer(); //looking for "="
				if (lookahead.attribute.relationalOperator != OP_EQ) {
					printError();
					lookahead = tokenizer();
					syntaxErrorNumber++;
					printf("ERROR!\n");
	;
				}
				else {
					lookahead = tokenizer();
					if (lookahead.attribute.varType == VAR_FLT ||
						lookahead.attribute.varType == VAR_INT ||
						lookahead.code == INT_T ||
						lookahead.code == FLT_T) {
						lookahead = tokenizer();
						if (lookahead.code == AOP_T) { // +/-/*/divide
							isValid = GILLARD_TRUE;
							lookahead = tokenizer();
						}
						else {
							printError();
							syntaxErrorNumber++;
							break;
						}
					}
					while (isValid && lookahead.code != EOS_T)
					{
						if (lookahead.attribute.varType == VAR_FLT ||
							lookahead.attribute.varType == VAR_INT ||
							lookahead.code == INT_T ||
							lookahead.code == FLT_T) {
							lookahead = tokenizer();
							if (lookahead.code == AOP_T) { // +/-/*/divide
								lookahead = tokenizer();
							}
							else if (lookahead.code == EOS_T) {
								break;
							}
							else {
								isValid = GILLARD_FALSE;

							}
						}
					}
				}
				break;
			case VAR_INT:
				lookahead = tokenizer(); //looking for "="
				if (lookahead.attribute.relationalOperator != OP_EQ) {
					printError();
					lookahead = tokenizer();
					syntaxErrorNumber++;
					printf("ERROR!\n");
					;
				}
				else {
					lookahead = tokenizer();
					if (lookahead.attribute.varType == VAR_INT || lookahead.code == INT_T) {
						lookahead = tokenizer();
						if (lookahead.code == AOP_T) { // +/-/*/divide
							isValid = GILLARD_TRUE;
							lookahead = tokenizer();
						}
						else {
							printError();
							syntaxErrorNumber++;
							break;
						}
					}
					while (isValid && lookahead.code != EOS_T)
					{
						if (lookahead.attribute.varType == VAR_INT || lookahead.code == INT_T) {
							lookahead = tokenizer();
							if (lookahead.code == AOP_T) { // +/-/*/divide
								lookahead = tokenizer();
							}
							else if (lookahead.code == EOS_T) {
								break;
							}
							else {
								isValid = GILLARD_FALSE;
							}
						}
					}
				}
				break;
			case VAR_STRING:
				lookahead = tokenizer(); //looking for "="
				if (lookahead.attribute.relationalOperator != OP_EQ) {
					printError();
					lookahead = tokenizer();
					syntaxErrorNumber++;
					printf("ERROR!\n");
				}
				else {
					lookahead = tokenizer();
					if (lookahead.attribute.varType == VAR_STRING || lookahead.code == STR_T) {
						lookahead = tokenizer();
						if (lookahead.code == AOP_T) { // +/-/*/divide
							isValid = GILLARD_TRUE;
							lookahead = tokenizer();
						}
						else {
							printError();
							syntaxErrorNumber++;
							break;
						}
					}
					while (isValid && lookahead.code != EOS_T)
					{
						if (lookahead.attribute.varType == VAR_STRING || lookahead.code == STR_T) {
							lookahead = tokenizer();
							if (lookahead.code == AOP_T) { // +/-/*/divide
								lookahead = tokenizer();
							}
							else if (lookahead.code == EOS_T) {
								break;
							}
							else {
								isValid = GILLARD_FALSE;
							}
						}
					}
				}
				break;
			default://Empty
				break;
			}
			if (!isValid) {
				printError();
				syntaxErrorNumber++;
			}

			break;
		case KW_T:
			// printf("Keyword detected\n");
			isValid = GILLARD_FALSE;;
			lookahead = tokenizer();
			if (lookahead.code != LPR_T) {
				lookahead = tokenizer();
			}else{
				if (lookahead.attribute.varType == VAR_STRING || lookahead.code == STR_T) {
					lookahead = tokenizer();
					if (lookahead.attribute.relationalOperator == OP_EQ || 
						lookahead.attribute.relationalOperator == OP_NE ||
						lookahead.attribute.relationalOperator == OP_GT ||
						lookahead.attribute.relationalOperator == OP_LT) {
						lookahead = tokenizer();
						if (lookahead.attribute.varType == VAR_STRING || lookahead.code == STR_T) {
							lookahead = tokenizer();
							while (lookahead.code != RBR_T)
							{
								lookahead = tokenizer();
							}
							isValid = GILLARD_TRUE;
						}
					}
				}else if (lookahead.attribute.varType == VAR_INT || lookahead.code == INT_T) {
					lookahead = tokenizer();
					if (lookahead.attribute.relationalOperator == OP_EQ ||
						lookahead.attribute.relationalOperator == OP_NE ||
						lookahead.attribute.relationalOperator == OP_GT ||
						lookahead.attribute.relationalOperator == OP_LT) {
						lookahead = tokenizer();
						if (lookahead.attribute.varType == VAR_INT || lookahead.code == INT_T) {
							lookahead = tokenizer();
							while (lookahead.code != RBR_T)
							{
								lookahead = tokenizer();
							}
							isValid = GILLARD_TRUE;
						}
					}
				}else if (lookahead.attribute.varType == VAR_FLT || lookahead.code == FLT_T) {
					lookahead = tokenizer();
					if (lookahead.attribute.relationalOperator == OP_EQ ||
						lookahead.attribute.relationalOperator == OP_NE ||
						lookahead.attribute.relationalOperator == OP_GT ||
						lookahead.attribute.relationalOperator == OP_LT) {
						lookahead = tokenizer();
						if (lookahead.attribute.varType == VAR_FLT || lookahead.code == FLT_T) {
							lookahead = tokenizer();
							while (lookahead.code != RBR_T)
							{
								lookahead = tokenizer();
							}
							isValid = GILLARD_TRUE;
						}
					}
				}
				
			}
			if (!isValid) {
				printError();
				syntaxErrorNumber++;
			}
			break;
		default:
			; // Empty
		}
		lookahead = tokenizer();
		//printf("%d\n", lookahead.code);
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
			printf("ERROR!\n");
			return;
		}
		// if (lookahead.code == RBR_T) {
		// 	printf("right braket detected!\n");
		// }
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
gillard_void statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
gillard_void statementsPrime() {
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			statements();
			break;
		}
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
gillard_void statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
gillard_void outputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
gillard_void outputVariableList() {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case FLT_T:
		matchToken(FLT_T, NO_ATTR);
		break;
	case INT_T:
		matchToken(INT_T, NO_ATTR);
		break;
	case VAR_T:
		matchToken(VAR_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}
