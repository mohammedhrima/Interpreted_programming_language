#include "header.h"

// character methods
int ft_isdigit(int c)
{
   return (c >= '0' && c <= '9');
}
int ft_isalpha(int c)
{
   return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
int ft_isalnum(int c)
{
   return (ft_isalpha(c) || ft_isdigit(c));
}
int ft_isspace(int c)
{
   return (c == ' ' || c == '\n' || c == '\t');
}

// string methods
int ft_strlen(char *str)
{
   int i = 0;
   while (str && str[i])
      i++;
   return i;
}
void ft_strncpy(char *dest, char *src, int size)
{
   if (dest == NULL || src == NULL)
      ft_printf(STDERR_FILENO, "receive NULL in strncpy\n");
   int len = ft_strlen(dest);
   int i = 0;
   while (src[i] && i < size)
   {
      dest[len + i] = src[i];
      i++;
   }
}
void ft_strcpy(char *dest, char *src)
{
   if (dest == NULL || src == NULL)
      ft_printf(STDERR_FILENO, "receive NULL in strcpy\n");
   int len = ft_strlen(dest);
   int i = 0;
   while (src[i])
   {
      dest[len + i] = src[i];
      i++;
   }
}
char *ft_strchr(char *s, int c)
{
   int i;

   i = 0;
   if (!s)
      return (NULL);
   while (s[i])
   {
      if (s[i] == (char)c)
         return (s + i);
      i++;
   }
   if (c == 0 && s[i] == 0)
      return (s + i);
   return (NULL);
}
char *ft_strrchr(char *s, int c)
{
   int len;

   len = ft_strlen(s);
   s += len;
   if (c == 0)
      return ((char *)s);
   while (len >= 0)
   {
      if (*s == (char)c)
         return ((char *)s);
      s--;
      len--;
   }
   return (NULL);
}

char *ft_strdup(char *str)
{
   char *res = calloc(ft_strlen(str), sizeof(char));
   ft_strcpy(res, str);
   return (res);
}

char *strjoin(char *string1, char *string2)
{
   char *res = calloc(ft_strlen(string1) + ft_strlen(string2) + 1, sizeof(char));
   if (res == NULL)
      ft_printf(STDERR_FILENO, "malloc failed in strjoin");
   if (string1)
      ft_strcpy(res, string1);
   if (string2)
      ft_strcpy(res + ft_strlen(res), string2);
   return res;
}

int ft_strncmp(char *s1, char *s2, size_t n)
{
   size_t i;

   i = 0;
   while (i < n && s2[i] && s1[i] && (unsigned char)s1[i] == (unsigned char)s2[i])
      i++;
   if (i == n)
      return (0);
   return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int ft_strcmp(char *s1, char *s2)
{
   size_t i;

   i = 0;
   while (s2[i] && s1[i] && (unsigned char)s1[i] == (unsigned char)s2[i])
      i++;
   return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *the_return(char *needle, char *str, size_t len)
{
   size_t i;
   int j;
   size_t k;

   i = 0;
   if (!*needle)
      return (str);
   while (str && str[i] && i < len)
   {
      k = i;
      j = 0;
      while (str[k] == needle[j] && needle[j] && str[k] && k < len)
      {
         k++;
         j++;
      }
      if (!needle[j])
         return (&(str[i]));
      i++;
   }
   return (NULL);
}

char *ft_strnstr(char *haystack, char *needle, size_t len)
{
   char *str;

   if (ft_strncmp(needle, "", 1) == 0)
      return ((char *)haystack);
   str = (char *)haystack;
   return (the_return(needle, str, len));
}
// mems
void *ft_memcpy(void *dst, void *src, size_t n)
{
   size_t i;
   char *ptr1;
   char *ptr2;

   if (!dst)
      return (src);
   if (!src)
      return (dst);
   ptr1 = (char *)dst;
   ptr2 = (char *)src;
   i = 0;
   while (i < n)
   {
      ptr1[i] = ptr2[i];
      i++;
   }
   return (dst);
}

// readline
char *readline(int fd)
{
   int len = 0;
   char *res = NULL;
   char *c = calloc(2, sizeof(char));
   char *tmp;

   while (1)
   {
      int n = read(fd, c, sizeof(char));
      if (n <= 0)
         break;
      tmp = strjoin(res, c);
      free(res);
      res = tmp;
      if (c[0] == '\n' || c[0] == '\0')
         break;
   }
   return res;
}

// ft_printf
void ft_putchar(int fd, char c)
{
   write(fd, &c, sizeof(char));
}

void ft_putstr(int fd, char *str)
{
   int i = 0;
   if (str == NULL)
      ft_putstr(fd, "(null)");
   while (str && str[i])
      ft_putchar(fd, str[i++]);
}

void ft_putnbr(int fd, long num)
{
   if (num < 0)
   {
      ft_putchar(fd, '-');
      num = -num;
   }
   if (num < 10)
      ft_putchar(fd, num + '0');
   else
   {
      ft_putnbr(fd, num / 10);
      ft_putnbr(fd, num % 10);
   }
}

void ft_putfloat(int fd, double num, int decimal_places)
{
   if (num < 0.0)
   {
      ft_putchar(fd, '-');
      num = -num;
   }
   long int_part = (long)num;
   double float_part = num - (double)int_part;
   while (decimal_places > 0)
   {
      float_part = float_part * 10;
      decimal_places--;
   }
   ft_putnbr(fd, int_part);
   ft_putchar(fd, '.');
   ft_putnbr(fd, (long)round(float_part));
}

void print_space(int fd, int line_long, char c)
{
   int i = 0;
   while (i < line_long)
   {
      ft_putchar(fd, c);
      i++;
   }
}
int len_of_num(long num)
{
   int res = 0;
   if (num < 0)
   {
      res++;
      num = -num;
   }
   if (num >= 0)
      res++;
   while (num >= 10)
   {
      num /= 10;
      res++;
   }
   return (res);
}

void ft_printf(int fd, char *fmt, ...)
{
   va_list ap;
   print_space(STDOUT, 3 - (int)len_of_num((long)line), '0');
   ft_putnbr(fd, line);
   ft_putstr(fd, "| ");
   line++;

   va_start(ap, fmt);
   int i = 0;
   while (fmt && fmt[i])
   {
      if (fmt[i] == '%')
      {
         i++;
         int space = -1;
         if (isdigit(fmt[i]))
            space = 0;
         while (isdigit(fmt[i]))
         {
            space = 10 * space + fmt[i] - '0';
            i++;
         }
         if (fmt[i] == 'v')
         {
            var *variable = va_arg(ap, var *);
            if (variable)
            {

               ft_putstr(fd, "name: ");
               ft_putstr(fd, variable->name);
               ft_putstr(fd, ", type: ");
               ft_putstr(fd, to_string(variable->type));
               ft_putstr(fd, ", index: ");
               ft_putnbr(fd, (long)variable->curr_index);
               ft_putstr(fd, ", value: '");
               if (variable->type == characters_)
                  ft_putstr(fd, variable->value.string);
               if (variable->type == integer_ || variable->type == float_)
                  ft_putfloat(fd, variable->value.number, 6);
               if (variable->type == boolean_)
               {
                  if (variable->value.boolean)
                     ft_putstr(fd, "true");
                  else
                     ft_putstr(fd, "false");
               }
               ft_putstr(fd, "'");
               if (variable->temporary)
                  ft_putstr(fd, ", temporary");
               else
                  ft_putstr(fd, ", permanent");
            }
            else
               ft_putstr(fd, "(null obj)");
         }
         if (fmt[i] == 'd')
         {
            if (space == 0)
               space = va_arg(ap, int);
            int num = va_arg(ap, int);
            if (space > 0)
               space -= len_of_num((long)num);
            print_space(fd, space, ' ');
            ft_putnbr(fd, (long)num);
         }
         if (fmt[i] == 'f')
         {
            double num = va_arg(ap, double);
            ft_putfloat(fd, num, 6);
         }
         if (fmt[i] == 'c')
         {
            space--;
            int c = va_arg(ap, int);
            ft_putchar(fd, c);
         }
         if (fmt[i] == 's')
         {
            if (space == 0)
               space = va_arg(ap, int);
            char *str = va_arg(ap, char *);
            if (space > 0)
               space -= ft_strlen(str);
            print_space(fd, space, ' ');
            ft_putstr(fd, str);
         }
      }
      else
         ft_putchar(fd, fmt[i]);
      i++;
   }
   va_end(ap);
   if (fd == STDERR_FILENO)
   { // exit(1);
   }
}

// signals
void handle_signal(int signum)
{
   exit(0);
}

// ats
long ft_atoi(char *str)
{
   long res = 0;
   long sign = 1;
   int i = 0;

   while (ft_isspace(str[i]))
      i++;
   if (str[i] == '+')
      i++;
   else if (str[i] == '-')
   {
      i++;
      sign = -1;
   }
   while (ft_isdigit(str[i]))
   {
      res = res * 10 + (str[i] - '0');
      i++;
   }
   return sign * res;
}

double ft_atof(char *str)
{
   double res = 0.0;
   double sign = 1.0;
   double fraction = 0.1;
   int i = 0;

   while (ft_isspace(str[i]))
      i++;
   if (str[i] == '+')
      i++;
   else if (str[i] == '-')
   {
      i++;
      sign = -1.0;
   }
   while (ft_isdigit(str[i]))
   {
      res = res * 10.0 + (str[i] - '0');
      i++;
   }
   if (str[i] == '.')
   {
      i++;
      while (ft_isdigit(str[i]))
      {
         res = res + fraction * (str[i] - '0');
         fraction *= 0.1;
         i++;
      }
   }
   return sign * res;
}