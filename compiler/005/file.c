// c headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>

// typedefs
typedef struct Node Node;
typedef struct Token Token;
typedef struct Val Val;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(int type);

#define in STDIN_FILENO
#define out STDOUT_FILENO
#define err STDERR_FILENO

// tokens
typedef enum
{
    none_, // skip 0
    // be carefulll before doing any change here !!!
    variable_,
    characters_,
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
    {":", end_statement},
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
    {",", comma_},
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
                    case variable_:
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
    ft_printf(out, "temporary variables: \n");
    while (TEMPORARIES[i])
    {
        Val *temporary = TEMPORARIES[i];
        if (temporary)
            ft_printf(out, "     %v\n", temporary);
        i++;
    }
}
// parsing functions
char *type_to_string(int type)
{
    switch (type)
    {
    // case eof_:
    //     return "EOF";
    case variable_:
        return "VARIABLE";
    case characters_:
        return "CHARACTERS";
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
        return "ANC";
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
    case variable_:
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
    case if_:
    case end_statement:
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
    Token *curr_token;
    // int start = 0;
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
            new_token(variable_);
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
    ft_printf(out, "new node with type '%t'\n", new->token->type);
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
        tk_pos++;
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
        tokens[tk_pos]->type == variable_ ||
        tokens[tk_pos]->type == comma_)
    {
        left = new_node(tokens[tk_pos]);
        // if (left->token->type == integer_ || left->token->type == float_)
        //     ft_printf(out, "tokens value is : '%f' \n", left->token->value.number);
        tk_pos++;
    }
    else if (
        tokens[tk_pos]->type == lparent_)
    {
        tk_pos++; // skip 1st parent
        left = expr();
        // expect rparent_
        tk_pos++; // skip est parent
    }
    return left;
}

// execute and create variables

// Val *new_temporary_var(Type var_type) // to see after if it's necessary
// {
//     Val *new = calloc(1, sizeof(Val));
//     new->type = var_type;
//     ft_printf(out, "new temporary var with type '%t'\n", var_type);
//     TEMPORARIES[tmp_index++] = new;
//     return (new);
// }

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
    ft_printf(out, "variable '%s' not found\n", name);
    return NULL;
}

Val *new_permanent_var(Val *left, Val *value)
{
    value->name = left->name;
    ft_printf(out, "new permanent will return %v\n", value);
    PERMANENTS[per_index++] = value;
    return (value);
}

Val *eval(Node *node)
{
    Val *res = NULL;
    ft_printf(out, "eval received '%t'\n", node->token->type);
    if (node->token->type == assign_)
    {
        Val *exist = get_var(node->left->token->value.name);
        if (exist)
        {
            char *name = exist->name;
            *exist = *eval(node->right);
            exist->name = name;
            return exist;
        }
        else
            return (new_permanent_var(&node->left->token->value, eval(node->right)));
    }
    else if (node->token->type == variable_)
    {
        return (get_var(node->token->value.name));
    }
    else if (node->token->type == if_)
    {
        Val *left = eval(node->left);
        if (left->type == boolean_)
        {
            if (left->boolean) // is true
            {
                return (eval(node->right));
            }
            else
            {
                return (NULL); // to be handled after
                // is false
            }
        }
        else
        {
            // error should be boolean
            ft_printf(err, "if should be boolean\n");
        }
    }
    else if (node->token->type == integer_ || node->token->type == float_ || node->token->type == characters_)
    {
        ft_printf(out, "return '%v'\n", node->token->value);
        return &node->token->value;
    }
    else if (node->token->type == add_ || node->token->type == sub_)
    {
        Val *left = eval(node->left);
        Val *right = eval(node->right);
        if (left->type != right->type && (left->type != integer_ && right->type != integer_))
            ft_printf(err, "can't '%t' different data type '%t' and '%t' \n", node->token->type, left->type, right->type);
        if (left->type == integer_ || left->type == float_)
        {
            left->type = (left->type == float_ || right->type == float_) ? float_ : integer_;
            double number = 0.0;
            if (node->token->type == add_)
                number = left->number + right->number;
            if (node->token->type == sub_)
                number = left->number - right->number;
            left->number = number;
            return left;
        }
        else if (left->type == characters_ && node->token->type == add_)
        {
            char *characters = calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            ft_strcpy(characters, left->characters);
            ft_strcpy(characters + ft_strlen(characters), right->characters);
            left->characters = characters;
            return left;
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
            left->type = (left->type == float_ || right->type == float_) ? float_ : integer_;
            double number = 0.0;
            if (node->token->type == mul_)
                number = left->number * right->number;
            if (node->token->type == div_)
                number = left->number / right->number;
            left->number = number;
            return left;
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
    else
        ft_printf(err, "Error 0: in eval in '%t'\n", node->token->type);
    return (NULL);
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
    ;
    while (curr && eval(curr))
    {
        // eval(curr);
        curr = expr();
        // eval(curr);
    }
    // while ((curr = expr()))
    // {
    //     // curr = expr();
    //     if (curr->token->type == assign_ )
    //     {
    //         // check types before assiging
    //         ft_printf(out, "do '%t', between '%t' and '%t'\n", curr->token->type, curr->left->token->type, curr->right->token->type);
    //         new_permanent_var(&curr->left->token->value, eval(curr->right));
    //     }
    //     else if(curr->token->type == if_)
    //     {
    //         ft_printf(out, "found if\n");
    //         eval(curr);
    //         // exit(0);
    //     }
    //     // else
    //     // {
    //     //     eval(curr);
    //     //   //  ft_printf(err, "Unknown token type in execute '%t'\n", curr->token->type);
    //     // }
    // }
}

int main(void)
{
    /*
        x = [1, 2 + 3,3,5 ,8]
        x[0] = 7
        print(x)
        x = 554
        y = x + 2
    */
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