// c headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>

// typedefs
// typedef struct Var Var;
typedef struct Node Node;
typedef struct Token Token;
typedef struct Var Var;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(int type);
// Var *eval(Node *node);

#define in STDIN_FILENO
#define out STDOUT_FILENO
#define err STDERR_FILENO

// tokens

typedef enum
{
    none_, // skip 0
    // be carefulll before doing any change here !!!
    // eof_,
    variable_,
    characters_,
    boolean_,
    // number_,
    integer_,
    float_,
    assign_,
    equal_,
    less_than_,
    more_than_,
    function_,
    if_,
    while_,
    and_,
    or_,
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

// Var
struct Var
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
            Var *elems;
            int array_size;
        };
        // type object
        struct
        {
            char **keys;
            Var *values;
            int object_size;
        };
    };
};
// Token
struct Token
{
    Var value;
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
    {"while", while_},
    {0, 0},
};
// they expect nothing
struct
{
    char *name;
    Type type;
} operator_tokens[] = {
    {"==", equal_},
    {"<=", less_than_},
    {">=", more_than_},
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
Var *PERMANENTS[500];
Var *TEMPORARIES[500];
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
    return (c == ' ' || c == '\t' || c == '\n');
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
            if (fmt[i] == 'v')
            {
                Var *var = va_arg(ap, Var *);
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
                        ft_putstr(err, "ft_printf didn't know variable type\n");
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
        Var *variable = PERMANENTS[i];
        if (variable)
            ft_printf(out, "     %v\n", variable);
        i++;
    }
    i = 0;
    ft_printf(out, "temporary variables: \n");
    while (TEMPORARIES[i])
    {
        Var *temporary = TEMPORARIES[i];
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
        return "VARIABALE";
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
    case less_than_:
        return "LESS THAN";
    case more_than_:
        return "MORE THAN";
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
    default:
        ft_printf(err, "type not found in type_to_string '%d'\n", type);
    }
    return NULL;
}
// news functions;
Token *new_token(Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->value.index = tk_pos;
    if (type == variable_)
    {
        new->value.name = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->value.name, text + start, txt_pos - start);
        ft_printf(out, "new token type: '%s' with name '%s'\n", type_to_string(type), new->value.name);
    }
    else if (type == characters_)
    {
        new->value.type = type;
        new->value.characters = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->value.characters, text + start, txt_pos - start);
        ft_printf(out, "new token type: '%s' with value characters '%s'\n", type_to_string(type), new->value.characters);
    }
    else if (type == integer_ || type == float_)
    {
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
        ft_printf(out, "new token type: '%s' with value number '%f'\n", type_to_string(type), new->value.number);
    }
    else
        ft_printf(out, "new token type: '%s'\n", type_to_string(type));
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
    ft_printf(out, "new node with type '%s'\n", type_to_string(new->token->type));
    return (new);
}

Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = add_sub();
    if (tokens[tk_pos]->type == assign_)
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
        if (left->token->type == integer_ || left->token->type == float_)
            ft_printf(out, "tokens value is : '%f' \n", left->token->value.number);
        tk_pos++;
    }
    return left;
}

// execute and create variables
Var *new_var(Type var_type, bool permanent)
{
    Var *new = calloc(1, sizeof(Var));
    new->type = var_type;
    ft_printf(out, "new var with type '%s'\n", type_to_string(var_type));
    if (permanent)
    {
        PERMANENTS[per_index] = new;
        per_index++;
    }
    else
    {
        TEMPORARIES[tmp_index] = new;
        tmp_index++;
    }
    return (new);
}

Var *add_var(Var *var, bool permanent)
{
    if (permanent)
    {
        PERMANENTS[per_index] = var;
        per_index++;
    }
    else
    {
        TEMPORARIES[tmp_index] = var;
        tmp_index++;
    }
    return var;
}

Var *operation(Var *left, Var *right, Type op)
{
    Var *res = new_var(left->type, false);
    // check data type before all operations
    if (op == add_)
        res->number = left->number + right->number;
    if (op == sub_)
        res->number = left->number - right->number;
    if (op == mul_)
        res->number = left->number * right->number;
    if (op == div_)
        res->number = left->number / right->number;
    return res;
}

Var *eval(Node *node)
{
    if (node->token->type == integer_ || node->token->type == float_ || node->token->type == characters_)
        return &node->token->value;
    if (node->token->type == add_ || node->token->type == sub_ || node->token->type == mul_ || node->token->type == div_)
        return (operation(eval(node->left), eval(node->right), node->token->type));
    return (0);
}

void execute()
{
    Node *curr = expr();
    Var *res = eval(curr);
    ft_printf(out, "res : %v\n", res);
    /*
    important, in assign:
        left should be always variable
        right should be a data type
    */
    // if (curr->type == assign_)
    // {
    //     // ft_printf(out, "do '%s', between '%s' and '%s'\n", type_to_string(curr->type), curr->left->content, type_to_string(curr->right->type));
    //     // new_var(curr->left->content, curr->right->type, curr->right->content);
    // }
    // else
    // {
    //     ft_printf(err, "error chi l3eyba\n");
    // }
    // Var *res = eval(curr);
}

int main(void)
{
    /*
        x = [1, 2 + 3,3,5 ,8]
        x[0] = 7
        print(x)
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