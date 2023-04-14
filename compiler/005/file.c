#include "header.h"
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

var *new_var(char *name, data type, bool is_temporary)
{

   static int per_index;
   static int tmp_index;
   var *var_exist = NULL;

   if (is_temporary)
   {
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

Token *new_token(var *variable)
{
   Token *new = calloc(1, sizeof(Token));
   new->variable = variable;
   new->right = NULL;
   return new;
}

var *Interpret(char *str)
{
   int start = 0;
   int end = 0;
   int len = ft_strlen(text);
   // ft_printf(STDOUT, "%s\n", str);
   while (pos < len) // can segfault
   {
      skip_space();
      // visualizing
      if (ft_strcmp("vars", text + pos) == 0)
         visualize_variables();
      else
      {
         indexes();
         assign();
         ft_printf(STDOUT,"exit assign with pos: '%d'\n", pos);
      }
      ft_printf(STDOUT,"loop\n");
      pos++;
   }
   return (NULL);
}

int main(void)
{
   signal(SIGINT, handle_signal);

   while (1)
   {
      pos = 0;
      ft_printf(STDOUT, "");
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