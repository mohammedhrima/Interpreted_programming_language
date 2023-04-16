#include "header.h"

char *to_string(data type)
{
   char *types_stock[10] = {"characters", "integer", "float", "boolean", "array", "none"};
   return (types_stock[type]);
}

// void print_variabel(var * variable)
// {
//     ft_printf(STDOUT)
// }

void assign_var(var *dest, var *src)
{
   if (dest->type != none_ && dest->type != src->type)
      ft_printf(STDERR, "can't assign '%s' type '%s' to '%s' type '%s'\n", src->name, to_string(src->type), dest->name, to_string(dest->type));
   else
   {
      int dest_index = dest->curr_index;
      char *dest_name = dest->name;

      ft_memcpy(dest, src, sizeof(var));
      dest->curr_index = dest_index;
      dest->name = dest_name;
   }
   // ft_printf(STDOUT, "conflit in data type between '%s' type '%s' and '%s' type '%s'\n", src->name, to_string(src->type), dest->name, to_string(dest->type));
}

void skip_alpha_num(void)
{
   while (ft_isalpha(text[pos]))
   {
      pos++;
      while (ft_isdigit(text[pos]))
         pos++;
   }
}

char *get_identifier(int start, int end)
{
   char *name = calloc(end - start + 2, sizeof(char));
   ft_strncpy(name, text + start, end - start + 1);
   return name;
}

var *get_variable_from_stock(char *name)
{
   int i = 0;
   while (variables[i])
   {
      if (!ft_strcmp(name, variables[i]->name))
         return variables[i];

      i++;
   }
   return NULL;
}

var *operation_(var *left, var *right, int operation)
{
   // char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
   ft_printf(STDOUT, "receive left: %v\n", left);
   ft_printf(STDOUT, "receive right: %v\n", right);
   ft_printf(STDOUT, "receive opearation: '%c'\n", operation);

   data type = left->type;
   var *temporary = NULL;
   if (left->type != right->type)
   {
      ft_printf(STDERR, "can't do '%c' between '%s' and '%s'\n", operation, to_string(left->type), to_string(right->type));
      return (NULL);
   }
   if (type == integer_ || type == float_)
   {
      // create variable with same type as left and right
      temporary = new_var("operation", type, true);
      if (operation == '+')
      {
         temporary->type = type;
         temporary->value.number = left->value.number + right->value.number;
         ft_printf(STDOUT, "result '%f'\n", temporary->value.number);
         return temporary;
      }
      else if (operation == '-')
      {
         temporary->type = type;
         temporary->value.number = left->value.number - right->value.number;
         return temporary;
      }
      else if (operation == '*')
      {
         temporary->type = type;
         temporary->value.number = left->value.number * right->value.number;
         return temporary;
      }
      else if (operation == '/')
      {
         temporary->type = type;
         temporary->value.number = left->value.number / right->value.number;
         return temporary;
      }
      else if (operation == '<')
      {
         temporary->type = boolean_;
         temporary->value.boolean = left->value.number < right->value.number;
         return temporary;
      }
      else if (operation == '>')
      {
         temporary->type = boolean_;
         temporary->value.boolean = left->value.number > right->value.number;
         return temporary;
      }
      else
      {
         ft_printf(STDERR, "Unknow opearation '%c'\n", operation);
         exit(1);
      }
   }
   else if (type == characters_)
   {
      // create variable with same type as left and right
      temporary = new_var("operation", type, true);
      if (operation == '+')
      {
         char *joined_string = calloc(ft_strlen(left->value.string) + ft_strlen(right->value.string) + 1, sizeof(char));
         ft_strcpy(joined_string + ft_strlen(joined_string), left->value.string);
         ft_strcpy(joined_string + ft_strlen(joined_string), right->value.string);
         temporary->value.string = joined_string;
         return temporary;
      }
      if (operation == '-')
      {
         temporary->type = none_;
         ft_printf(STDERR, "can't substract '%s' from '%s'\n", to_string(type), to_string(type));
      }
   }
   else
   {
      ft_printf(STDERR, "can't do '%c' to type '%s'\n", operation, to_string(type));
      return (NULL);
   }
   return NULL;
}

var *operation__(var *left, var *right, char *operation)
{
   // char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
   ft_printf(STDOUT, "receive left: %v\n", left);
   ft_printf(STDOUT, "receive right: %v\n", right);
   ft_printf(STDOUT, "receive opearation: '%s'\n", operation);

   data type = left->type;
   var *temporary = NULL;
   if (left->type != right->type)
   {
      ft_printf(STDERR, "can't do '%s' between '%s' and '%s'\n", operation, to_string(left->type), to_string(right->type));
      return (NULL);
   }
   if (type == integer_ || type == float_)
   {
      // create variable with same type as left and right
      temporary = new_var("operation", type, true);
      if (ft_strncmp("==", operation, 2) == 0)
      {
         temporary->type = boolean_;
         temporary->value.boolean = !(left->value.number - right->value.number);
         ft_printf(STDOUT, "result '%d'\n", temporary->value.boolean);
         return temporary;
      }
      else
      {
         ft_printf(STDERR, "Unknow opearation '%c'\n", operation);
         exit(1);
      }
   }
   else if (type == characters_)
   {
      // create variable with same type as left and right
      // temporary = new_var("operation", type, true);
      // if (operation == '+')
      // {
      //    char *joined_string = calloc(ft_strlen(left->value.string) + ft_strlen(right->value.string) + 1, sizeof(char));
      //    ft_strcpy(joined_string + ft_strlen(joined_string), left->value.string);
      //    ft_strcpy(joined_string + ft_strlen(joined_string), right->value.string);
      //    temporary->value.string = joined_string;
      //    return temporary;
      // }
      // if (operation == '-')
      // {
      //    temporary->type = none_;
      //    ft_printf(STDERR, "can't substract '%s' from '%s'\n", to_string(type), to_string(type));
      // }
   }
   else
   {
      ft_printf(STDERR, "can't do '%s' to type '%s'\n", operation, to_string(type));
      return (NULL);
   }
   return NULL;
}

int is_math_operation(int c)
{
   return (c == '-' || c == '+' || c == '/' || c == '*');
}

int skip_space()
{
   int skiped_space = 0;
   while (text && isspace(text[pos]))
   {
      pos++;
      skiped_space++;
   }
   return (skiped_space);
}

Token *assign()
{
   // ft_printf(STDOUT, "call assign\n");
   int len = ft_strlen(text);
   int start;
   Token *left = NULL;
   while (pos < len)
   {
      /*
      check if it starts with " -> getting string
      return Token with variable type temporary
      copy it to current variable
      ---------------------------------------------
      else i'm getting a variable
      */
      skip_space();
      if (text[pos] == '"' || text[pos] == '\'')
      {
         char left_coat = text[pos];
         int start = ++pos;
         while (text[pos] && text[pos] != left_coat)
            pos++;
         if (text[pos] != left_coat)
         {
            ft_printf(STDERR, "%0s\n", pos + 1, "^");
            ft_printf(STDERR, "expecting '%c' in index %d\n", left_coat, pos);
            exit(-1);
            break;
            // exit or something
         }
         // ft_printf(STDOUT, "get identifier from %d to %d\n", start, pos - 1);
         char *string = get_identifier(start, pos - 1);
         // ft_printf(STDOUT, "string value: %s\n", string);
         var *variable = new_var("tmp", characters_, true);
         variable->value.string = string;
         // ft_printf(STDOUT, "value: '%s'\n", string);
         pos++; // skip last quots
         left = new_token(variable);
         // skip_space();
         return left;
      }
      // could be a number
      else if (ft_isdigit(text[pos]))
      {
         var *variable = new_var("tmp", integer_, true);
         double number = 0.0;
         double presision = 0.0;
         while (ft_isdigit(text[pos]))
         {
            number = 10 * number + text[pos] - '0';
            pos++;
         }
         if (text[pos] == '.')
         {
            variable->type = float_;
            pos++;
            presision = 0.0;
            while (ft_isdigit(text[pos]))
            {
               presision = 0.1 * (presision + text[pos] - '0');
               pos++;
            }
         }
         // expect new line or something
         //
         //
         number += presision;
         variable->value.number = number;
         left = new_token(variable);
         return left;
      }
      else if (ft_strncmp(text + pos, "true", ft_strlen("true")) == 0 || ft_strncmp(text + pos, "false", ft_strlen("false")) == 0)
      {
         var *variable = new_var("tmp", boolean_, true);
         if (ft_strnstr(text + pos, "true", 4))
         {
            variable->value.boolean = true;
            pos += 4;
         }
         if (ft_strnstr(text + pos, "false", 5))
         {
            variable->value.boolean = false;
            pos += 5;
         }
         left = new_token(variable);
         return left;
      }
      else if (ft_isalpha(text[pos]))
      {
         /*
         declaration of varibale happen here
         check if next to me '='
         call assign again
         */
         start = pos;
         while (ft_isalpha(text[pos]) || ft_isdigit(text[pos]))
            pos++;
         // variable name
         char *left_name = get_identifier(start, pos - 1);
         // ft_printf(STDOUT, "name: '%s', pos: '%d'\n", left_name, pos);

         var *variable = get_variable_from_stock(left_name);
         if (variable == NULL)
            variable = new_var(left_name, none_, false);
         int left_index = variable->curr_index;
         left = new_token(variable);
         skip_space();
         if (text[pos] == '=')
         {
            pos++;
            ft_printf(STDOUT, "found '='\n");
            skip_space();

            if (text[pos] == '[')
            {
               pos++;
               left->variable->type = array_;
               skip_space();
               ft_printf(STDOUT, "found '['\n");
               ft_printf(STDOUT, "left : %v \n", left->variable);
               Token *tmp = left;
               while (text[pos] && text[pos] != ']')
               {
                  // I put tmp here to keep the head of array
                  // and keep addign to temp the nget next token
                  Token *tmp_token = assign();
                  var *tmp_var = tmp_token->variable;
                  tmp->right = new_token(new_var("", none_, false));
                  ft_memcpy(tmp->right->variable, tmp_var, sizeof(var));
                  // tmp->right = assign();
                  ft_printf(STDOUT, "right: %v , got assign form pos: '%d'\n", left->right->variable, pos);
                  skip_space();
               }

               if (text[pos] != ']')
               {
                  ft_printf(STDERR, "%0s\n", pos + 1, "^");
                  ft_printf(STDERR, "expecting ']' in index %d\n", pos);
                  exit(-1);
               }
               pos++; // skip ']'
               return left;
            }

            // ft_printf(STDOUT, "get value from '%s'\n", text + pos);
            // if (left->right)
            // {
            left->right = assign();
            skip_space();
            // array
            if (text[pos] && ft_strchr("+-/*<>", text[pos]))
            {
               int operation = text[pos];
               pos++;
               left->right->right = assign();
               ft_printf(STDOUT, "found add '%f' to '%f'\n", left->right->variable->value.number, left->right->right->variable->value.number);
               memcpy(left->variable, operation_(left->right->variable, left->right->right->variable, operation), sizeof(var));
               // return left;
               // exit(0);
            }
            else if (ft_strncmp("==", text + pos, 2) == 0)
            {
               ft_printf(STDOUT, "==\n");
               pos += 2;
               left->right->right = assign();
               ft_printf(STDOUT, "compare %f and %f\n", left->right->variable->value.number, left->right->right->variable->value.number);
               memcpy(left->variable, operation__(left->right->variable, left->right->right->variable, "=="), sizeof(var));
               // exit(0);
            }
            else
            {
               // memcpy for assignement
               memcpy(left->variable, left->right->variable, sizeof(var));
            }

            // else
            // {
            left->variable->name = left_name;
            left->variable->curr_index = left_index;
            left->variable->temporary = false;

            // if(left->right->variable)
            // free temporary variable
            return left;
            // }
         }

         // break;
      }
      else
      {
         ft_printf(STDERR, "Syntax error '%c'\n", text[pos]);
         exit(-1);
      }
      break;
   }
   return left;
}