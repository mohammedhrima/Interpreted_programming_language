; // c headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>

// typedefs
typedef struct Node Node;
typedef struct Token Token;
typedef struct Value Value;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(int type);
void output(char *fmt, ...);
Value *get_var(char *name);

// MACROS
#define in STDIN_FILENO
#define out STDOUT_FILENO
#define err STDERR_FILENO

typedef enum
{
    none_,
    identifier_,
    characters_,
    character_,
    boolean_,
    integer_,
    float_,
    array_,
    assign_,
    equal_,
    not_equal_,
    more_than_,
    less_than_,
    more_than_or_equal_,
    less_than_or_equal_,
    function_,
    if_,
    while_,
    and_,
    or_,
    dots_,
    end_statement_,
    // tab_,
    new_line_,
    lparent_,
    rparent_,
    lbracket_,
    rbracket_,
    input_,
    output_,
    comma_,
    add_,
    sub_,
    mul_,
    div_
} Type;

// Value
struct Value
{
    char *name;
    Type type;
    int index;
    union
    {
        double number;    // type integer, float
        char *characters; // string , char
        bool boolean;
        // array
        struct
        {
            Value *elems;
            int array_size;
        };
        // type object
        struct
        {
            char **keys;
            Value *values;
            int object_size;
        };
    };
};
// Token
struct Token
{
    // int column;
    Value value;
    Type type;
};
// expect space
struct
{
    char *name;
    Type type;
} statements_tokens[] = {
    {"if ", if_},
    {"while ", while_},
    {"or ", or_},
    {"and ", and_},
    {"func ", function_},
    {"end", end_statement_},
    {":", dots_},
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
    // {"\n", new_line_},
    // {"   ", tab_},
    {0, 0},
};
// they expect (
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
char *text;
int txt_pos;
Token *tokens[500];
int tk_pos;
Value *VARIABLES[500];
int var_index;
int line;
int start;
int tab_level;

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
    return (c == ' ');
}
int ft_isnewline(int c)
{
    return (c == '\n');
}
int ft_istab(int c)
{
    return (c == '\t');
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
        ft_printf(err, "receive NULL in strncpy\n");
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
        ft_printf(err, "receive NULL in strncpy\n");
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
    else
        write(fd, str, ft_strlen(str));
    // while (str && str[i])
    //     ft_putchar(fd, str[i++]);
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
                    space = va_arg(ap, long);
                int num = va_arg(ap, long);
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
            if (fmt[i] == 'k')
            {
                Token *token = va_arg(ap, Token *);
                if (token)
                {
                    ft_putstr(fd, type_to_string(token->type));
                    ft_putstr(fd, " ");
                    switch (token->type)
                    {
                    case identifier_:
                        ft_putstr(fd, "with name: ");
                        ft_putstr(fd, token->value.name);
                        break;
                    case characters_:
                    case character_:
                        ft_putstr(fd, token->value.characters);
                        break;
                    case integer_:
                        ft_putnbr(fd, token->value.number);
                        break;
                    case float_:
                        ft_putfloat(fd, token->value.number);
                        break;
                    case boolean_:
                        if (token->value.boolean)
                            ft_putstr(fd, "true");
                        else
                            ft_putstr(fd, "false");
                        break;
                    case array_:
                        for (int i = 0; i < token->value.array_size; i++)
                        {
                            switch (token->value.elems[0].type)
                            {
                            case characters_:
                                ft_putstr(fd, token->value.elems[i].characters);
                                break;
                            case integer_:
                                ft_putnbr(fd, token->value.elems[i].number);
                                break;
                            case float_:
                                ft_putfloat(fd, token->value.elems[i].number);
                                break;
                            case boolean_:
                                if (token->value.boolean)
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
                    case assign_:
                    case equal_:
                    // case new_line_:
                    case if_:
                    case end_statement_:
                    case dots_:
                    // case tab_:
                    case more_than_:
                        break;
                    default:
                        ft_putstr(err, "ft_printf didn't know this token type '");
                        ft_putstr(err, type_to_string(token->value.type));
                        ft_putstr(err, "'\n");
                        exit(0);
                    }
                    // ft_putstr(fd, " in column ");
                    // ft_putnbr(fd, token->column);
                    // ft_putstr(fd, "'");
                    ft_putstr(fd, " in pos ");
                    ft_putnbr(fd, token->value.index);
                    ft_putstr(fd, "'");
                }
                else
                    ft_putstr(fd, "(null token)");
            }
            if (fmt[i] == 'v')
            {
                Value *var = va_arg(ap, Value *);
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
    ft_printf(out, "variables: \n");
    while (VARIABLES[i])
    {
        if (VARIABLES[i])
            ft_printf(out, "     %v\n", VARIABLES[i]);
        i++;
    }
    ft_printf(out, "\n");
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
        return "CHARACTER ";
    case integer_:
        return "INTEGER   ";
    case float_:
        return "FLOAT     ";
    case array_:
        return "ARRAY     ";
    case assign_:
        return "ASSIGNEMENT";
    case equal_:
        return "EQUAL      ";
    case not_equal_:
        return "NOT EQUAL  ";
    case less_than_:
        return "LESS THAN  ";
    case more_than_:
        return "MORE THAN  ";
    case less_than_or_equal_:
        return "LESS THAN OR EQUAL";
    case more_than_or_equal_:
        return "MORE THAN OR EQUAL";
    case function_:
        return "FUNCTION  ";
    case if_:
        return "IF        ";
    case or_:
        return "OR        ";
    case and_:
        return "AND       ";
    case while_:
        return "WHILE LOOP";
    case output_:
        return "OUTPUT    ";
    case input_:
        return "INPUT     ";
    case lparent_:
        return "LEFT PARENT  ";
    case rparent_:
        return "RIGHT PARENT ";
    case lbracket_:
        return "LEFT BRACKET ";
    case rbracket_:
        return "RIGHT BRACKET";
    case comma_:
        return "COMMA";
    case none_:
        return "NONE";
    case add_:
        return "ADDITION     ";
    case sub_:
        return "SUBSTRACTION ";
    case mul_:
        return "MULTIPLACTION";
    case div_:
        return "DIVISION     ";
    case boolean_:
        return "BOOLEAN      ";
    case end_statement_:
        return "END STATEMENT";
    case dots_:
        return "DOTS";
    // case tab_:
    //     return "TAB";
    // case new_line_:
    //     return "NEW LINE     ";
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
    // new->column = tab_level;
    switch (type)
    {
    case identifier_:
        new->value.type = type;
        new->value.name = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->value.name, text + start, txt_pos - start);
        // break;
    case characters_:
        new->value.type = type;
        new->value.characters = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->value.characters, text + start, txt_pos - start);
        // break;
    case integer_:
    case float_:
        new->value.type = type;
        double num = 0.0;
        double sign = 1;
        if (text[start] == '-' || text[start] == '+')
        {
            sign = -1;
            start++;
        }
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
        new->value.number = sign *num;
        // break;
    case assign_:
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
    case end_statement_:
    case dots_:
    // case tab_:
    // case new_line_:
    case array_:
        ft_printf(out, "new token %k\n", new);
        break;
    case none_:
        break;
    default:
        ft_printf(err, "verify given token '%t'\n", type);
    }
    tokens[tk_pos] = new;
    tk_pos++;
    tokens[tk_pos] = NULL;
    return new;
}
// tokenize
char *tab = "   ";
char *tab_space(int size)
{
    if (size)
    {
        char *res = calloc(size * ft_strlen(tab) + 1, sizeof(char));
        memset(res, ' ', size * ft_strlen(tab));
        return res;
    }
    return "";
}

Token *build_tokens(int level)
{
    Token *token;
    int curr_level = 0;
    while (text[txt_pos])
    {
        // skip new lines
        if (text[txt_pos] == '\0')
        {
            break;
        }
        if (text[txt_pos] == '\n')
        {
            txt_pos++;
            // return new_token(new_line_);
            continue;
        }
        if (ft_strncmp(text + txt_pos, tab, ft_strlen(tab)) == 0)
        {
            curr_level++;
            text += ft_strlen(tab);
            continue;
        }
        // if (curr_level < level)
        // {
        //     ft_printf(out, "found end statement in pos '%d' int level %d\n", tk_pos, curr_level);
        //     return new_token(end_statement_);
        // }
        int type = 0;
        for (int i = 0; statements_tokens[i].type; i++)
        {
            if (ft_strncmp(statements_tokens[i].name, text + txt_pos, ft_strlen(statements_tokens[i].name)) == 0)
            {
                type = statements_tokens[i].type;
                txt_pos += ft_strlen(statements_tokens[i].name);
                break;
            }
        }
        if (type)
        {
            token = new_token(type);
            if (type == dots_)
            {
                ft_printf(out, "found dots\n");
                return token;
            }
            if (type == if_)
            {
                ft_printf(out, "found if , search for dots in level '%d'\n", curr_level);
                while (token->type != dots_ && token->type != none_)
                {
                    token = build_tokens(curr_level);
                }
                // expect :
                if (token->type != dots_)
                    ft_printf(err, "Expected ':'\n");
                // expect \n
                if (text[txt_pos] != '\n')
                {
                    ft_printf(err, "Expected '\\n'\n");
                }
                else
                    txt_pos++;
                char *tab_to_skip = tab_space(curr_level + 1);
                while (ft_strncmp(text + txt_pos, tab_to_skip, curr_level + 1) == 0)
                {
                    txt_pos += (curr_level + 1) * ft_strlen(tab);
                    token = build_tokens(curr_level + 1);
                }
                new_token(end_statement_);
                // if (token->type != end_statement_)
                //     ft_printf(err, "Expected 'EOS'\n");
            }
            return build_tokens(0);
            // return new_token(end_statement_);
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
            return new_token(type);
            // continue;
        }
        if (ft_isspace(text[txt_pos]))
        {
            txt_pos++;
            continue;
        }
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
            return new_token(type);
            // continue;
        }
        if (ft_isalpha(text[txt_pos]))
        {
            start = txt_pos;
            while (ft_isalnum(text[txt_pos]))
                txt_pos++;
            return new_token(identifier_);
            // continue;
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
            return new_token(tmp_type);
            // continue;
        }
        if (text[txt_pos] == '"' || text[txt_pos] == '\'')
        {
            start = txt_pos++;
            while (text[txt_pos] && text[txt_pos] != text[start])
                txt_pos++;
            if (text[txt_pos] != text[start])
                ft_printf(err, "expected '%c'\n", text[start]);
            start++;
            txt_pos++;
            return new_token(characters_);
            // continue;
        }
        else
            ft_printf(err, "Unknown value s:'%s', c:'%c', d:'%d' \n", text + txt_pos, text[txt_pos], text[txt_pos]);
    }
    return new_token(none_);
}

// build nodes
Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    ft_printf(out, "new node '%k'\n", new->token);
    return (new);
}
#if 0
Value *new_array_node(int start, int end)
{
    Value *new = calloc((end - start) + 2, sizeof(Value));

    int i = 0;
    int j = 0;
    while (start + i < end)
    {
        if (tokens[start + i]->type == comma_)
            i++;
        if (tokens[start + i]->type == identifier_)
        {
            Value *to_find = get_var(tokens[start + i]->value.name);
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
#endif

Node *expr();
Node *assign();         // =
Node *conditions();     // if, while
Node *logic_operator(); // and, or
Node *relational();     // == !=
Node *add_sub();        // + -
Node *mul_div();        // * /
Node *prime();

void skip(Type type)
{
    if (tokens[tk_pos]->type != type)
        ft_printf(err, "error current type is '%t' and want to skip '%t'\n", tokens[tk_pos]->type, type);
    tk_pos++;
}

Node *expr()
{

    Node *left = assign();
    return left;
}

Node *assign()
{

    Node *left = conditions();
    if (tokens[tk_pos]->type == assign_)
    {
        // skip tabs
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        skip(tokens[tk_pos]->type);
        node->right = conditions();
        return (node);
    }
    return left;
}

Node *conditions()
{
    if (tokens[tk_pos]->type == if_)
    {
        // int tabs0 = tokens[tk_pos]->column + 1;
        Node *node = new_node(tokens[tk_pos]);
        // skip if_
        skip(tokens[tk_pos]->type);
        // get condition
        node->left = relational();
        // expect :
        skip(dots_);
        // get block
        // node->right = expr();

        return node;
    }
    return relational();
}

Node *relational()
{
    Node *left = prime();
    if (tokens[tk_pos]->type == equal_ || tokens[tk_pos]->type == not_equal_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        skip(tokens[tk_pos]->type);
        node->right = prime();
        return (node);
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
        tokens[tk_pos]->type == identifier_ ||
        tokens[tk_pos]->type == end_statement_)
    {
        left = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
    }
    return left;
}

// get or create variable
Value *get_var(char *name)
{
    for (int i = 0; i < var_index; i++)
    {
        if (ft_strcmp(VARIABLES[i]->name, name) == 0)
            return (VARIABLES[i]);
    }
    return NULL;
}
Value *new_variable(Value *var)
{
    ft_printf(out, "new variable\n");
    VARIABLES[var_index++] = var;
    return (var);
}
// evaluate
Value *eval(Node *node)
{
    if (node == NULL)
        return NULL;
    ft_printf(out, "eval received '%t'\n", node->token->type);
    switch (node->token->type)
    {
    // assignement
    case assign_:
    {
        Value *left = eval(node->left);
        char *name = left->name;
        Value *right = eval(node->right);
        memcpy(left, right, sizeof(Value));
        left->name = name;
        return left;
    }
    // identifier
    case identifier_:
    {
        Value *identifier = get_var(node->token->value.name);
        if (identifier)
            return (identifier);
        else
            return (new_variable(&node->token->value));
    }
    // value
    case integer_:
    case float_:
    case characters_:
    case array_:
        return (&node->token->value);
    // relational operator
    case equal_:
    case not_equal_:
    case less_than_:
    case more_than_:
    case less_than_or_equal_:
    case more_than_or_equal_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
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
    case if_:
    {
        Value *left = eval(node->left);
        if (left->type == boolean_)
        {
            ft_printf(out, "in if, left: %v\n", left);
            if (left->boolean == true) // is true
            {
                Node *node = expr();
                while (node->token->type != end_statement_)
                {
                    eval(node);
                    node = expr();
                }
                ft_printf(out, "exit if\n");
                // exit(0);
            }
            else
            {
                // ft_printf(out, "in else, left: %v\n", left);
                // while (tokens[tk_pos]->type != end_statement_)
                //     tk_pos++;
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
    }
    ft_printf(out, "evall finished\n");
    return NULL;
}
// execute
void execute()
{
    Node *curr = expr();
    while (curr)
    {
        eval(curr);
        curr = expr();
    }
}
int main(void)
{
    FILE *fp = NULL;
    long file_size = 0;

    // open file and read it and feed the text
    fp = fopen("file.hr", "r");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    text = calloc(file_size + 1, sizeof(char));
    fseek(fp, 0, SEEK_SET);
    fread(text, file_size, sizeof(char), fp);
    fclose(fp);

    // start compilling
    ft_printf(out, "%s\n", text);
    indexes();

    // build tokens
    Token *curr = build_tokens(0);
    while (1)
    {
        curr = build_tokens(0);
        if (curr && curr->type == none_)
            break;
        if (!curr)
            break;
    }
    ft_printf(out, "\n");
    // execute
    tk_pos = 0;
    execute();
    visualize_variables();
}