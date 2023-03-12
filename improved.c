#include "lex.h"
#include <stdarg.h>
#include <stdio.h>

#define MAXFIRST 16
#define SYNCH SEMI

void factor(void);
void term(void);
void expression(void);
int match(int token);
int lex(void);
void advance(void);
void statements(void);
int legal_lookhead(int first_arg, ...);

char *yytext = "";		   // Lexeme not '\0'
int yyleng = 0;			   // Lexeme length
int yylineno = 0;		   // input line number
static int Lookahead = -1; // Lookhead token

int main(void)
{
}

int legal_lookhead(int first_arg, ...) // check elipse after
{
	va_list args;
	int tok;
	int error_printed;
	int rval;

	/*
	Simple error detection and recovery. Aguments are a 0 terminated list of those tokens that can
	legitimately come text in the input. If the list is empty,
	the end of file must come next. Print error
	message if necessary. Error recovery is performed by discarding all input symbols until one that's in
	the input list is found
	Return:
		true: if there is no error or if we recovered from the error,
		false: if we can't recover.
	*/
	int lookaheads[MAXFIRST], *p = lookaheads, *current;
	int error_printed = 0;
	int rval = 0;

	va_start(args, first_arg);
	if (!first_arg)
	{
		if (match(EOF))
			rval = 1;
	}
	else
	{
		*p++;
		while ((tok = va_arg(args, int)) && p < &lookaheads[MAXFIRST])
			*(++p) = tok;
		while (!match(SYNCH))
		{
			for (current = lookaheads; current < p; ++current)
			{
				if (match(*current))
				{
					rval = 1;
					goto exit;
				}
			}
			if (!error_printed)
			{
				fprintf(stderr, "Line %d: Syntax error\n", yylineno);
				error_printed = 1;
			}
			advance();
		}
	}
exit:
	va_end(args);
	return rval;
}

void statements(void)
{
	// stataments -> expression SEMI | expression SEMI statements
	while (!match(EOI))
	{
		expression();
		if (match(SEMI))
			advance();
		else
			fprintf(stderr, "%d: Inserting missing semicolon\n", yylineno);
	}
}

void expression(void)
{
	// expression -> term expression'
	// expression' -> PLUS term expression' | epsilon
	if (!legal_lookahead(NUM_OR_ID, LP, 0)) // expressions all have to start with either a NUM_OR_ID  or LP
		return;								// otherwise something wrong with the expression
	term();
	while (match(PLUS))
	{
		advance();
		term();
	}
}

void term(void)
{
	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;
	factor();
	while (match(TIMES))
	{
		advance();
		factor();
	}
}

void factor(void)
{
	if (!legal_lookahead(NUM_OR_ID, LP, 0))
		return;
	if (match(NUM_OR_ID))
		advance();
	else if (match(LP))
	{
		advance();
		expression();
		if (match(RP))
			advance();
		else
			fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno);
	}
	else
		fprintf(stderr, "%d: Number or identifier expected\n", yylineno);
}

int match(int token)
{
	// Return true if "token matches the current lookahead symbol."
	if (Lookahead == -1)
		Lookahead = lex();
	return (token == Lookahead);
}

int lex(void)
{
	static char input_buffer[128];
	char *current;

	current = yytext + yyleng;
	while (1)
	{
		while (!*current)
		{
			// Get new lines, skiping any white spaces on the line
			// until a nonblank line is found
			current = input_buffer;
			if (!gets(input_buffer))
			{
				*current = '\0';
				return (EOI);
			}
			++yylineno;
			while (isspace(*current))
				++current;
		}
		for (; *current; ++current)
		{
			// get the next token
			yytext = current;
			yyleng = 1;
			switch (*current)
			{
			case EOF:
				return (EOI);
			case ';':
				return (SEMI);
			case '*':
				return (TIMES);
			case '+':
				return (PLUS);
			case '(':
				return (LP);
			case ')':
				return (RP);
			case '\n':
			case '\t':
			case ' ':
				break;
			default:
				if (!isalnum(*current))
					fprintf(stderr, "Ignoring illegal input <%c>\n", *current);
				else
				{
					while (isalnum(*current))
						++current;
					yyleng = current - yytext;
					return (NUM_OR_ID);
				}
				break;
			}
		}
	}
}
void advance(void)
{
	// Advance the lookahead to the next input symbol
	Lookahead = lex();
}