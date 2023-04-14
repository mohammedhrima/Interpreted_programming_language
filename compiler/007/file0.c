
#include "header.h"

var *new_var(char *name, data type, bool is_temporary)
{
   static int per_index;
   static int tmp_index;
   var *var_exist = NULL;

   if (is_temporary)
   {
      // ft_printf(STDOUT, "var name '%s' in index %d, temporary\n", name, tmp_index);
      temporaries[tmp_index] = calloc(1, sizeof(var));
      temporaries[tmp_index]->name = name;
      temporaries[tmp_index]->type = type;
      temporaries[tmp_index]->curr_index = tmp_index;
      temporaries[tmp_index]->temporary = true;
      tmp_index++;
      return temporaries[tmp_index - 1];
   }
   if (get_variable_from_stock(name))
   {
      ft_printf(STDERR, "variable '%s' already exist\n", name);
      return NULL;
   }
   else
   {
      // ft_printf(STDOUT, "var name '%s' in index %d, permanent\n", name, per_index);
      variables[per_index] = calloc(1, sizeof(var));
      variables[per_index]->name = name;
      variables[per_index]->type = type;
      variables[per_index]->curr_index = per_index;
      variables[per_index]->temporary = false;
      per_index++;
      return variables[per_index - 1];
   }
   return (NULL);
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

var *Interpret(char *str)
{
   // skip_space(); // check if add it to variable
   int start = pos;
   int end = pos;
   int len;
   int skiped;
   var *new = NULL;
   char *bool_value = NULL;
   int operation;
   char *name;

   while (text[pos])
   {

      // skip spaces
      skip_space();
      // get variables
      if (ft_isalpha(text[pos]))
      {
         // running vars
         if (ft_strcmp("vars", text + pos) == 0)
         {
            pos += 4;
            visualize_variables();
         }
         else if (ft_strnstr(text + pos, "func", ft_strlen("func")))
         {
            pos += ft_strlen("func");
            if (text[pos] != ' ')
            {
               ft_printf(STDERR, "%0s\n", pos + 1, "^");
               ft_printf(STDERR, "expecting ' ' in index %d\n", pos);
               break;
               // exit or something
            }
            // ft_printf(STDOUT, "is functions\n");
            start = pos + ft_strlen("func");
            end = start;
            while (text[end] && text[end] != '(')
               end++;
            if (text[end] != '(')
            {
               ft_printf(STDERR, "%0s\n", end + 1, "^");
               ft_printf(STDERR, "expecting '(' in index %d\n", end);
               break;
               // exit or something
            }
            start = end;
            while (text[end] && text[end] != ')')
               end++;
            if (text[end] != ')')
            {
               ft_printf(STDERR, "%0s\n", end + 1, "^");
               ft_printf(STDERR, "expecting ')' in index %d\n", end);
               break;
               // exit or something
            }
            if (end > start + 1)
            {
               int prams_index = 0;
               char *params[200];
               memset(params, 0, 200);
               params[prams_index++] = get_variable_name(start, end);
               end++; // skip (
               start = end;
               params[prams_index++] = get_variable_name(start, end);
               ft_printf(STDOUT, "function name '%s' takes arguments \n", name);
               int i = 0;
               while (params[i])
               {
                  ft_printf(STDOUT, "'%s' \n", params[i]);
                  i++;
               }
               ft_printf(STDOUT, "\n");
            }
         }
         // varibales handling
         else
         {
            // get variable name
            start = pos;
            while (ft_isalpha(text[pos]))
               pos++;
            end = pos;
            while (ft_isdigit(text[pos]))
               pos++;
            end = pos;

            skip_space();
            // ft_printf(STDOUT, "pos: %d -> %c\n", pos, text[pos]);
            if (text[pos] == '=')
            {
               pos++; // skip =
               skip_space();
               // len = end - start;
               // name = calloc(len + 1, sizeof(char));
               // ft_strncpy(name, text + start, len);
               name = get_variable_name(start, end);
               new = get_variable_from_stock(name);
               if (new == NULL)
                  new = new_var(name, none_, false);
               // set name
               skip_space();

               // set value and data type
               // chars
               if (text[pos] == '"' || text[pos] == '\'')
               {
                  int left_quotes_index = pos++; // save index of first quotes
                  start = pos;
                  while (text[pos] && text[pos] != text[left_quotes_index])
                     pos++;
                  if (text[pos] != text[left_quotes_index])
                  {
                     // get back to left
                     // var_index--;
                     // free(new);
                     // new = NULL;
                     ft_printf(STDERR, "%0s\n", pos + 1, "^");
                     ft_printf(STDERR, "expecting \'%c\' in index %d\n", text[left_quotes_index], pos);
                     line += 2;
                     break;
                  }
                  else
                  {
                     new->type = characters_;
                     len = pos - left_quotes_index - 1;
                     new->value.string = calloc(len + 1, sizeof(char));
                     ft_strncpy(new->value.string, text + left_quotes_index + 1, len);
                     pos++;
                     // ft_printf(STDOUT, "%v\n", new);
                  }
                  // return new;
               }
               // number
               else if (ft_isdigit(text[pos]))
               {
                  if (ft_strchr(text + pos, '.'))
                     new->type = float_;
                  else
                     new->type = integer_;
                  new->value.number = ft_atof(text + pos);
                  while (ft_isdigit(text[pos]))
                     pos++;
               }
               else if (ft_strncmp(text + pos, "true", ft_strlen("true")) == 0 || ft_strncmp(text + pos, "false", ft_strlen("false")) == 0)
               {
                  // expect new line !!
                  new->type = boolean_;
                  if (ft_strnstr(text + pos, "true", 4))
                  {
                     new->value.boolean = true;
                     pos += 4;
                  }
                  if (ft_strnstr(text + pos, "false", 5))
                  {
                     new->value.boolean = false;
                     pos += 5;
                  }
                  // return new;
               }
               // asignement from another variable
               else if (ft_isalpha(text[pos]))
               {
                  start = pos;
                  end = start;
                  while (ft_isalpha(text[end]) || ft_isdigit(text[end]))
                     end++;
                  // get varibale name
                  char *right_name = get_variable_name(start, end);
                  pos = end;
                  // find if this varibale exists in stock
                  // if so return it
                  var *right_var = get_variable_from_stock(right_name);
                  // visualize_variables();
                  if (right_var)
                  {
                     assign_var(new, right_var);
                     // ft_printf(STDOUT, "exist\n");
                  }
                  else
                     ft_printf(STDOUT, "Unknown variable \'%s\'\n", right_name);
                  // visualize_variables();
               }
               else
                  ft_printf(STDERR, "Unknown data type\n");
               // skip spaces at end of declaration or assignment
               while (ft_isspace(text[pos]))
                  pos++;
               // expect new line !!
               if (text[pos] != '\0')
               {
                  ft_printf(STDERR, "%0s\n", pos + 1, "^");
                  ft_printf(STDERR, "expecting 'new line' in index %d\n", pos);
                  line += 2;
                  break;
               }
            }
            else if (text[pos] && ft_strchr("><-+/*", text[pos])) // == 1 didn't work, check after why
            {
               // ft_printf(STDOUT, "%d: found '%c' in pos: %d\n", __LINE__, text[pos], pos);
               start = pos - 1;
               while (start > 0 && ft_isspace(text[start]))
                  start--;
               end = start + 1;
               while (start > 0 && (ft_isalpha(text[start]) || ft_isdigit(text[start])))
                  start--;
               if (ft_isspace(text[start]))
                  start++;
               // left varaiable
               char *left_name = get_variable_name(start, end);

               start = pos + 1;
               while (ft_isspace(text[start]))
                  start++;
               end = start;
               while ((ft_isalpha(text[end]) || ft_isdigit(text[end])))
                  end++;
               // right name
               char *right_name = get_variable_name(start, end);
               if (ft_strlen(right_name) == 0)
               {
                  ft_printf(STDERR, "%0s\n", pos + 3, "^");
                  ft_printf(STDERR, "expected expression\n");
                  break;
               }
               // if (ft_strlen(left_name) == 0)
               // {
               //    ft_printf(STDERR, "%0s\n", pos - 1, "^");
               //    ft_printf(STDERR, "expected expression\n");
               //    break;
               // }
               if (text[pos] == '<' || text[pos] == '>')
               {
                  operation = text[pos];
                  // ft_printf(STDOUT, "do '%s' '%c' '%s'\n", left_name, operation, right_name);
                  var *left_var = get_variable_from_stock(left_name);
                  if (left_var == NULL)
                     ft_printf(STDERR, "Unknown variable '%s'\n", left_name);
                  // ft_printf(STDOUT, "%v\n", left_var);
                  var *right_var = get_variable_from_stock(right_name);
                  if (right_var == NULL)
                     ft_printf(STDERR, "Unknown variable '%s'\n", right_name);
                  // ft_printf(STDOUT, "%v\n", right_var);
                  var *res;
                  if (right_var && left_var)
                     res = less_than_more_than(left_var, right_var, operation);
                  ft_printf(STDOUT, "line %d: %v\n", __LINE__, res);
                  // return new;
               }
               if (is_math_operation(text[pos]))
               {
                  operation = text[pos];
                  // ft_printf(STDOUT, "do '%s' '%c' '%s'\n", left_name, operation, right_name);
                  var *left_var = get_variable_from_stock(left_name);
                  if (left_var == NULL)
                     ft_printf(STDERR, "Unknown variable '%s'\n", left_name);
                  var *right_var = get_variable_from_stock(right_name);
                  if (right_var == NULL)
                     ft_printf(STDERR, "Unknown variable '%s'\n", right_name);
                  if (right_var && left_var)
                     new = math_operation(left_var, right_var, operation);
                  ft_printf(STDOUT, "line %d: %v\n", __LINE__, new);
                  // return new;
               }

               // return (new);
            }
         }
      }
      // else
      //     ft_printf(STDERR, "syntax error\n");
      pos++;
   }
   return NULL;
}

int main(void)
{
   signal(SIGINT, handle_signal);
   while (1)
   {
      pos = 0;
      ft_printf(STDOUT, "");
      // line++;
      text = readline(STDIN);
      if (text)
      {
         if (text[0] == '\n')
            continue;
         text[ft_strlen(text) - 1] = '\0'; // replace \n with \0
         var *variable = Interpret(text);
      }
      free(text);
   }
}
