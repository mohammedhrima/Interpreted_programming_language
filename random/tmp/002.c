// c headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>
#include <string.h>

// typedefs
typedef struct Node Node;
typedef struct Token Token;
typedef struct Val Val;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(int type);
void output(char *fmt, ...);
Val *get_var(char *name);

#define in STDIN_FILENO
#define out STDOUT_FILENO
#define err STDERR_FILENO

// tokens
typedef enum
{
    none_, // skip 0
    // be carefulll before doing any change here !!!
    identifier_,
    characters_,
    character_,
    boolean_,
    integer_,
    float_,
    assign_,
    equal_,
    not_equal_,
    less_than_,
    more_than_,
    less_than_or_equal_,
    more_than_or_equal_,
    function_,
    if_,
    while_,
    and_,
    or_,
    end_statement,
    tab_,
    lparent_,
    rparent_,
    lbracket_,
    rbracket_,
    array_,
    output_,
    input_,
    comma_,
    Node_,
    add_,
    sub_,
    mul_,
    div_
} Type;

// Val
struct Val
{
    char *name;
    Type type;
    int index;
    // bool permanent;
    union
    {
        double number;    // type integer, float
        char *characters; // type characters
        bool boolean;     // type boolean
        // type array
        struct
        {
            Val *elems;
            int array_size;
        };
        // type object
        struct
        {
            char **keys;
            Val *values;
            int object_size;
        };
    };
};
// Token
struct Token
{
    Val value;
    Type type;
};
// they expect space after keyword
struct
{
    char *name;
    Type type;
} statements_tokens[] = {
    {"if", if_},
    {"while", while_},
    {"or", or_},
    {"and", and_},
    {"func", function_},
    {0, 0},
};
// they expect nothing
struct
{
    char *name;
    Type type;
} operator_tokens[] = {
    {"==", equal_},
    {"!=", not_equal_},
    {"<", less_than_},
    {">", more_than_},
    {"<=", less_than_or_equal_},
    {">=", more_than_or_equal_},
    {"=", assign_},
    {"(", lparent_},
    {")", rparent_},
    {"[", lbracket_},
    {"]", rbracket_},
    {",", comma_},
    {"+", add_},
    {"-", sub_},
    {"*", mul_},
    {"/", div_},
    {":", end_statement},
    {"\t", tab_},
    {0, 0},
};
// they expect '('
struct
{
    char *name;
    Type type;
} functions_tokens[] = {
    {"output", output_},
    {"input", input_},
    {0, 0},
};

// Node
struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

// Global variables
Token *tokens[1000];
int tk_pos;
char *text;
int txt_pos;
Val *PERMANENTS[500];
Val *TEMPORARIES[500];
int per_index;
int tmp_index;
int line;
int start;

// tools
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
    return (c == ' ' || c == '\n');
}

// string methods
int ft_strlen(char *str)
{
    int i = 0;
    while (str && str[i])
        i++;
    return i;
}
void ft_strcpy(char *dest, char *src)
{
    if (dest == NULL || src == NULL)
        ft_printf(STDERR_FILENO, "receive NULL in strncpy\n");
    int len = ft_strlen(dest);
    int i = 0;
    while (src[i])
    {
        dest[len + i] = src[i];
        i++;
    }
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
void ft_putfloat(int fd, double num)
{
    if (num < 0.0)
    {
        ft_putchar(fd, '-');
        num = -num;
    }
    long int_part = (long)num;
    double float_part = num - (double)int_part;
    int decimal_places = 6;
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
    print_space(out, 3 - (int)len_of_num((long)line), '0');
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
            if (ft_isdigit(fmt[i]))
                space = 0;
            while (ft_isdigit(fmt[i]))
            {
                space = 10 * space + fmt[i] - '0';
                i++;
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
            if (fmt[i] == 't')
            {
                Type type = va_arg(ap, Type);
                ft_putstr(fd, type_to_string(type));
            }
            if (fmt[i] == 'v')
            {
                Val *var = va_arg(ap, Val *);
                if (var)
                {
                    ft_putstr(fd, "name: ");
                    ft_putstr(fd, var->name);
                    ft_putstr(fd, ", type: ");
                    ft_putstr(fd, type_to_string(var->type));
                    ft_putstr(fd, ", index: ");
                    ft_putnbr(fd, (long)var->index);
                    ft_putstr(fd, ", value: '");
                    switch (var->type)
                    {
                    case identifier_:
                        break;
                    case characters_:
                    case character_:
                        ft_putstr(fd, var->characters);
                        break;
                    case integer_:
                        ft_putnbr(fd, var->number);
                        break;
                    case float_:
                        ft_putfloat(fd, var->number);
                        break;
                    case boolean_:
                        if (var->boolean)
                            ft_putstr(fd, "true");
                        else
                            ft_putstr(fd, "false");
                        break;
                    case array_:
                        for (int i = 0; i < var->array_size; i++)
                        {
                            switch (var->elems[0].type)
                            {
                            case characters_:
                                ft_putstr(fd, var->elems[i].characters);
                                break;
                            case integer_:
                                ft_putnbr(fd, var->elems[i].number);
                                break;
                            case float_:
                                ft_putfloat(fd, var->elems[i].number);
                                break;
                            case boolean_:
                                if (var->boolean)
                                    ft_putstr(fd, "true");
                                else
                                    ft_putstr(fd, "false");
                                break;
                            default:
                                break;
                            }
                            ft_putchar(fd, ' ');
                        }
                        break;
                    default:
                        ft_putstr(err, "ft_printf didn't know this variable type '");
                        ft_putstr(err, type_to_string(var->type));
                        ft_putstr(err, "'\n");
                        exit(0);
                    }
                    ft_putstr(fd, "'");
                }
                else
                    ft_putstr(fd, "(null var)");
            }
            if (fmt[i] == 'f')
            {
                double num = va_arg(ap, double);
                ft_putfloat(fd, num);
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
    if (fd == err)
        exit(1);
}

// debuging functions
void indexes()
{
    int i = 0;
    ft_printf(out, "");
    while (text && text[i])
    {
        ft_putnbr(out, i % 10);
        i++;
    }
    ft_putstr(out, "\n");
}
void visualize_variables(void)
{
    int i = 0;
    ft_printf(out, "\n");
    ft_printf(out, "permanent variables: \n");
    while (PERMANENTS[i])
    {
        Val *variable = PERMANENTS[i];
        if (variable)
            ft_printf(out, "     %v\n", variable);
        i++;
    }
    i = 0;
    /* ft_printf(out, "temporary variables: \n");
     while (TEMPORARIES[i])
     {
         Val *temporary = TEMPORARIES[i];
         if (temporary)
             ft_printf(out, "     %v\n", temporary);
         i++;
     }*/
}
// parsing functions
char *type_to_string(int type)
{
    switch (type)
    {
    // case eof_:
    //     return "EOF";
    case identifier_:
        return "IDENTIFIER";
    case characters_:
        return "CHARACTERS";
    case character_:
        return "CHARACTER";
    case integer_:
        return "INTEGER";
    case float_:
        return "FLOAT";
    case array_:
        return "ARRAY";
    case assign_:
        return "ASSIGNEMENT";
    case equal_:
        return "EQUAL";
    case not_equal_:
        return "NOT EQUAL";
    case less_than_:
        return "LESS THAN";
    case more_than_:
        return "MORE THAN";
    case less_than_or_equal_:
        return "LESS THAN OR EQUAL";
    case more_than_or_equal_:
        return "MORE THAN OR EQUAL";
    case function_:
        return "FUNCTION";
    case if_:
        return "IF";
    case or_:
        return "OR";
    case and_:
        return "AND";
    case while_:
        return "WHILE LOOP";
    case output_:
        return "OUTPUT";
    case input_:
        return "INPUT";
    case lparent_:
        return "LEFT PARENT";
    case rparent_:
        return "RIGHT PARENT";
    case lbracket_:
        return "LEFT BRACKET";
    case rbracket_:
        return "RIGHT BRACKET";
    case comma_:
        return "COMMA";
    case none_:
        return "NONE";
    case add_:
        return "ADDITION";
    case sub_:
        return "SUBSTRACTION";
    case mul_:
        return "MULTIPLACTION";
    case div_:
        return "DIVISION";
    case boolean_:
        return "BOOLEAN";
    case end_statement:
        return "END STATEMENT";
    case tab_:
        return "TAB";
    default:
        ft_putstr(err, "error not found in type_to_string '");
        ft_putnbr(err, type);
        ft_putstr(err, "'\n");
        exit(0);
    }
    return NULL;
}

// news functions;
Token *new_token(Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->value.index = tk_pos;
    switch (type)
    {
    case identifier_:
        new->value.type = type;
        new->value.name = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->value.name, text + start, txt_pos - start);
        ft_printf(out, "new token type: '%t' with name '%s'\n", type, new->value.name);
        break;
    case characters_:
        new->value.type = type;
        new->value.characters = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->value.characters, text + start, txt_pos - start);
        ft_printf(out, "new token type: '%t' with value characters '%s'\n", type, new->value.characters);
        break;
    case integer_:
    case float_:
        new->value.type = type;
        double num = 0.0;
        while (ft_isdigit(text[start]))
        {
            num = 10 * num + text[start] - '0';
            start++;
        }
        if (text[start] == '.')
            start++;
        double pres = 0.1;
        while (ft_isdigit(text[start]))
        {
            num = num + pres * (text[start] - '0');
            pres /= 10;
            start++;
        }
        new->value.number = num;
        ft_printf(out, "new token type: '%t' with value number '%f'\n", type, new->value.number);
        break;
    case assign_:
    case none_:
    case add_:
    case mul_:
    case sub_:
    case div_:
    case equal_:
    case not_equal_:
    case more_than_:
    case less_than_:
    case more_than_or_equal_:
    case less_than_or_equal_:
    case lparent_:
    case rparent_:
    case rbracket_:
    case lbracket_:
    case output_:
    case if_:
    case comma_:
    case end_statement:
    case tab_:
    case array_:
        ft_printf(out, "new token type: '%t'\n", type);
        break;
    default:
        ft_printf(err, "verify given token '%t'\n", type);
    }
    tokens[tk_pos++] = new;
    tokens[tk_pos] = NULL;
    return new;
}

// tokenize
void build_tokens()
{
    while (text[txt_pos])
    {
        while (ft_isspace(text[txt_pos]))
            txt_pos++;
        if (text[txt_pos] == '\0')
            break;
        int type = 0;
        for (int i = 0; statements_tokens[i].type; i++)
        {
            if (ft_strncmp(statements_tokens[i].name, text + txt_pos, ft_strlen(statements_tokens[i].name)) == 0 && ft_isspace(text[txt_pos + ft_strlen(statements_tokens[i].name)]))
            {
                type = statements_tokens[i].type;
                txt_pos += ft_strlen(statements_tokens[i].name);
                break;
            }
        }
        if (type)
        {
            new_token(type);
            continue;
        }
        for (int i = 0; operator_tokens[i].type; i++)
        {
            if (ft_strncmp(operator_tokens[i].name, text + txt_pos, ft_strlen(operator_tokens[i].name)) == 0)
            {
                type = operator_tokens[i].type;
                txt_pos += ft_strlen(operator_tokens[i].name);
                break;
            }
        }
        if (type)
        {
            new_token(type);
            continue;
        }
        // start = txt_pos;
        for (int i = 0; functions_tokens[i].type; i++)
        {
            if (ft_strncmp(functions_tokens[i].name, text + txt_pos, ft_strlen(functions_tokens[i].name)) == 0)
            {
                type = functions_tokens[i].type;
                txt_pos += ft_strlen(functions_tokens[i].name);
                break;
            }
        }
        if (type)
        {
            new_token(type);
            continue;
        }
        if (ft_isalpha(text[txt_pos]))
        {
            start = txt_pos;
            while (ft_isalnum(text[txt_pos]))
                txt_pos++;
            new_token(identifier_);
            continue;
        }
        if (ft_isdigit(text[txt_pos]))
        {
            Type tmp_type = integer_;
            start = txt_pos;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
            {
                tmp_type = float_;
                txt_pos++;
            }
            double pres = 0.1;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            new_token(tmp_type);
            continue;
        }
        if (text[txt_pos] == '"' || text[txt_pos] == '\'')
        {
            start = txt_pos++;
            while (text[txt_pos] && text[txt_pos] != text[start])
                txt_pos++;
            if (text[txt_pos] != text[start])
                ft_printf(err, "expected '%c'\n", text[start]);
            start++;
            new_token(characters_);
            txt_pos++;
            continue;
        }
        else
            ft_printf(err, "Unknown value s:'%s', c:'%c', d:'%d' \n", text + txt_pos, text[txt_pos], text[txt_pos]);
    }
    new_token(none_);
}

// build nodes
Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    if (new->token->type == integer_)
        ft_printf(out, "new node with type '%t' and value '%f'\n", new->token->type, new->token->value.number);
    else
        ft_printf(out, "new node with type '%t'\n", new->token->type);
    return (new);
}

Val *new_array_node(int start, int end)
{
    Val *new = calloc((end - start) + 2, sizeof(Val));

    int i = 0;
    int j = 0;
    while (start + i < end)
    {
        if (tokens[start + i]->type == comma_)
            i++;
        if (tokens[start + i]->type == identifier_)
        {
            Val *to_find = get_var(tokens[start + i]->value.name);
            if (to_find == NULL)
                ft_printf(err, "in new array variable '%s' not found\n", tokens[start + i]->value.name);
            new[j] = *to_find;
        }
        else
        {
            new[j] = tokens[start + i]->value;
        }
        ft_printf(out, "new[%d]: %v\n", j, &new[j]);
        i++;
        j++;
    }
    return (new);
}

Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();
Node *equality();
Node *comparison();

Node *expr()
{
    if (tokens[tk_pos]->type == if_)
    {
        Node *node = new_node(tokens[tk_pos]);
        tk_pos++;
        // condition
        node->left = expr();
        // expect :
        if (tokens[tk_pos]->type != end_statement)
        {
            ft_printf(err, "expected ':'\n");
        }
        tk_pos++;
        // expect tab
        // bloc
        node->right = expr();
        return node;
    }
    return assign();
}

Node *assign()
{
    Node *left = equality();
    if (tokens[tk_pos]->type == assign_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        tk_pos++;
        node->right = equality();
        left = node;
    }
    return left;
}

Node *equality()
{
    Node *left = comparison();
    if (tokens[tk_pos]->type == equal_ || tokens[tk_pos]->type == not_equal_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        tk_pos++;
        node->right = comparison();
        left = node;
    }
    return left;
}

Node *comparison()
{
    Node *left = add_sub();
    if (
        tokens[tk_pos]->type == less_than_ || tokens[tk_pos]->type == more_than_ ||
        tokens[tk_pos]->type == less_than_or_equal_ || tokens[tk_pos]->type == more_than_or_equal_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        tk_pos++;
        node->right = add_sub();
        left = node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    while (tokens[tk_pos]->type == add_ || tokens[tk_pos]->type == sub_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        tk_pos++;
        node->right = mul_div();
        left = node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    while (tokens[tk_pos]->type == mul_ || tokens[tk_pos]->type == div_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        tk_pos++;
        node->right = prime();
        left = node;
    }
    return left;
}

Node *prime()
{
    Node *left = NULL;
    if (
        tokens[tk_pos]->type == integer_ ||
        tokens[tk_pos]->type == float_ ||
        tokens[tk_pos]->type == characters_ ||
        tokens[tk_pos]->type == identifier_)
    {
        left = new_node(tokens[tk_pos]);
        tk_pos++;
        if (tokens[tk_pos]->type == lbracket_)
        {
            // Node *node = prime();
            left->left = prime();
            // expect right bracket
        }
    }
    else if (tokens[tk_pos]->type == lparent_)
    {
#if 0
        ft_printf(out, "get tuple\n");
        // expect (
        tk_pos++;

        Node *node = expr();
        ft_printf(out, "-> %t\n", node->token->type);
        Node *tmp = node;
        while (tokens[tk_pos]->type != rparent_)
        {
            tmp->left = expr();
            ft_printf(out, "-> %t\n", node->token->type);
            tmp = tmp->left;
        }
        while (node)
        {
            ft_printf(out, "-> %t\n", node->token->type);
            node = node->left;
        }
        exit(0);
        // expect )
        tk_pos++;
        return node;
#endif
    }
    // id did put if instead of else if for case array[index]
    else if (
        tokens[tk_pos]->type == lbracket_)
    {
#if 0
        ft_printf(out, "get array\n");
        tk_pos++; // skip 1st bracket
        int start = tk_pos; // replace token that has LEFT bracket by the HEAD of array
        while (tokens[tk_pos]->type != rbracket_)
        {
            expr();
            if (tokens[tk_pos]->type == comma_)
                tk_pos++;
        }
        // expect RIGHT BRACKET !!!
        ft_printf(out, "start :%d , end :%d\n", start, tk_pos);
        left = new_node(tokens[start]);
        left->token->type = array_;
        left->token->value.elems = new_array_node(start, tk_pos); // replace token that has RIGHT bracket by the END (NONE) of array
        left->token->value.type = array_;
        left->token->value.array_size = (tk_pos - start) / 2 + 1;
        tk_pos++; // skip 2nd bracket
        // expect rbracket
        ft_printf(out, "left token type %t\n", left->token->type);
        // exit(0);
#else
        Node *node = new_node(tokens[tk_pos]);
        node->token->type = array_;
        Node *tmp = node;
        int i = 0;
        tk_pos++;
        ft_printf(out, "get array %t, tk_pos: %d\n", tokens[tk_pos]->type, tk_pos);
        while (tokens[tk_pos] && tokens[tk_pos]->type != rbracket_)
        {
            if (tokens[tk_pos]->type == comma_)
                tk_pos++; // skip comma_
            ft_printf(out, "get array %t, tk_pos: %d, -> %f\n", tokens[tk_pos]->type, tk_pos, tokens[tk_pos]->value.number);
            tmp->left = expr();
            tmp = tmp->left;
            i++;
        }
        tmp->left = NULL;
        // int j = 0;
        // node->token->value.elems = calloc(i + 2,sizeof(Val));
        // while(j < i)
        // {
        //     node->token->value.elems[j] =
        // }
        // tmp->left = new_node(new_token(none_));
        tk_pos++; // skip right bracket
        return node;
#endif
    }
    else if (tokens[tk_pos]->type == output_)
    {
        ft_printf(out, "build output node\n");
        Node *node = new_node(tokens[tk_pos]);
        tk_pos++;             // skip output token
        node->left = prime(); // prime will skip '(' and ')'
        // ft_printf(out, "[%d] -> %t\n", tk_pos, tokens[tk_pos]->type);
        ft_printf(out, "line 955 -> %t\n", node->left->token->type);

        // exit(0);
        return node;
    }
    return left;
}

// execute and create variables
Val *get_var(char *name)
{
    for (int i = 0; i < per_index; i++)
    {
        if (ft_strcmp(PERMANENTS[i]->name, name) == 0)
            return (PERMANENTS[i]);
    }

    for (int i = 0; i < tmp_index; i++)
    {
        if (ft_strcmp(TEMPORARIES[i]->name, name) == 0)
            return (TEMPORARIES[i]);
    }
    // ft_printf(out, "variable '%s' not found\n", name);
    return NULL;
}

Val *new_val(Val *var)
{
    ft_printf(out, "new varibale\n");
    PERMANENTS[per_index++] = var;
    return (var);
}

Val *eval(Node *node)
{
    ft_printf(out, "eval received '%t'\n", node->token->type);
    /*
    in assignement call eval
    when you assign a variable change it's type to data type that it's holding it
    don't assign type variable to variable, because that means that, assigning NONE to NONE
    */
    if (node->token->type == assign_)
    {
        // check data type for existed varbiales
        ft_printf(out, "do assignement between type '%t' and '%t' \n", node->left->token->type, node->right->token->type);
        Val *left = eval(node->left);
        Val *right = eval(node->right);
        ft_printf(out, "left ->'%t' and right -> '%t' \n", left->type, right->type);

        if (left->type != identifier_ && left->type != right->type)
        {
            if (left->type == character_ && right->type == characters_)
                ;
            else
                ft_printf(err, "can't assign '%s' type %t to '%s' type %t \n", left->name, left->type, right->name, right->type);
        }

        ft_printf(out, "in assign received '%v' and '%v' \n", left, right);
        char *name = left->name;
        if (left->type == character_ && right->type == characters_)
        {
            // char *res = calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            // memcpy(res, left->characters, ft_strlen(left->characters));
            // memcpy(res + ft_strlen(res), right->characters, ft_strlen(right->characters));
            memcpy(left->characters, right->characters, ft_strlen(right->characters));
            left->type = characters_;
            // int i = 0;

            // ft_printf(out, "we will return '%v'\n", left);
            // exit(0);
        }
        else
            memcpy(left, right, sizeof(Val)); // to be changed after
        left->name = name;
        ft_printf(out, "after assignement %v\n", left);
        return left;
    }
    else if (node->token->type == identifier_)
    {
        // in case i declared it and saved it before
        //  here i will loke for it and return it with it's value and data type
        Val *exist = get_var(node->token->value.name);
        if (exist)
        {
#if 1
            if (exist->type == array_ && node->left)
            {
                // ft_printf(out, "identifier '%s' has a left '%t'\n", node->token->value.name, node->left->token->type);
                // ft_printf(out, "with value %v' -> '%f'\n", &node->left->token->value.elems[0], node->left->token->value.elems[0].number);
                // ft_printf(out, "identifier '%s' is '%v'\n", node->token->value.name, exist);
                // ft_printf(out, "return %v'\n", &exist->elems[(int)node->left->token->value.elems[0].number]);
                ft_printf(out, "etirate over type array\n");
                return (&exist->elems[(int)node->left->token->value.elems[0].number]);
            }
            else if (exist->type == characters_ && node->left)
            {
                // ft_printf(out, "identifier '%s' has a left '%t'\n", node->token->value.name, node->left->token->type);
                // ft_printf(out, "with value %v' -> '%f'\n", &node->left->token->value.elems[0], node->left->token->value.elems[0].number);
                ft_printf(out, "itirate over type characters\n");

                // ft_printf(out, "return '%c'\n", exist->characters[(int)node->left->token->value.elems[0].number]);
                Val *ptr = calloc(1, sizeof(Val));
                ptr->type = character_;
                // exist->characters[(int)node->left->token->value.elems[0].number] = '\0';
                ptr->characters = exist->characters + (int)node->left->token->value.elems[0].number;
                // ft_printf(out, "return '%v'\n", ptr);
                return ptr;
                // exit(0);
            }
#else
            if (tokens[tk_pos]->type == lbracket_)
            {
                tk_pos++;
                Val *index = eval(prime());
                // verify if it's integer
                tk_pos++;

                // ft_printf(out, "index: %v\n", index);
                ft_printf(out, "1024 from %v\n", exist);
                ft_printf(out, "1025 return %v\n", &exist->elems[(int)index->number]);
                return (&exist->elems[(int)index->number]);
            }
#endif
        }
        if (!exist)
            exist = new_val(&node->token->value);
        ft_printf(out, "1031 return %v\n", exist);
        return (exist);
    }
    else if (node->token->type == integer_ ||
             node->token->type == float_ ||
             node->token->type == characters_ ||
             node->token->type == array_)
    {
        if (node->token->type != array_)
            ft_printf(out, "1039 return '%v'\n", node->token->value);
        else
        {
            ft_printf(out, "left: %t\n", node->token->type);
            ft_printf(out, "I'm returning an array %v\n", node->left->token);

            exit(0);
            // ft_printf(out, "token type now %t\n", tokens[tk_pos]->type);
        }
        return &node->token->value;
    }
    else if (node->token->type == output_)
    {
        // ft_printf(out, "output : '%v'\n", node->left->token->value);
        // ft_printf(out, "output : '%t'\n", node->left->left->token->type);
        // ft_printf(out, "call output\n");
        // ft_printf(out, "%t\n", node->left->token->type);
        Val *left = eval(node->left);
        if (left->type == identifier_)
            ft_printf(err, "error: '%s' not found\n", left->name);
        ft_printf(out, "call output: with value %v\n", left);
        output("1", left);

        exit(0);
        // exit(0);
        return (left);
    }
    else if (node->token->type == if_)
    {
        Val *left = eval(node->left);
        ft_printf(out, "in if, left: %v\n", left);
        if (left->type == boolean_)
        {
            if (left->boolean) // is true
            {
                return (eval(node->right));
            }
            else
            {
                // return var with value false
                return (left);
            }
        }
        else
        {
            // error should be boolean
            ft_printf(err, "if should be boolean\n");
        }
    }
    else if (node->token->type == add_ || node->token->type == sub_)
    {
        Val *left = eval(node->left);
        Val *right = eval(node->right);
        if (left->type != right->type && (left->type != integer_ && right->type != integer_))
            ft_printf(err, "can't '%t' different data type '%t' and '%t' \n", node->token->type, left->type, right->type);
        if (left->type == integer_ || left->type == float_)
        {
            node->token->value.type = (left->type == float_ || right->type == float_) ? float_ : integer_;
            double number = 0.0;
            if (node->token->type == add_)
                number = left->number + right->number;
            if (node->token->type == sub_)
                number = left->number - right->number;
            node->token->value.number = number;
            return &node->token->value;
        }
        else if (left->type == characters_ && node->token->type == add_)
        {
            ft_printf(out, "left: %v | right: %v\n", left, right);
            node->token->value.type = characters_;
            node->token->value.characters = calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            ft_strcpy(node->token->value.characters, left->characters);
            ft_strcpy(node->token->value.characters + ft_strlen(node->token->value.characters), right->characters);
            // left->characters = characters;
            ft_printf(out, "return: %v\n", node->token->value);
            return &node->token->value;
        }
        else
            ft_printf(err, "Error 1: in eval in '%t'\n", node->token->type);
    }
    else if (node->token->type == mul_ || node->token->type == div_)
    {
        Val *left = eval(node->left);
        Val *right = eval(node->right);
        if (left->type != right->type && (left->type != integer_ && right->type != integer_))
            ft_printf(err, "can't '%t' different data type '%t' and '%t' \n", node->token->type, left->type, right->type);
        if (left->type == integer_ || left->type == float_)
        {
            node->token->value.type = (left->type == float_ || right->type == float_) ? float_ : integer_;
            double number = 0.0;
            if (node->token->type == mul_)
                number = left->number * right->number;
            if (node->token->type == div_)
                number = left->number / right->number;
            node->token->value.number = number;
            return &node->token->value;
        }
        else
            ft_printf(err, "Error 2: in eval in '%t'\n", node->token->type);
    }
    else if (
        node->token->type == equal_ || node->token->type == not_equal_ ||
        node->token->type == less_than_ || node->token->type == more_than_ ||
        node->token->type == less_than_or_equal_ || node->token->type == more_than_or_equal_)
    {
        Val *left = eval(node->left);
        Val *right = eval(node->right);
        if (left->type != right->type && (left->type != integer_ && right->type != integer_))
            ft_printf(err, "can't '%t' different data type '%t' and '%t' \n", node->token->type, left->type, right->type);
        if (left->type == integer_ || left->type == float_)
        {
            node->token->value.type = boolean_;
            switch (node->token->type)
            {
            case equal_:
                node->token->value.boolean = left->number == right->number;
                break;
            case not_equal_:
                node->token->value.boolean = left->number != right->number;
                break;
            case more_than_:
                node->token->value.boolean = left->number > right->number;
                break;
            case less_than_:
                node->token->value.boolean = left->number < right->number;
                break;
            case more_than_or_equal_:
                node->token->value.boolean = left->number >= right->number;
                break;
            case less_than_or_equal_:
                node->token->value.boolean = left->number <= right->number;
                break;
            default:
                break;
            }
            return &node->token->value;
        }
        else
            ft_printf(err, "Error 3: in eval in '%t'\n", node->token->type);
    }

    ft_printf(err, "Error 0: in eval in '%t'\n", node->token->type);
    return (NULL);
}
// built in functions
void output(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int i = 0;
    int fd = out;
    while (fmt && fmt[i])
    {
        Val *var = va_arg(ap, Val *);
        if (var)
        {
            switch (var->type)
            {
            case identifier_:
                break;
            case characters_:
                ft_putstr(fd, var->characters);
                break;
            case integer_:
                ft_putnbr(fd, var->number);
                break;
            case float_:
                ft_putfloat(fd, var->number);
                break;
            case boolean_:
                if (var->boolean)
                    ft_putstr(fd, "true");
                else
                    ft_putstr(fd, "false");
                break;
            case array_:
                for (int i = 0; i < var->array_size; i++)
                {
                    switch (var->elems[0].type)
                    {
                    case characters_:
                        ft_putstr(fd, var->elems[i].characters);
                        break;
                    case integer_:
                        ft_putnbr(fd, var->elems[i].number);
                        break;
                    case float_:
                        ft_putfloat(fd, var->elems[i].number);
                        break;
                    case boolean_:
                        if (var->boolean)
                            ft_putstr(fd, "true");
                        else
                            ft_putstr(fd, "false");
                        break;
                    default:
                        break;
                    }
                    ft_putchar(fd, ' ');
                }
                break;
            default:
                ft_putstr(err, "error, append in output");
                exit(0);
            }
        }
        else
            ft_putstr(fd, "(null var)");
        i++;
    }
    ft_putchar(fd, '\n');
}

void execute()
{
    /*
    important, in assign:
        left should be always variable
        right should be a data type
    */
    Node *curr = NULL;
    curr = expr();
    while (curr && eval(curr))
        curr = expr();
}

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
    ft_printf(out, "%s\n", text);
    indexes();
    build_tokens();
    tk_pos = 0;
    execute();
    visualize_variables();
}