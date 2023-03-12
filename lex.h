#ifndef LEX_H
#define LEX_H

#define EOI 0       // end of input
#define SEMI 1      // ;
#define PLUS 2      // +
#define TIMES 3     // *
#define LP 4        // (
#define RP 5        // )
#define NUM_OR_ID 6 // decimal number or indentifier

extern char *yytext;
extern int yyleng;
extern yylineno;

#endif