#include "header.h"

char *to_string(data type)
{
   char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
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

// start , start index of identifier
// end, end index of identifier
var *assign1()
{
   // int start;
   // int end;
   // while (text[pos]) // can segault
   // {
   //    skip_space();
   //    start = pos;
   //    skip_alpha_num();
   //    end = pos - 1;
   //    skip_space();

   //    char *name = get_identifier(start, end);
   //    ft_printf(STDOUT, "get name from %d to %d: '%s', pos:'%d'\n", start, end, name, pos);

   //    var *per = get_variable_from_stock(name);
   //    if (per == NULL)
   //       per = new_var(name, none_, false);

   //    pos++;
   //    //  call assign if find another =
   //    skip_space();
   //    if (text[pos] == '=')
   //    {
   //       pos++;
   //       ft_printf(STDOUT, "name: '%s'\n", name);
   //       var *next = assign();
   //       ft_memcpy(per, next, sizeof(var));
   //       return per;
   //    }
   //    else
   //    {
   //       ft_printf(STDOUT, "line 73\n");
   //       /*
   //       here check if it's a varibale
   //       string, number, wherever ...
   //       */
   //       if (text[pos] == '\"' || text[pos] == '\'')
   //       {
   //          // define type
   //          per->type = characters_;
   //          // get value
   //          char left_coat = text[pos];
   //          pos++;
   //          start = pos;
   //          while (text[pos] && text[pos] != left_coat)
   //             pos++;
   //          // expect right coat
   //          if (text[pos] != left_coat)
   //          {
   //             ft_printf(STDERR, "%0s\n", pos + 1, "^");
   //             ft_printf(STDERR, "expecting '%c' in index %d\n", pos + 1);
   //             return NULL;
   //          }
   //          end = pos - 1;
   //          per->value.string = get_identifier_name(start, end);
   //          ft_printf(STDOUT, "return per with value : '%s'\n", per->value.string);
   //          return per;
   //       }
   //    }

   //    // pos++;
   // }
   return (NULL);
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

// return boolean
var *less_than_more_than(var *left, var *right, int operation)
{
   // char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
   if (left->type != right->type)
   {
      ft_printf(STDERR, "can't compare type '%s' to '%s'\n", to_string(left->type), to_string(right->type));
      return (false);
   }
   data type = left->type;
   var *temporary = new_var("<>", boolean_, true);
   if (type == integer_ || type == float_)
   {
      bool res = false;
      if (operation == '>')
      {
         res = left->value.number > right->value.number;
         temporary->value.boolean = left->value.number > right->value.number;
         // ft_printf(STDOUT, "do > res: %d\n", res);
      }
      if (operation == '<')
      {
         res = left->value.number < right->value.number;
         temporary->value.boolean = left->value.number < right->value.number;
         // ft_printf(STDOUT, "do < res: %d\n", res);
      }
   }
   else
      ft_printf(STDOUT, "verify types\n");
   ft_printf(STDOUT, "return %v\n", temporary);
   return (temporary);
}

var *math_operation(var *left, var *right, int operation)
{
   // char *types_stock[10] = {"characters", "integer", "float", "boolean", "none"};
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
         return temporary;
      }
      if (operation == '-')
      {
         temporary->type = type;
         temporary->value.number = left->value.number - right->value.number;
         return temporary;
      }
      if (operation == '*')
      {
         temporary->type = type;
         temporary->value.number = left->value.number * right->value.number;
         return temporary;
      }
      if (operation == '/')
      {
         temporary->type = type;
         temporary->value.number = left->value.number / right->value.number;
         return temporary;
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
   return temporary;
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
      if (text[pos] == '"')
      {
         int start = ++pos;
         while (text[pos] != '"')
            pos++;
         if(text[pos] != '"')
         {
            //error expect "
         }
         ft_printf(STDOUT, "get identifier from %d to %d\n", start, pos - 1);
         char *string = get_identifier(start, pos - 1);
         ft_printf(STDOUT, "string value: %s\n", string);
         var *variable = new_var("tmp", characters_, true);
         variable->value.string = string;
         ft_printf(STDOUT, "value: '%s'\n", string);
         left = new_token(variable);
         return left;
      }
      else
      {
         /*
         check if next to me '='
         call assign again
         */
         start = pos;
         while (ft_isalpha(text[pos]))
            pos++;
         // variable name
         char *left_name = get_identifier(start, pos - 1);
         ft_printf(STDOUT, "name: '%s', pos: '%d'\n", left_name, pos);
         var *variable = new_var(left_name, none_, false);
         int left_index = variable->curr_index;
         left = new_token(variable);
         if (text[pos] == '=')
         {
            pos++;
            ft_printf(STDOUT, "found '='\n");
            ft_printf(STDOUT, "get value from '%s'\n", text + pos);
            // if (left->right)
            // {
            left->right = assign();
            memcpy(left->variable, left->right->variable, sizeof(var));
            left->variable->name = left_name;
            left->variable->curr_index = left_index;
            //if(left->right->variable)
            //free temporary variable
         }
         break;
      }
      break;
   }
   return left;
}