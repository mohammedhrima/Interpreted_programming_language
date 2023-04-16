#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// tokens
typedef enum
{
	none_tk,
	eof_tk,
	identifier_tk,
	character_tk,
	number_tk,
	operation_tk
} Token_type;

typedef struct Token Token;
struct Token
{
	Token_type type;
	union
	{
		char *char_value;
		int int_value;
	} value;
};

char *text;
int txt_pos;

Token tokens[100];
int tk_pos;
// int pos;

void skip_space()
{
	while (text && isspace(text[txt_pos]))
		txt_pos++;
}

void tokenize(void)
{
	int start = 0;
	int end = 0;
	while (text[txt_pos])
	{
		while (isspace(text[txt_pos]))
			txt_pos++;
		Token *token = &tokens[tk_pos];
		if (isdigit(text[txt_pos]))
		{
			token->type = number_tk;
			while (isdigit(text[txt_pos]))
			{
				token->value.int_value = token->value.int_value * 10 + (text[txt_pos] - '0');
				txt_pos++;
			}
		}
		else if (isalpha(text[txt_pos]))
		{
			token->type = identifier_tk;
			start = txt_pos;
			while (isalnum(text[txt_pos]))
				txt_pos++;
			token->value.char_value = calloc(txt_pos - start + 1, sizeof(char));
			memcpy(token->value.char_value, text + start, txt_pos - start);
		}
		else if (text[txt_pos] == '"')
		{
			token->type = character_tk;
			txt_pos++;
			start = txt_pos;
			while (text[txt_pos] && text[txt_pos] != '"')
				txt_pos++;
			token->value.char_value = calloc(txt_pos - start + 1, sizeof(char));
			memcpy(token->value.char_value, text + start, txt_pos - start);
			// expect "
			txt_pos++;
		}
		else if (strchr("=+-", text[txt_pos]))
		{
			token->type = operation_tk;
			token->value.int_value = text[txt_pos];
			txt_pos++;
		}
		else
		{
			printf("unknown value\n");
			exit(0);
		}
		// increment tk_pos
		tk_pos++;
	}
	tokens[tk_pos].type = eof_tk;
	// return tokens;
}
// variables
typedef struct
{
	// Token token;
	char *name;
	int int_val;
} Var;

Var variables[100];
int var_pos = 0;

Var *new_var(char *name)
{
	Var *new = &variables[var_pos++];
	new->name = name;
	new->int_val = 0;
	return (new);
}

Var *find_var(char *name)
{
	int i = 0;
	while (i < var_pos)
	{
		if (strcmp(name, variables[i].name) == 0)
			return (&variables[i]);
		i++;
	}
	return NULL;
}

// parse
typedef enum
{
	binary_nd,
	number_nd,
	identifier_nd
} Node_type;

typedef struct Node Node;
struct Node
{
	Node *left;
	Node *right;
	Node_type type;
	/*	
	if type number_nd
		token->int_val : will hold the number
	if type identifier_nd
		
	*/
	Token *token;
	// Var *value; // variable to hold
};

// Node *new_node(Node_type type)
// {
// 	Node *new = calloc(11, sizeof(Node));
// 	new->left = NULL;
// 	new->right = NULL;
// 	new->type = type;
// 	new->value = calloc(1, sizeof(Var));
// 	// new->token = token;
// 	return (new);
// }

// void skip_token(Token_type type)
// {
// 	if (tokens[tk_pos].type != type)
// 	{
// 		printf("error: expected token '%d', but got '%d' \n", type, tokens[tk_pos].type);
// 		exit(1);
// 	}
// 	tk_pos++;
// }

// Node *expr();
// Node *assign();
// Node *add();
// Node *mul();
// Node *primary();

// Node *expr()
// {
// 	return assign();
// }

// // for =
// Node *assign()
// {
// 	Node *left = add();
// }

// Node *add()
// {
// 	Node *left = mul();
// }

// Node *mul()
// {
// 	Node *left = primary();
// }

// Node *primary()
// {
// 	Node *node = NULL;
// 	if (tokens[tk_pos].type == number_tk)
// 	{
// 		node = new_node(number_nd);

// 		tk_pos++;
// 		return node;
// 	}
// 	else if (tokens[tk_pos].type == identifier_tk)
// 	{
// 		node = new_node(identifer_nd);
// 		node->value = find_var(node);
// 		if (node->value == NULL)
// 			node->value = new_var(node);
// 		tk_pos++;
// 		return node;
// 	}
// }

// int eval()
// {
// 	int i = 0;
// 	while (i < tk_pos)
// 	{
// 		Node *curr = expr();
// 		// if(curr->type = )
// 	}
// }

int main(void)
{
	FILE *fp = NULL;
	long file_size = 0;

	// open file
	fp = fopen("file.hr", "r");
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);

	text = calloc(file_size + 1, sizeof(char));
	fseek(fp, 0, SEEK_SET);

	fread(text, file_size, 1, fp);

	fclose(fp);

	tokenize();
	int i = 0;
	while (i <= tk_pos)
	{
		switch (tokens[i].type)
		{
		case identifier_tk:
			printf("type: identifier, value: '%s'\n", tokens[i].value.char_value);
			break;
		case character_tk:
			printf("type: character, value: '%s'\n", tokens[i].value.char_value);
			break;
		case number_tk:
			printf("type: number, value: '%d'\n", tokens[i].value.int_value);
			break;
		case operation_tk:
			printf("type: operation, value: '%c'\n", tokens[i].value.int_value);
			break;
		case eof_tk:
			printf("type: EOF\n");
			break;
		default:
			printf("error unknown token type: %d\n", tokens[i].type);
		}
		i++;
	}
	// reset token position
	// tk_pos = 0;
	// parse();
}
