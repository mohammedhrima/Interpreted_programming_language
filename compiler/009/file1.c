// #include "header.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
char *text;
int pos;

typedef enum
{
   string_ = 256,
   number_,
   identifier_,
} Type;

typedef struct Token Token;

struct Token
{

   int type;
   int int_val;
   char *str;
   char *name;
};

Token tokens[1000000];
int ct = 0;

void skip_space()
{
   while (isspace(text[pos]))
      pos++;
}

Token *tokenize(void)
{
   int start = 0;
   int end = 0;
   int j = 0;
   int i = 0;
   while (text[i])
   {
      while (isspace(text[i]))
         i++;
      if (!text[i])
         break;
      Token *token = &tokens[j];

      if (isdigit(text[i]))
      {
         token->type = number_;
         while (isdigit(text[i]))
         {
            token->int_val = token->int_val * 10 + (text[i] - '0');
            i++;
         }
      }
      else if (isalpha(text[i]))
      {
         token->type = identifier_;
         int j = i;
         while (isalnum(text[i]))
            i++;
         token->name = malloc(i - j + 1);
         memcpy(token->name, text + j, i - j);
         token->name[i - j] = 0;
      }
      else if (text[i] == '"')
      {
         i++;
         int j = i;
         while (text[i] != '"')
            i++;
         char *str = malloc(i - j + 1);
         memcpy(str, text + j, i - j);
         str[i - j] = 0;
         token->type = string_;
         token->str = str;
         i++;
         // return token;
      }
      else if (strchr("=+-*/()", text[i]))
      {
         token->type = text[i];
         i++;
      }
      else
      {
         printf("error: unknown token '%c'\n", text[i]);
         exit(0);
      }
      j++; // tokens indexes
   }
   tokens[j].type = 0;
   return tokens;
}
enum Node_Type {
   NODE_BINARY,
   NODE_NUMBER,
   NODE_VAR,
   NODE_STRING,
   NODE_ASSIGN,
};

typedef struct Var {
   Token *token;
   char *name;
   int int_val;
} Var;

typedef struct Node
{
   struct Node *left;
   struct Node *right;
   int type;
   Token *token;
   Var *var;
} Node;

void skip(int type)
{
   if (tokens[ct].type != type)
   {
      printf("error: expected another token\n");
      exit(1);
   }
   ct++;
}

Node *new_node(int type)
{
   Node *node = calloc(1, sizeof(Node));
   node->type = type;
   node->token = &tokens[ct];
   return node;
}

Var vars[10000];
int var_count;

Var *add_var(char *name)
{
   Var *v = &vars[var_count++];
   v->name = name;
   v->token = &tokens[ct];
   return v;
}

Var *find_var(char *name)
{
   for (int i = 0; i < var_count; i++)
      if (!strcmp(name, vars[i].name))
         return &vars[i];
   return 0;
}

Node *assign();
Node *add();
Node *mul();
Node *primary();

Node *expr()
{
   return assign();
}

Node *assign()
{
   Node *left = add();

   if (tokens[ct].type == '=')
   {
      Node *node = new_node(NODE_ASSIGN);
      ct++;
      node->left = left;
      if (node->left->type != NODE_VAR)
      {
         printf("error: expected a variable '='\n");
         exit(0);
      }
      node->right = assign();
      return node;
   }
   return left;
}

Node *add()
{
   Node *left = mul();
   while (tokens[ct].type == '+' || tokens[ct].type == '-')
   {
      Node *node = new_node(NODE_BINARY);
      ct++;
      node->left = left;
      node->right = mul();
      left = node;
   }
   return left;
}

Node *mul()
{
   Node *left = primary();
   while (tokens[ct].type == '*' || tokens[ct].type == '/')
   {
      Node *node = new_node(NODE_BINARY);
      ct++;
      node->left = left;
      node->right = primary();
      left = node;
   }
   return left;
}

Node *primary()
{
   if (tokens[ct].type == '(')
   {
      ct++;
      Node *node = expr();
      skip(')');
      return node;
   }
   else if (tokens[ct].type == number_)
   {
      Node *node = new_node(NODE_NUMBER);
      ct++;
      return node;
   }
   else if (tokens[ct].type == identifier_)
   {
      Node *node = new_node(NODE_VAR);
      node->var = find_var(node->token->name);
      if (!node->var)
         node->var = add_var(node->token->name);
      ct++;
      return node;
   }
   else if (tokens[ct].type == string_)
   {
      Node *node = new_node(NODE_STRING);
      ct++;
      return node;
   }
   else
   {
      printf("error: expected an expression\n");
      exit(1);
   }
}

#include <assert.h>

int eval(Node *node)
{
   if (node->type == NODE_NUMBER)
      return node->token->int_val;
   else if (node->type == NODE_VAR)
      return node->var->int_val;
   else if (node->type == NODE_ASSIGN)
   {
      node->left->var->int_val = eval(node->right);
      return node->left->var->int_val;
   }
   // else if ()
   // {
   //    while (eval(node->condition))
   //    {
   //       eval(node->body);
   //    }
   // }
   else if (node->type == NODE_BINARY)
   {
      int left = eval(node->left);
      int right = eval(node->right);
      switch (node->token->type)
      {
         case '+': return left + right;
         case '-': return left - right;
         case '/': return left / right;
         case '*': return left * right;
         default: assert(0);
      }
   }
   assert(0);
}

int main(void)
{
   // signal(SIGINT, handle_signal);
   FILE *fp = NULL;
   long file_size = 0;

   fp = fopen("file.hr", "r");
   // get end of file
   fseek(fp, 0, SEEK_END);
   file_size = ftell(fp);

   text = calloc(sizeof(char) + 1, file_size);
   fseek(fp, 0, SEEK_SET);

   fread(text, file_size, 1, fp);
   fclose(fp);
   // printf("\'%s\'", text);
   Token *tokens = tokenize();
   for (int i = 0; tokens[i].type; i++)
   {
      printf("type: ");
      if (tokens[i].type == string_)
         printf("string, str:\"%s\"", tokens[i].str);
      else if (tokens[i].type == number_)
         printf("number, val:%d", tokens[i].int_val);
      else if (tokens[i].type == identifier_)
         printf("identfier, name:\"%s\"", tokens[i].name);
      else
         printf("%c", tokens[i].type);
      printf("\n");
   }
   while (tokens[ct].type)
      printf("%d\n", eval(expr()));
   
}