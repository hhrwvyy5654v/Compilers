/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char *tokenString)
{
  switch (token)
  {
  case IF:
  case THEN:
  case ELSE:
  case END:
  case REPEAT:
  case UNTIL:
  case READ:
  case WRITE:
  case TRUE1:
  case FALSE1:
  case OR:
  case AND:
  case NOT:
  case INT:
  case BOOL1:
  case FLOAT:
  case STRING:
  case DOUBLE:
  case DO:
  case WHILE:
    fprintf(listing,
            "KEY: %s\n", tokenString);
    break;
  case ASSIGN:
    fprintf(listing, "SYM: :=\n");
    break;
  case LT:
    fprintf(listing, "SYM: <\n");
    break;
  case MT:
    fprintf(listing, "SYM: >\n");
    break;
  case LE:
    fprintf(listing, "SYM: <=\n");
    break;
  case ME:
    fprintf(listing, "SYM: >=\n");
    break;
  case EQ:
    fprintf(listing, "SYM: =\n");
    break;
  case COMMA:
    fprintf(listing, "SYM: ,\n");
    break;
  case UPDOX:
    fprintf(listing, "SYM: \'\n");
    break;
  case PERCENT:
    fprintf(listing, "SYM: %%\n");
    break;
  case LPAREN:
    fprintf(listing, "SYM: (\n");
    break;
  case RPAREN:
    fprintf(listing, "SYM: )\n");
    break;
  case SEMI:
    fprintf(listing, "SYM: ;\n");
    break;
  case PLUS:
    fprintf(listing, "SYM: +\n");
    break;
  case MINUS:
    fprintf(listing, "SYM: -\n");
    break;
  case TIMES:
    fprintf(listing, "SYM: *\n");
    break;
  case OVER:
    fprintf(listing, "SYM: /\n");
    break;
  case ENDFILE:
    fprintf(listing, "EOF\n");
    break;
  case NUM:
    fprintf(listing,
            "NUM, val= %s\n", tokenString);
    break;
  case ID:
    fprintf(listing,
            "ID, name= %s\n", tokenString);
    break;
  case STR:
    fprintf(listing,
            "STR, val= %s\n", tokenString);
    break;
  case ERROR:
    fprintf(listing,
            "ERROR: %s\n", tokenString);
    break;
  default: /* should never happen */
    fprintf(listing, "Unknown token: %d\n", token);
  }
}
