// c headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>
#include <string.h>

// macros
#define in STDIN_FILENO
#define out STDOUT_FILENO
#define err STDERR_FILENO

// typedefs
typedef struct Node Node;
typedef struct Token Token;
typedef struct Token Value;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(int type);
Token *eval(Node *node);

typedef enum
{
    skip_,
    eof_,
    none_,
    identifier_,
    characters_,
    character_,
    boolean_,
    integer_,
    float_,
    array_,
    obj_,
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
    new_line_,
    lparent_,
    rparent_,
    lbracket_,
    rbracket_,
    lcbracket_,
    rcbracket_,
    input_,
    output_,
    comma_,
    add_,
    sub_,
    mul_,
    div_,
    iteration_,
    length_,
    dot_,
} Type;

struct Token
{
    char *name;
    Type type;
    int index;
    union
    {
        long double number; // type integer, float
        char *characters;   // string , char
        bool boolean;
        // array
        struct
        {
            Token **array;
            int array_len;
            Node **array_head;
        };
        // type object
        struct
        {
            Token **object;
            int object_len;
            Node **object_head;
        };
        // output_
        struct
        {
            // Token **params;
            int params_len;
            Node **params_head;
        };
    };
};

struct
{
    char *name;
    Type type;
} statements_tokens[] = {
    {"if ", if_},
    {"while ", while_},

    {"func ", function_},
    {"end ", end_statement_},
    {0, 0},
};

struct
{
    char *name;
    Type type;
} operators_tokens[] = {
    {"&&", and_},
    {"||", or_},
    {"and ", and_},
    {"or ", or_},
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
    {"{", lcbracket_},
    {"}", rcbracket_},
    {",", comma_},
    {"+", add_},
    {"-", sub_},
    {"*", mul_},
    {"/", div_},
    {":", dots_},
    {0, 0},
};

struct
{
    char *special;
    char replace;
} special_characters[] =
    {
        {"\\n", '\n'},
        {"\\\"", '\"'},
        {"\\\'", '\''},
        {"\\\\", '\\'},
        {0, 0},
};

struct
{
    char *name;
    Type type;
} attributes_tokens[] = {
    {".len", length_},
    {".", dot_},
    {0, 0},
};

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
int tmp_pos;
Token *VARIABLES[500];
int var_index;
int line;
int start;

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

int ft_strcmp(char *s1, char *s2)
{
    size_t i;

    i = 0;
    while (s2[i] && s1[i] && (unsigned char)s1[i] == (unsigned char)s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
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
    if (float_part)
    {
        ft_putchar(fd, '.');
        ft_putnbr(fd, (long)round(float_part));
    }
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
            if (fmt[i] == 'k' || fmt[i] == 'v')
            {
                Token *token = va_arg(ap, Token *);
                if (token)
                {
                    ft_putstr(fd, type_to_string(token->type));
                    ft_putstr(fd, " with name: ");
                    ft_putstr(fd, token->name);
                    switch (token->type)
                    {
                    case identifier_:
                        break;
                    case characters_:
                    case character_:
                        ft_putstr(fd, " ,value: ");
                        ft_putstr(fd, token->characters);
                        break;
                    case integer_:
                        ft_putstr(fd, " ,value: ");
                        ft_putfloat(fd, token->number, 0);
                        break;
                    case float_:
                        ft_putstr(fd, " ,value: ");
                        ft_putfloat(fd, token->number, 6);
                        break;
                    case boolean_:
                        ft_putstr(fd, " ,value: ");
                        if (token->boolean)
                            ft_putstr(fd, "true");
                        else
                            ft_putstr(fd, "false");
                        break;
                    case array_:
                        ft_putstr(fd, " ,value: \n");
                        for (int i = 0; i < token->array_len; i++)
                        {
                            ft_printf(fd, "         %k\n", token->array[i]);
                        }
                        ft_putstr(fd, "     with size: ");
                        ft_putnbr(fd, token->array_len);
                        break;
                    case obj_:
                        ft_putstr(fd, " ,value: \n");
                        for (int i = 0; i < token->object_len; i++)
                        {
                            ft_printf(fd, "         %k\n", token->object[i]);
                        }
                        ft_putstr(fd, "     with size: ");
                        ft_putnbr(fd, token->object_len);
                        break;
                    case assign_:
                    case new_line_:
                    case if_:
                    case end_statement_:
                    case dots_:
                    case eof_:
                    case equal_:
                    case more_than_:
                    case less_than_:
                    case less_than_or_equal_:
                    case more_than_or_equal_:
                    case sub_:
                    case add_:
                    case lbracket_:
                    case rbracket_:
                    case lcbracket_:
                    case rcbracket_:
                    case lparent_:
                    case rparent_:
                    case comma_:
                    case mul_:
                    case length_:
                    case dot_:
                    case output_:
                        break;
                    default:
                        ft_putstr(err, "ft_printf didn't know this token type: ");
                        ft_putstr(err, type_to_string(token->type));
                        exit(1);
                    }
                    ft_putstr(fd, ", in position [");
                    ft_putnbr(fd, token->index);
                    ft_putstr(fd, "]");
                }
                else
                    ft_putstr(fd, "(null token)");
            }
            if (fmt[i] == 'f')
            {
                long double num = va_arg(ap, long double);
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
    if (fd == err)
        exit(1);
}

// built in functions
void output(Token *token)
{
    int fd = 0;
    if (token)
    {
        switch (token->type)
        {
        case identifier_:
            ft_printf(err, "Undeclared variable '%s'\n", token->name);
        case characters_:
        case character_:
        {
            char *string = token->characters;
            int i = 0;
            while (string[i])
            {
                for (int j = 0; special_characters[j].special; j++)
                {
                    if (ft_strncmp(&string[i], special_characters[j].special, ft_strlen(special_characters[j].special)) == 0)
                    {
                        ft_putchar(fd, special_characters[j].replace);
                        i += ft_strlen(special_characters[j].special);
                        break;
                    }
                }
                ft_putchar(fd, string[i]);
                if (string[i] == '\0')
                    break;
                i++;
            }
        }
        break;
        case integer_:
            ft_putfloat(fd, token->number, 0);
            break;
        case float_:
            ft_putfloat(fd, token->number, 6);
            break;
        case boolean_:
            if (token->boolean)
                ft_putstr(fd, "true");
            else
                ft_putstr(fd, "false");
            break;
        case array_:
            ft_putstr(fd, "[ ");
            for (int i = 0; i < token->array_len; i++)
            {
                output(token->array[i]);
                if (i < token->array_len - 1)
                    ft_putstr(fd, ", ");
            }
            ft_putstr(fd, " ]");
            break;
        case obj_:
            ft_putstr(fd, "{\n");
            for (int i = 0; i < token->object_len; i++)
            {
                ft_putstr(fd,"    ");
                ft_putstr(fd, token->object[i]->name);
                ft_putstr(fd, ": ");
                output(token->object[i]);
                if (i < token->object_len - 1)
                    ft_putstr(fd, ",\n");
            }
            ft_putstr(fd, "\n}");
            break;
        default:
            ft_putstr(err, "Error in ouput\n");
            exit(1);
        }
    }
    else
        ft_putstr(fd, "NULL");
}

// debuging functions
void indexes()
{
    int i = 0;
    ft_putstr(out, "\n");
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
    ft_printf(out, "\nvariables: \n");
    while (VARIABLES[i])
    {
        if (VARIABLES[i])
            ft_printf(out, "     %k\n", VARIABLES[i]);
        i++;
    }
    ft_printf(out, "\n");
}

// parsing functions
char *type_to_string(int type)
{
    struct
    {
        char *string;
        Type type;
    } Types[] = {
        {"EOF", eof_},
        {"NONE", none_},
        {"IDENTIFIER", identifier_},
        {"CHARACTERS", characters_},
        {"CHARACTER", character_},
        {"BOOLEAN", boolean_},
        {"INTEGER", integer_},
        {"FLOAT", float_},
        {"ARRAY", array_},
        {"OBJ", obj_},
        {"ASSIGNEMENT", assign_},
        {"EQUAL", equal_},
        {"NOT EQUAL", not_equal_},
        {"MORE THAN", more_than_},
        {"LESS THAN", less_than_},
        {"MORE THAN OR EQUAL", more_than_or_equal_},
        {"LESS THAN OR EQUAL", less_than_or_equal_},
        {"FUNCTION", function_},
        {"IF", if_},
        {"WHILE LOOP", while_},
        {"AND", and_},
        {"OR", or_},
        {"DOTS", dots_},
        {"END STATEMENT", end_statement_},
        {"NEW LINE  ", new_line_},
        {"LEFT PARENT", lparent_},
        {"RIGHT PARENT", rparent_},
        {"LEFT BRACKET", lbracket_},
        {"RIGHT BRACKET", rbracket_},
        {"LEFT CURLY BRACKET", lcbracket_},
        {"RIGHT CURLY BRACKET", rcbracket_},
        {"INPUT", input_},
        {"OUTPUT", output_},
        {"COMMA", comma_},
        {"ADDITION", add_},
        {"SUBSTRACTION", sub_},
        {"MULTIPLACTION", mul_},
        {"DIVISION", div_},
        {"ITERATION", iteration_},
        {"LENGTH", length_},
        {"DOT", dot_},
        {0, 0},
    };
    for (int i = 0; Types[i].string; i++)
    {
        if (Types[i].type == type)
            return (Types[i].string);
    }
    ft_putstr(err, "Error: type note found '");
    ft_putnbr(err, type);
    ft_putstr(err, "'\n");
    return NULL;
}

Token *new_token(Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->index = tk_pos;
    switch (type)
    {
    case identifier_:
    {
        new->name = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->name, text + start, txt_pos - start);
        ft_printf(out, "new token: %k\n", new);
        break;
    }
    case characters_:
    {
        new->characters = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->characters, text + start, txt_pos - start);
        ft_printf(out, "new token: %k\n", new);
        break;
    }
    case integer_:
    case float_:
    {
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
        new->number = num;
        ft_printf(out, "new token: %k\n", new);
        break;
    }
    case boolean_:
    {
        ft_printf(out, "new token: %k\n", new);
        break;
    }
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
    case rcbracket_:
    case lcbracket_:
    case output_:
    case if_:
    case comma_:
    case end_statement_:
    case dots_:
    case new_line_:
    case array_:
    case none_:
    case eof_:
    case iteration_:
    case length_:
    case dot_:
        ft_printf(out, "new token: %k\n", new);
        break;
    default:
        ft_printf(err, "verify given token '%t'\n", type);
    }
    tokens[tk_pos] = new;
    tk_pos++;
    tokens[tk_pos] = NULL;
    return new;
}

// Tokenize
Token *build_tokens()
{
    Token *token;
    int curr_level = 0;
    while (text[txt_pos])
    {
        if (text[txt_pos] == '\0')
            break;
        if (text[txt_pos] == '\n')
        {
            curr_level = 0;
            txt_pos++;
            continue;
        }
        if (text[txt_pos] == ' ')
        {
            txt_pos++;
            continue;
        }
        int type = 0;
        for (int i = 0; operators_tokens[i].type; i++)
        {
            if (ft_strncmp(operators_tokens[i].name, text + txt_pos, ft_strlen(operators_tokens[i].name)) == 0)
            {
                type = operators_tokens[i].type;
                txt_pos += ft_strlen(operators_tokens[i].name);
                break;
            }
        }
        if (type)
        {
            new_token(type);
            continue;
        }
        type = 0;
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
            new_token(type);
            continue;
        }
        type = 0;
        for (int i = 0; attributes_tokens[i].type; i++)
        {
            if (ft_strncmp(attributes_tokens[i].name, text + txt_pos, ft_strlen(attributes_tokens[i].name)) == 0)
            {
                type = attributes_tokens[i].type;
                txt_pos += ft_strlen(attributes_tokens[i].name);
                break;
            }
        }
        if (type)
        {
            new_token(type);
            continue;
        }
        type = 0;
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
        int isboolean = ft_strcmp(text + txt_pos, "true");
        if (isboolean == 0 || isboolean == ' ' || isboolean == '\n')
        {
            token = new_token(boolean_);
            token->boolean = true;
            txt_pos += ft_strlen("true");
            continue;
        }
        isboolean = ft_strcmp(text + txt_pos, "false");
        if (isboolean == 0 || isboolean == ' ' || isboolean == '\n')
        {
            token = new_token(boolean_);
            token->boolean = false;
            txt_pos += ft_strlen("false");
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
            start = txt_pos;
            // expect number
            Type tmp_type = integer_;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
            {
                tmp_type = float_;
                txt_pos++;
            }
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            new_token(tmp_type);
            continue;
        }
        if (text[txt_pos] == '"' || text[txt_pos] == '\'')
        {
            start = txt_pos;
            txt_pos++;
            while (text[txt_pos] && text[txt_pos] != text[start])
                txt_pos++;
            if (text[txt_pos] != text[start])
                ft_printf(err, "expected '%c'\n", text[start]);
            start++;
            new_token(characters_);
            txt_pos++;
            continue;
        }
        ft_printf(err, "Unknown value s:'%s', c:'%c', d:'%d' \n", text + txt_pos, text[txt_pos], text[txt_pos]);
    }
    return (new_token(eof_));
    // return NULL;
}

// build nodes
Node *expr();       //
Node *assign();     // =
Node *equality();   // ==  !=
Node *comparison(); // < > <= >=
Node *add_sub();    // + -
Node *mul_div();    // * /
Node *unary();      // sign + -
Node *iteration();  // value[]
Node *attribute();  // .len ...
Node *prime();      // final value

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Token));
    new->token = token;
    ft_printf(out, "new node: %k\n", new->token);
    return (new);
}

void skip(Type type)
{
    if (tokens[tk_pos]->type != type)
        ft_printf(err, "error current type is '%t' and want to skip '%t in pos '%d''\n", tokens[tk_pos]->type, type, tk_pos);
    tk_pos++;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    // ft_printf(out, "call assign %t\n", tokens[tk_pos]->type);
    Node *left = equality();
    if (tokens[tk_pos]->type == assign_)
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = left;
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
        skip(tokens[tk_pos]->type);
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
        skip(tokens[tk_pos]->type);
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
        skip(tokens[tk_pos]->type);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = unary();
    while (tokens[tk_pos]->type == mul_ || tokens[tk_pos]->type == div_)
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = left;
        node->right = unary();
        left = node;
    }
    return left;
}

Node *unary()
{
    if (tokens[tk_pos]->type == sub_)
    {
        // minus number token
        Token *minus = calloc(1, sizeof(Token));
        minus->type = integer_;
        minus->number = -1;
        // mul token
        tokens[tk_pos]->type = mul_;
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = new_node(minus);
        node->right = attribute();
        return node;
    }
    return attribute();
}

Node *attribute()
{
    Node *left = iteration();
    // for object
    while (tokens[tk_pos]->type == dot_)
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = left;
        node->right = iteration();
        if (node->right->token->type != identifier_ && node->right->token->type != length_)
            ft_printf(err, "Expected identifier got '%t'", node->right->token->type);
        left = node;
        // return node;
    }
    if (tokens[tk_pos]->type == length_)
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = left;
        node->right = NULL;
        // node->right = prime();
        return node;
    }
    return left;
}

Node *iteration()
{
    Node *left = prime();
    if (tokens[tk_pos]->type == lbracket_)
    {
        // ft_printf(out, "do iteration\n");
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->token->type = iteration_;
        node->left = left;
        node->right = prime();
        skip(rbracket_);
        return node;
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
        tokens[tk_pos]->type == boolean_ ||
        tokens[tk_pos]->type == identifier_ ||
        tokens[tk_pos]->type == end_statement_)
    {
        left = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
    }
    else if (tokens[tk_pos]->type == lbracket_)
    {
        // method 2
        left = new_node(tokens[tk_pos]);
        skip(lbracket_);
        left->token->type = array_;
        int len = 0;
        Node **list = calloc(len + 1, sizeof(Node *));
        if (tokens[tk_pos]->type != rbracket_) // enter if array is not empty
        {
            list[len] = assign();
            len++;
            list = realloc(list, (len + 1) * sizeof(Node *));
            while (tokens[tk_pos]->type != rbracket_ && tokens[tk_pos]->type != eof_)
            {
                skip(comma_);
                list[len] = assign();
                len++;
                list = realloc(list, (len + 1) * sizeof(Node *));
            }
        }
        skip(rbracket_);
        left->token->array_len = len;
        left->token->array_head = list;
        // ft_printf(out, "len is %d\n", len);
        return left;
    }
    else if (tokens[tk_pos]->type == lcbracket_)
    {
        // method 2
        left = new_node(tokens[tk_pos]);
        skip(lcbracket_);
        left->token->type = obj_;
        int len = 0;
        Node **list = calloc(len + 1, sizeof(Node *));
        if (tokens[tk_pos]->type != rcbracket_) // enter if object is not empty
        {
            Token *identifier = tokens[tk_pos]; // expect identifier
            skip(identifier_);
            char *name = identifier->name;
            skip(dots_); // expect dots
            list[len] = add_sub();
            list[len]->token->name = name;
            len++;
            list = realloc(list, (len + 1) * sizeof(Node *));
            while (tokens[tk_pos]->type != rcbracket_ && tokens[tk_pos]->type != eof_)
            {
                skip(comma_);
                identifier = tokens[tk_pos]; // expect identifier
                skip(identifier_);
                name = identifier->name;
                skip(dots_); // expect dots
                list[len] = add_sub();
                list[len]->token->name = name;
                len++;
                list = realloc(list, (len + 1) * sizeof(Node *));
            }
        }
        skip(rcbracket_);
        left->token->object_len = len;
        left->token->object_head = list;
        // ft_printf(out, "len is %d\n", len);
        return left;
    }
    else if (tokens[tk_pos]->type == output_)
    {
        left = new_node(tokens[tk_pos]);
        skip(output_);
        skip(lparent_);
        int len = 0;
        Node **list = calloc(len + 1, sizeof(Node *));
        if (tokens[tk_pos]->type != rparent_) // enter if array is not empty
        {
            list[len] = equality();
            len++;
            list = realloc(list, (len + 1) * sizeof(Node *));
            while (tokens[tk_pos]->type != rparent_ && tokens[tk_pos]->type != eof_)
            {
                skip(comma_);
                list[len] = equality();
                len++;
                list = realloc(list, (len + 1) * sizeof(Node *));
            }
        }
        skip(rparent_);
        left->token->params_head = list;
        left->token->params_len = len;
        // ft_printf(out, "len is %d\n", len);
        return left;
    }
    return left;
}

// evaluate
Token *get_var(char *name)
{
    for (int i = 0; i < var_index; i++)
    {
        if (ft_strcmp(VARIABLES[i]->name, name) == 0)
            return (VARIABLES[i]);
    }
    return NULL;
}

Token *new_variable(Token *var)
{
    VARIABLES[var_index++] = var;
    return (var);
}

Value *eval(Node *node)
{
    if (node == NULL)
        return NULL;
    // ft_printf(out, "eval received %t\n", node->token->type);
    switch (node->token->type)
    {
    // assignement
    case assign_:
    {
#if 0
        // to be check after
        switch (node->right->token->type) // right is the holder
        {
        case identifier_:
        case characters_:
        case character_:
        case boolean_:
        case integer_:
        case float_:
        case array_:
        case obj_:
            break;
        default:
            ft_printf(err, "can't assign to %t\n", node->right->token->type);
        }
        switch (node->left->token->type) // left is the value
        {
        case identifier_:
        case characters_:
        case character_:
        case boolean_:
        case integer_:
        case float_:
        case array_:
        case obj_:
            break;
        default:
            ft_printf(err, "%t can't be assign\n", node->left->token->type);
        }
#endif
        Value *left = eval(node->left);
        Value *right = eval(node->right);

        if (right->type == identifier_)
            ft_printf(err, "Error , Undclared varibale: %s\n", right->name);

        if (left->type != identifier_ && left->type != right->type)
        {
            if (left->type == character_ || left->type == characters_)
                ;
            else
                ft_printf(err, "can't assign '%s' type %t to '%s' type %t \n", left->name, left->type, right->name, right->type);
        }

        char *name = left->name;
        int index = left->index;

        if (right->type == character_)
        {
            if (left->type == identifier_)
            {
                left->type = characters_;
                left->characters = calloc(2, sizeof(char));
                left->characters[0] = right->characters[0];
            }
            else if (left->type == character_)
            {
                left->characters[0] = right->characters[0];
            }
            else if (left->type == characters_)
            {
                // ft_printf(err, "Error can't set string to one character");
                //  free(left->characters);
                left->characters = calloc(2, sizeof(char));
                left->characters[0] = right->characters[0];
            }
        }
        else if (right->type == characters_)
        {
            if (left->type == identifier_)
            {
                left->type = characters_;
                left->characters = calloc(ft_strlen(right->characters) + 1, sizeof(char));
                memcpy(left->characters, right->characters, ft_strlen(right->characters));
            }
            else if (left->type == characters_)
            {
                // protect it from leaks after
                left->characters = calloc(ft_strlen(right->characters) + 1, sizeof(char));
                memcpy(left->characters, right->characters, ft_strlen(right->characters));
            }
            else if (left->type == character_)
            {
                if (ft_strlen(right->characters) == 1)
                    left->characters[0] = right->characters[0];
                else
                    ft_printf(err, "Error: can't assign string to character\n");
            }
        }
        else
            memcpy(left, right, sizeof(Value)); // to be changed after, change only for chracters, I guess !!!
        left->name = name;
        left->index = index;
        return left;
    }
    // identifier
    case identifier_:
    {
        Value *identifier = get_var(node->token->name);
        if (identifier)
            return (identifier);
        else
            return (new_variable(node->token));
    }
    // value
    case integer_:
    case float_:
    case characters_:
    case character_:
    case boolean_:
    {
        // ft_printf(out, "return %k\n", node->token);
        return (node->token);
    }
    case iteration_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variabale '%s'\n", left->name);
        if (left->type != array_ && left->type != characters_)
            ft_printf(err, "can't iterate over type; %t\n", left->type);
        if (right->type != integer_)
            ft_printf(err, "expected integer to iterate with it\n");
        // iterate
        long i = (long)right->number;
        // protect it from segfault
        if (left->type == array_)
        {
            ft_printf(out, "return %v\n", left->array[i]);
            // exit(0);
            return left->array[i];
        }
        else if (left->type == characters_)
        {
            node->token->type = character_;
            node->token->characters = left->characters + i;
            return (node->token);
        }
        else
            ft_printf(err, "Error in iteration\n");
        // exit(0);
    }
    // method 2
    case array_:
    {
        // exit(0);
        Node **head = node->token->array_head;
        node->token->array = calloc(node->token->array_len + 1, sizeof(Node *));
        int i = 0;
        while (i < node->token->array_len)
        {
            node->token->array[i] = eval(head[i]);
            i++;
        }
        // ft_printf(out, "array -> %k\n", node->token);
        return (node->token);
    }
    case obj_:
    {
        Node **head = node->token->object_head;
        node->token->object = calloc(node->token->object_len + 1, sizeof(Node *));
        int i = 0;
        while (i < node->token->object_len)
        {
            node->token->object[i] = eval(head[i]);
            i++;
        }
        // ft_printf(out, "array -> %k\n", node->token);
        return (node->token);
    }
    case output_:
    {
        // exit(0);
        Node **head = node->token->params_head;
        // node->token->array = calloc(node->token->params_len + 1, sizeof(Node *));
        int i = 0;
        // ft_printf(out, "execute output, it has %d params\n", node->token->params_len);
        while (i < node->token->params_len)
        {
            // ft_printf(out, "%v ", eval(head[i]));
            output(eval(head[i]));
            ft_putstr(out, " ");
            // exit(0);
            i++;
        }
        ft_printf(out, "\n");
        // exit(0);
        // ft_printf(out, "array -> %k\n", node->token);
        return (node->token);
    }
    // operator
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        // ft_printf(out, "do: %t between\n        left: %v\n        right: %v\n", node->token->type, left, right);
        if (
            (left->type == integer_ && right->type == integer_) ||
            (left->type == float_ && right->type == float_) ||
            (left->type == integer_ && right->type == float_) ||
            (left->type == float_ && right->type == integer_))
        {
            long double number = 0.0;
            if (node->token->type == add_)
                number = left->number + right->number;
            if (node->token->type == sub_)
                number = left->number - right->number;
            if (node->token->type == mul_)
                number = left->number * right->number;
            if (node->token->type == div_)
                number = left->number / right->number;
            node->token->type = (left->type == float_ || right->type == float_) ? float_ : integer_;
            node->token->number = number;
        }
        else if (
            node->token->type == add_ &&
            ((left->type == characters_ && right->type == characters_) ||
             (left->type == character_ && right->type == characters_) ||
             (left->type == characters_ && right->type == character_) ||
             (left->type == character_ && right->type == character_)))
        {
            node->token->type = (left->type == characters_ || right->type == characters_) ? characters_ : character_;
            node->token->characters = calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            ft_strcpy(node->token->characters, left->characters);
            ft_strcpy(node->token->characters + ft_strlen(node->token->characters), right->characters);
            // ft_printf(out, "result: %k\n", node->token);
        }
        else
        {
            ft_printf(err, "Error 1: in eval in '%t'\n       left: '%v'\n       right: '%v'\n", node->token->type, left, right);
        }
        ft_printf(out, "return: %k\n", node->token);
        return (node->token);
    }
    case equal_:
    case not_equal_:
    case more_than_:
    case less_than_:
    case more_than_or_equal_:
    case less_than_or_equal_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        // ft_printf(out, "do: %t between\n        left: %v\n        right: %v\n", node->token->type, left, right);
        if (
            (left->type == integer_ && right->type == integer_) ||
            (left->type == float_ && right->type == float_) ||
            (left->type == integer_ && right->type == float_) ||
            (left->type == float_ && right->type == integer_))
        {
            switch (node->token->type)
            {
            case equal_:
                node->token->boolean = left->number == right->number;
                break;
            case not_equal_:
                node->token->boolean = left->number != right->number;
                break;
            case more_than_:
                node->token->boolean = left->number > right->number;
                break;
            case less_than_:
                node->token->boolean = left->number < right->number;
                break;
            case more_than_or_equal_:
                node->token->boolean = left->number >= right->number;
                break;
            case less_than_or_equal_:
                node->token->boolean = left->number <= right->number;
                break;
            default:
                break;
            }
            node->token->type = boolean_;
            return (node->token);
        }
        else if (
            node->token->type == equal_ &&
            ((left->type == characters_ && right->type == characters_) ||
             (left->type == character_ && right->type == characters_) ||
             (left->type == characters_ && right->type == character_) ||
             (left->type == character_ && right->type == character_)))
        {
            node->token->type = boolean_;
            node->token->boolean = ft_strcmp(left->characters, right->characters) == 0;
            return (node->token);
        }
        else
        {
            ft_printf(err, "Error 1: in eval in '%t'\n       left: '%v'\n       right: '%v'\n", node->token->type, left, right);
        }
    }
    case length_:
    {
        // ft_printf(out, "enter length\n");
        Value *left = eval(node->left);
        node->token->type = integer_;
        if (left->type == character_ || left->type == characters_)
        {
            // ft_printf(out, "calculate len of %v\n", left);
            node->token->number = (double)ft_strlen(left->characters);
            // ft_printf(out, "len is %f\n", node->token->number);
            return node->token;
        }
        else if (left->type == array_)
        {
            node->token->number = left->array_len;
            return node->token;
        }
        else
            ft_printf(err, "data type '%t' has no attribute '%t'\n", left->type, length_);
        // exit(0);
    }
    case dot_:
    {
        // ft_printf(out, "enter dot\n");
        Value *left = eval(node->left);
        char *name = node->right->token->name;
        if (left->type == identifier_)
        {
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        }
        // check that left is obj, if you check that right is idenitifier taht may cause error
        // in case there is a variable with same name as this attribute
        ft_printf(out, "search for %s\n", name);
        if (left->type == obj_)
        {
            int i = 0;
            while (i < left->object_len)
            {
                if (strcmp(left->object[i]->name, name) == 0)
                {
                    // ft_printf(out, "\nreturn %v\n", left->object[i]);
                    return left->object[i];
                }
                i++;
            }
        }
        if (left->type == obj_)
        {
            if (left->name)
                ft_printf(err, "Error: '%s' has no attribute '%s'\n", left->name, name);
            else
                ft_printf(err, "Error: OBJ has no attribute '%s'\n", name);
        }
        else
            ft_printf(err, "Error: %t has no attribute '%s'\n", left->type, name);
    }
    default:
        ft_printf(err, "receive unkonwn type '%t' \n", node->token->type);
    }
    return NULL;
}

// execute
void execute()
{
    while (tokens[tk_pos]->type != eof_)
    {
        eval(expr());
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
    build_tokens();
    tmp_pos = tk_pos;
    tk_pos = 0;
    execute();
    visualize_variables();
}