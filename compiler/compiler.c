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
#define DEBUG true

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
    error_,
    eof_,
    identifier_,
    break_,
    continue_,
    return_,
    characters_,
    boolean_,
    number_,
    array_,
    tuple_,
    obj_,
    assign_,
    equal_,
    not_equal_,
    not_,
    more_than_,
    less_than_,
    more_than_or_equal_,
    less_than_or_equal_,
    if_,
    elif_,
    else_,
    while_,
    and_,
    or_,
    dots_,
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
    add_assign_,
    sub_assign_,
    mul_assign_,
    div_assign_,
    iteration_,
    dot_,
    len_,
    func_dec,
    func_call,
    // bloc_,
} Type;

struct Token
{
    char *name;
    Type type;
    int level;
    int line;
    union
    {
        // used when building Token
        struct
        {
            long double number; // integer, float
            bool is_float;
        };
        struct
        {
            char *characters; // string , char
            bool is_char;
        };
        bool boolean;
        // array
        struct
        {
            Node **array_head;
            Token **array;
            int array_len;
        };
        // object
        struct
        {
            Node **object_head;
            char **keys;
            Token **object;
            int object_len;
        };
        // block
        struct
        {
            Token **PREV_VARIABLES;
            Token *VARIABLES[100];
            int var_index;
            Node **bloc_head;
        };
    };
};
// alpha tokens
// will be gotten ass identifiers
// then I compare them to the values here to check if it's one of them
// else it will be stored as identifier
struct
{
    char *name;
    Type type;
} alpha_tokens[] = {
    {"if", if_},
    {"elif", elif_},
    {"else", else_},
    {"while", while_},
    {"func", func_dec},
    {"output", output_},
    {"input", input_},
    {"true", boolean_},
    {"false", boolean_},
    {"and", and_},
    {"or", or_},
    {"is", equal_},
    {"not", not_},
    {"len", len_},
    {"break", break_},
    {"continue", continue_},
    {"return", return_},
    {0, 0},
};

struct
{
    char *name;
    Type type;
} operators_tokens[] = {
    {"&&", and_},
    {"||", or_},
    {"==", equal_},
    {"!=", not_equal_},
    {"!", not_},
    {"+=", add_assign_},
    {"-=", sub_assign_},
    {"*=", mul_assign_},
    {"/=", div_assign_},
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
    {".", dot_},
    {0, 0},
};

struct
{
    char *special;
    char replace;
} special_characters[] = {
    {"\\n", '\n'},
    {"\\\"", '\"'},
    {"\\\'", '\''},
    {"\\\\", '\\'},
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
int tmp_tk;
Token *VARIABLES[500];
Token *FUNCTIONS[500];
int var_index;
int func_index;
int line = 1;
int level;
int start;

Token **CURR_VARIABLES = VARIABLES;
int *curr_var_index = &var_index;

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
bool ft_putfloat(int fd, double num, int decimal_places)
{
    bool is_float = true;
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
    else
        is_float = false;
    return is_float;
}

void ft_printf(int fd, char *fmt, ...)
{
    if (DEBUG || fd == err)
    {
        va_list ap;
        va_start(ap, fmt);
        int i = 0;
        while (fmt && fmt[i])
        {
            if (fmt[i] == '%')
            {
                i++;
                if (fmt[i] == 'd')
                {
                    int num = va_arg(ap, long);
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
                        if (token->name)
                        {
                            ft_putstr(fd, " with name: ");
                            ft_putstr(fd, token->name);
                            ft_putstr(fd, ", ");
                        }
                        else
                            ft_putchar(fd, ' ');
                        switch (token->type)
                        {
                        case characters_:
                        {
                            ft_putstr(fd, "value: ");
                            if (token->is_char)
                                ft_putchar(fd, token->characters[0]);
                            else
                                ft_putstr(fd, token->characters);
                            break;
                        }
                        case number_:
                        {
                            ft_putstr(fd, "value: ");
                            if (token->is_float)
                            {
                                if (ft_putfloat(fd, token->number, 6) == false)
                                    token->is_float = false;
                            }
                            else
                                ft_putfloat(fd, token->number, 0);
                            break;
                        }
                        case boolean_:
                        {
                            ft_putstr(fd, "value: ");
                            if (token->boolean)
                                ft_putstr(fd, "true");
                            else
                                ft_putstr(fd, "false");
                            break;
                        }
                        case array_:
                        {
                            ft_putstr(fd, "value: \n");
                            for (int i = 0; i < token->array_len; i++)
                            {
                                ft_printf(fd, "         %k\n", token->array[i]);
                            }
                            ft_putstr(fd, "     with size: ");
                            ft_putnbr(fd, token->array_len);
                            break;
                        }
                        case tuple_:
                        {
                            ft_putstr(fd, "value: \n");
                            for (int i = 0; i < token->array_len; i++)
                            {
                                ft_printf(fd, "         %k\n", token->array[i]);
                            }
                            ft_putstr(fd, "     with size: ");
                            ft_putnbr(fd, token->array_len);
                            break;
                        }
                        case obj_:
                        {
                            ft_putstr(fd, "value: \n");
                            for (int i = 0; i < token->object_len; i++)
                            {
                                ft_putstr(fd, "         ");
                                ft_putstr(fd, token->keys[i]);
                                ft_printf(fd, ": %k\n", token->object[i]);
                            }
                            ft_putstr(fd, "     with size: ");
                            ft_putnbr(fd, token->object_len);
                            break;
                        }
                        default:
                        {
                            if (type_to_string(token->type) == NULL)
                            {
                                ft_putstr(err, "ft_printf didn't know this token type: ");
                                ft_putstr(err, type_to_string(token->type));
                                exit(1);
                            }
                        }
                        }
                        // ft_putstr(fd, ", in position [");
                        // ft_putnbr(fd, token->index);
                        // ft_putstr(fd, "]");
                        if (token->line >= 0)
                        {
                            ft_putstr(fd, ", in line [");
                            ft_putnbr(fd, token->line);
                            ft_putstr(fd, "]");
                        }
                        ft_putstr(fd, ", in level [");
                        ft_putnbr(fd, token->level);
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
                    int c = va_arg(ap, int);
                    ft_putchar(fd, c);
                }
                if (fmt[i] == 's')
                {
                    char *str = va_arg(ap, char *);
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
}
// built in functions
void output(Token *token)
{
    int fd = 0;
    ft_putstr(fd, "\033[31m");
    if (token)
    {
        switch (token->type)
        {
        case identifier_:
            ft_printf(err, "Undeclared variable '%s' in output\n", token->name);
        // case params_:
        //     ft_printf(out, "Parameter with name '%s'\n", token->name);
        //     break;
        case characters_:
        {
            char *string = token->characters;
            if (token->is_char)
                ft_putchar(fd, string[0]);
            else
            {
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
                        else
                        {
                            ft_putchar(fd, string[i]);
                            i++;
                            break;
                        }
                    }
                    if (string[i] == '\0')
                        break;
                }
            }
            break;
        }
        case number_:
        {
            if (token->is_float)
            {
                if (ft_putfloat(fd, token->number, 6) == false)
                    token->is_float = false;
            }
            else
                ft_putfloat(fd, token->number, 0);
            break;
        }
        case boolean_:
        {
            if (token->boolean)
                ft_putstr(fd, "true");
            else
                ft_putstr(fd, "false");
            break;
        }
        case array_:
        {
            ft_putstr(fd, "[ ");
            for (int i = 0; i < token->array_len; i++)
            {
                output(token->array[i]);
                if (i < token->array_len - 1)
                    ft_putstr(fd, ", ");
            }
            ft_putstr(fd, " ]");
            break;
        }
        case tuple_:
        {
            ft_putstr(fd, "( ");
            for (int i = 0; i < token->array_len; i++)
            {
                output(token->array[i]);
                if (i < token->array_len - 1)
                    ft_putstr(fd, ", ");
            }
            ft_putstr(fd, " )");
            break;
        }
        case obj_:
        {
            ft_putstr(fd, " {\n");
            for (int i = 0; i < token->object_len; i++)
            {
                ft_putstr(fd, "    ");
                ft_putstr(fd, token->keys[i]);
                ft_putstr(fd, ": ");
                output(token->object[i]);
                if (i < token->object_len - 1)
                    ft_putstr(fd, ",\n");
            }
            ft_putstr(fd, "\n }");
            break;
        }
        default:
        {
            ft_putstr(err, "Error in output\n");
            exit(1);
        }
        }
    }
    else
        ft_putstr(fd, "NULL");
    ft_putstr(fd, "\033[0m");
}
// Debuging functions
void visualize_variables(void)
{
    int i = 0;
    ft_printf(out, "\n\nvariables: \n");
    while (VARIABLES[i])
    {
        if (VARIABLES[i])
            ft_printf(out, "     %k\n", VARIABLES[i]);
        i++;
    }
    ft_printf(out, "\nfunctions: \n");
    i = 0;
    while (FUNCTIONS[i])
    {
        if (FUNCTIONS[i])
            ft_printf(out, "     %k\n", FUNCTIONS[i]);
        ft_printf(out, "        with variables\n");
        int j = 0;
        while (FUNCTIONS[i]->VARIABLES[j])
        {
            ft_printf(out, "               %k\n", FUNCTIONS[i]->VARIABLES[j]);
            j++;
        }
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
        {"IDENTIFIER", identifier_},
        {"CHARACTERS", characters_},
        {"BOOLEAN", boolean_},
        {"NUMBER", number_},
        {"ARRAY", array_},
        {"OBJ", obj_},
        {"ASSIGNEMENT", assign_},
        {"EQUAL", equal_},
        {"NOT EQUAL", not_equal_},
        {"NOT", not_},
        {"MORE THAN", more_than_},
        {"LESS THAN", less_than_},
        {"MORE THAN OR EQUAL", more_than_or_equal_},
        {"LESS THAN OR EQUAL", less_than_or_equal_},
        {"FUNCTION DECLARATION", func_dec},
        {"FUNCTION CALL", func_call},
        {"TUPLE", tuple_},
        {"IF", if_},
        {"ELIF", elif_},
        {"ELSE", else_},
        {"WHILE", while_},
        {"AND", and_},
        {"OR", or_},
        {"DOTS", dots_},
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
        {"ADD ASSIGN", add_assign_},
        {"SUB ASSIGN", sub_assign_},
        {"DIV ASSIGN", div_assign_},
        {"MUL ASSIGN", mul_assign_},
        {"DOT", dot_},
        {"LEN", len_},
        {"BREAK", break_},
        {"CONTINUE", continue_},
        {"RETURN", return_},
        {0, 0},
    };
    for (int i = 0; Types[i].string; i++)
    {
        if (Types[i].type == type)
            return (Types[i].string);
    }
    ft_putstr(err, "Error: type not found '");
    ft_putnbr(err, type);
    ft_putstr(err, "'\n");
    exit(1);
    return NULL;
}

Token *new_token(Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->line = line;
    new->level = level;
    switch (type)
    {
    case identifier_:
    {
        type = 0;
        char *value = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(value, text + start, txt_pos - start);
        for (int i = 0; alpha_tokens[i].name; i++)
        {
            if (ft_strcmp(value, alpha_tokens[i].name) == 0)
            {
                type = alpha_tokens[i].type;
                break;
            }
        }
        if (type)
        {
            new->type = type;
            if (type == boolean_)
            {
                if (ft_strcmp(value, "true") == 0)
                    new->boolean = true;
                else if (ft_strcmp(value, "false") == 0)
                    new->boolean = false;
            }
        }
        else
            new->name = value;
        break;
    }
    case characters_:
    {
        new->characters = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->characters, text + start, txt_pos - start);
        break;
    }
    case number_:
    {
        double num = 0.0;
        while (ft_isdigit(text[start]))
        {
            num = 10 * num + text[start] - '0';
            start++;
        }
        if (text[start] == '.')
        {
            new->is_float = true;
            start++;
        }
        double pres = 0.1;
        while (ft_isdigit(text[start]))
        {
            num = num + pres * (text[start] - '0');
            pres /= 10;
            start++;
        }
        new->number = num;
        break;
    }
    default:
        type_to_string(type);
        break;
    }
    ft_printf(out, "new token: %k\n", new);
    tokens[tk_pos] = new;
    tk_pos++;
    tokens[tk_pos] = NULL;
    return new;
}

// Tokenizing
char *tab_space = "    ";
void build_tokens()
{
    Token *token;
    while (text[txt_pos])
    {
        if (text[txt_pos] == '\0')
            break;
        if (text[txt_pos] == '\n')
        {
            level = 0;
            line++;
            txt_pos++;
            continue;
        }
        if (ft_strncmp(&text[txt_pos], "//", ft_strlen("//")) == 0) // comments
        {
            while (text[txt_pos] && text[txt_pos] != '\n')
                txt_pos++;
            continue;
        }
        if (ft_strncmp(text + txt_pos, tab_space, ft_strlen(tab_space)) == 0)
        {
            level++;
            text += ft_strlen(tab_space);
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
            if (type == dots_) // for if statements and while loops ...
                level++;
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
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
                txt_pos++;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            new_token(number_);
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
    new_token(eof_);
}

// build nodes
Node *expr();           // expr
Node *assign();         // = += -= *= /=
Node **bloc(int level); // start with tab
Node *tenary();         // while, if, elif, else
Node *logic_or();       // || or
Node *logic_and();      // && and
Node *equality();       // ==  !=
Node *comparison();     // < > <= >=
Node *add_sub();        // + -
Node *mul_div();        // * /
Node *unary();          // sign + -
Node *attribute();      // .len ...
Node *iteration();      // iterate with index
Node *function();       // functions
Node *prime();          // final value

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

// bloc
Node **bloc(int lvl) // expected level
{
    ft_printf(out, "start bloc from %d\n", tokens[tk_pos]->line);
    if (tokens[tk_pos]->level < lvl)
        ft_printf(err, "Expected tab\n");
    int len = 0;

    Node **list = calloc(len + 1, sizeof(Node *));
    list[len] = expr();
    len++;
    list = realloc(list, (len + 1) * sizeof(Node *));
    while (tokens[tk_pos]->type != eof_ && tokens[tk_pos]->level == lvl)
    {
        list[len] = expr();
        // ft_printf(out, "curr level -> %d, and %k\n", tokens[tk_pos]->level, list[len]->token);
        len++;
        list = realloc(list, (len + 1) * sizeof(Node *));
    }
    list[len] = NULL;
    ft_printf(out, "exit bloc from %d with len %d\n", tokens[tk_pos]->line, len);
    return list;
}

Node *expr()
{
    return assign();
}



int is_assign(Type type)
{
    return (type == assign_ || type == add_assign_ || type == mul_assign_ || type == div_assign_);
}

// = += -= *= /=
Node *assign()
{
    Node *left = function();
    if (is_assign(tokens[tk_pos]->type))
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = left;
        node->right = function();
        left = node;
    }
    return left;
}

// functions
Node *function()
{
    // function call is in iteration
    if (tokens[tk_pos]->type == func_dec)
    {
        ft_printf(out, "build function\n");
        Node *node = new_node(tokens[tk_pos]);
        int func_level = tokens[tk_pos]->level;
        skip(func_dec);

        Node *func_name = prime();
        if (func_name->token->type != identifier_)
            ft_printf(err, "Error: Expected an identifier after 'func'\n");
        // function name
        node->token->name = func_name->token->name;
        // function params
        node->left = prime();
        skip(dots_);
        if (node->left->token->type != tuple_)
            ft_printf(err, "Error: Expected tuple after functions declaration\n");
        // node->right; //code block
        node->token->bloc_head = bloc(func_level + 1);

        return node;
    }
    if (tokens[tk_pos]->type == output_)
    {
        Node *left = new_node(tokens[tk_pos]);
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
        left->token->array_head = list;
        left->token->array_len = len;
        return left;
    }
    if (tokens[tk_pos]->type == return_)
    {
        Node *new = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        ft_printf(out, "found return \n");
        new->left = expr();
        // exit(0);
        return (new);
    }
    return (tenary());
}

// while, if, elif, else
Node *tenary()
{
    if (tokens[tk_pos]->type == if_ || tokens[tk_pos]->type == elif_)
    {
        Node *node = new_node(tokens[tk_pos]);
        int curr_level = tokens[tk_pos]->level;
        skip(tokens[tk_pos]->type);

        // left is condition
        node->left = expr();
        if (node->left == NULL)
            ft_printf(err, "Expected condition\n");
        skip(dots_);
        // right is bloc
        node->token->bloc_head = bloc(curr_level + 1);

        if (tokens[tk_pos]->type == elif_)
        {
            node->right = tenary();
        }
        if (tokens[tk_pos]->type == else_)
        {
            node->right = new_node(tokens[tk_pos]);
            skip(tokens[tk_pos]->type);
            skip(dots_);
            node->right->token->bloc_head = bloc(curr_level + 1);
        }
        return node;
    }
    else if (tokens[tk_pos]->type == while_)
    {
        Node *node = new_node(tokens[tk_pos]);
        int curr_level = tokens[tk_pos]->level;
        skip(tokens[tk_pos]->type);
        // left is condition
        node->left = expr();
        if (node->left == NULL)
            ft_printf(err, "Expected condition\n");
        skip(dots_);
        // right is bloc
        node->token->bloc_head = bloc(curr_level + 1);
        return node;
    }
    return logic_or();
}

// || or
Node *logic_or()
{
    Node *left = logic_and();
    if (tokens[tk_pos]->type == or_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        skip(tokens[tk_pos]->type);
        node->right = logic_and();
        left = node;
    }
    return left;
}

// && and
Node *logic_and()
{
    Node *left = equality();
    if (tokens[tk_pos]->type == and_)
    {
        Node *node = new_node(tokens[tk_pos]);
        node->left = left;
        skip(tokens[tk_pos]->type);
        node->right = equality();
        left = node;
    }
    return left;
}

// ==  !=
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

int is_comparison(Type type)
{
    return (type == less_than_ || type == more_than_ || type == less_than_or_equal_ || type == more_than_or_equal_);
}

// < > <= >=
Node *comparison()
{
    Node *left = add_sub();
    if (is_comparison(tokens[tk_pos]->type))
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = left;
        node->right = add_sub();
        left = node;
    }
    return left;
}

// + -
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

// * /
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

// sign + -
Node *unary()
{
    if (tokens[tk_pos]->type == sub_)
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->token->type = mul_;
        node->left = new_node(new_token(number_));
        node->left->token->number = -1;
        node->right = attribute();
        return node;
    }
    return attribute();
}

// .len ...
Node *attribute()
{
    Node *left = iteration();
    if (tokens[tk_pos]->type == dot_)
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = left;
        node->right = iteration();
        left = node;
    }
    return left;
}

// iterate with [index]
Node *iteration()
{
    Node *left = prime();
    if (tokens[tk_pos]->type == lbracket_)
    {
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->token->type = iteration_;
        node->left = left;
        node->right = function();
        skip(rbracket_);
        return node;
    }
    if (left && left->token->type == identifier_ && tokens[tk_pos]->type == lparent_)
    {
        ft_printf(out, "Call function\n");
        Node *node = new_node(left->token); // to be verified after
        node->token->type = func_call;
        node->left = prime(); // params
        return node;
    }
    return left;
}

int is_prime(Type type)
{
    return (type == identifier_ || type == number_ || type == characters_ || type == boolean_ || type == break_ || type == continue_);
}

Node *prime()
{
    Node *left = NULL;
    // if(tokens[tk_pos]->level < lvl)
    //     return NULL;
    if (is_prime(tokens[tk_pos]->type))
    {
        left = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
    }
    else if (tokens[tk_pos]->type == lbracket_)
    {
        left = new_node(tokens[tk_pos]);
        skip(lbracket_);
        left->token->type = array_;
        int len = 0;
        Node **list = calloc(len + 1, sizeof(Node *));
        if (tokens[tk_pos]->type != rbracket_) // enter if array is not empty
        {
            list[len] = assign(); // to be checked after
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
        left->token->array_head = list;
        left->token->array_len = len;
    }
    else if (tokens[tk_pos]->type == lparent_)
    {
        left = new_node(tokens[tk_pos]);
        skip(lparent_);
        left->token->type = tuple_;
        int len = 0;
        Node **list = calloc(len + 1, sizeof(Node *));
        if (tokens[tk_pos]->type != rparent_) // enter if array is not empty
        {
            list[len] = prime(); // to be checked after
            len++;
            list = realloc(list, (len + 1) * sizeof(Node *));
            while (tokens[tk_pos]->type != rparent_ && tokens[tk_pos]->type != eof_)
            {
                skip(comma_);
                list[len] = assign();
                len++;
                list = realloc(list, (len + 1) * sizeof(Node *));
            }
        }
        skip(rparent_);
        left->token->array_head = list;
        left->token->array_len = len;
        ft_printf(out, "-> len is : %d\n", len);
    }
    else if (tokens[tk_pos]->type == lcbracket_)
    {
        left = new_node(tokens[tk_pos]);
        skip(lcbracket_);
        left->token->type = obj_;
        int len = 0;
        Node **list = calloc(len + 1, sizeof(Node *));
        char **keys = calloc(len + 1, sizeof(char *));
        if (tokens[tk_pos]->type != rcbracket_) // enter if object is not empty
        {
            Token *identifier = tokens[tk_pos]; // expect identifier
            skip(identifier_);
            skip(dots_); // expect dots
            keys[len] = identifier->name;
            list[len] = add_sub();
            len++;
            list = realloc(list, (len + 1) * sizeof(Node *));
            keys = realloc(keys, (len + 1) * sizeof(char *));
            while (tokens[tk_pos]->type != rcbracket_ && tokens[tk_pos]->type != eof_)
            {
                skip(comma_);
                identifier = tokens[tk_pos]; // expect identifier
                skip(identifier_);
                skip(dots_); // expect dots
                keys[len] = identifier->name;
                list[len] = add_sub();
                len++;
                list = realloc(list, (len + 1) * sizeof(Node *));
                keys = realloc(keys, (len + 1) * sizeof(char *));
            }
        }
        skip(rcbracket_);
        left->token->keys = keys;
        left->token->object_len = len;
        left->token->object_head = list;
    }

    return left;
}

// Evaluate
Token *get_var(char *name)
{
    for (int i = 0; CURR_VARIABLES[i]; i++)
    {
        if (ft_strcmp(CURR_VARIABLES[i]->name, name) == 0)
        {
            return (CURR_VARIABLES[i]);
        }
    }
    for (int i = 0; VARIABLES[i]; i++)
    {
        if (ft_strcmp(VARIABLES[i]->name, name) == 0)
        {
            return (VARIABLES[i]);
        }
    }
    return NULL;
}

Token *new_variable(Token *var)
{
    CURR_VARIABLES[(*curr_var_index)++] = var;
    return (var);
}

Token *get_func(char *name)
{
    for (int i = 0; i < func_index; i++)
    {
        if (ft_strcmp(FUNCTIONS[i]->name, name) == 0)
        {
            return (FUNCTIONS[i]);
        }
    }
    ft_printf(err, "function not found %s\n", name);
    return NULL;
}

Token *new_func(Token *func)
{
    ft_printf(out, "new %v\n", func);
    FUNCTIONS[func_index++] = func;
    return (func);
}

Value *eval(Node *node)
{
    if (node == NULL)
        return NULL;
    switch (node->token->type)
    {
    // assignement
    case assign_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        ft_printf(out, "do assignement between %v and %v\n", left, right);
        if (right->type == identifier_)
            ft_printf(err, "Error , Undclared variable: %s\n", right->name);
        if (left->type != identifier_ && left->type != right->type)
            ft_printf(err, "can't assign '%s' type %t to '%s' type %t \n", left->name, left->type, right->name, right->type);

        char *name = left->name;
        if (right->type == characters_ && right->is_char == true)
        {
            if (left->type == identifier_)
            {
                left->type = characters_;
                left->is_char = true;
                left->characters = calloc(2, sizeof(char));
                left->characters[0] = right->characters[0];
            }
            else if (left->type == characters_ && left->is_char)
            {
                left->characters[0] = right->characters[0];
            }
            else if (left->type == characters_ && left->is_char == false)
            {
                left->characters[0] = right->characters[0];
            }
        }
        else if (right->type == characters_ && right->is_char == false)
        {
            if (left->type == identifier_)
            {
                left->type = characters_;
                left->characters = calloc(ft_strlen(right->characters) + 1, sizeof(char));
                memcpy(left->characters, right->characters, ft_strlen(right->characters));
            }
            else if (left->type == characters_ && left->is_char)
            {
                if (ft_strlen(right->characters) == 1)
                    left->characters[0] = right->characters[0];
                else
                    ft_printf(err, "Error: can't assign string to character\n");
            }
            else if (left->type == characters_ && left->is_char == false)
            {
                // protect it from leaks after
                left->characters = calloc(ft_strlen(right->characters) + 1, sizeof(char));
                memcpy(left->characters, right->characters, ft_strlen(right->characters));
            }
        }
        else
            memcpy(left, right, sizeof(Value)); // to be changed after, change only for chracters, I guess !!!
        left->name = name;
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
    case number_:
    case characters_:
    case boolean_:
    case break_:
    case continue_:
        return (node->token);
    case tuple_:
    case array_:
    {
        Node **head = node->token->array_head;
        node->token->array = calloc(node->token->array_len + 1, sizeof(Node *));

        int i = 0;
        while (i < node->token->array_len)
        {
            Value *to_assign = eval(head[i]);
            if (to_assign->type == identifier_)
                ft_printf(err, "key '%s', has no valid value\n", to_assign->name);
            node->token->array[i] = to_assign;
            i++;
        }
        return (node->token);
    }
    case obj_:
    {
        Node **head = node->token->object_head;
        node->token->object = calloc(node->token->object_len + 1, sizeof(Node *));

        int i = 0;
        while (i < node->token->object_len)
        {
            Value *to_assign = eval(head[i]);
            if (to_assign->type == identifier_)
                ft_printf(err, "key '%s', has no valid value\n", to_assign->name);
            node->token->object[i] = to_assign;
            i++;
        }
        return (node->token);
    }
    case iteration_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        Value *ret = calloc(1, sizeof(Value));
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        if (left->type != array_ && left->type != characters_)
            ft_printf(err, "can't iterate over type: %t\n", left->type);
        if (right->type != number_ || right->is_float)
            ft_printf(err, "expected integer value to iterate with it\n");
        // iterate
        long i = (long)right->number;
        // protect it from segfault
        if (left->type == array_)
        {
            ft_printf(out, "return %v\n", left->array[i]);
            return left->array[i];
        }
        else if (left->type == characters_)
        {
            ret->is_char = true;
            ret->type = characters_;
            ret->characters = left->characters + i;
            ft_printf(out, "return %v\n", ret);
            return (ret);
        }
        else
            ft_printf(err, "Error in iteration\n");
        break;
    }
    case if_:
    case elif_:
    {
        Value *condition = eval(node->left);
        Value *ret = NULL;
        if (condition->type != boolean_)
            ft_printf(err, "Error in if_ elif_, exepected boolean value\n");
        if (condition->boolean)
        {
            Node **bloc_head = node->token->bloc_head;
            int i = 0;
            while (bloc_head[i])
            {
                ret = eval(bloc_head[i]);
                i++;
            }
        }
        else
            ret = eval(node->right);
        return (ret); // to be verified
    }
    case else_:
    {
        Node **bloc_head = node->token->bloc_head;
        int i = 0;
        while (bloc_head[i])
        {
            eval(bloc_head[i]);
            i++;
        }
        return (NULL);
    }
    case while_:
    {
        Value *condition = eval(node->left);
        if (condition->type != boolean_)
            ft_printf(err, "Error in while_, exepected boolean value\n");
        while (condition->boolean)
        {
            Node **bloc_head = node->token->bloc_head;
            int i = 0;
            while (bloc_head[i])
            {
                Value *ret = eval(bloc_head[i]);
                if (ret && ret->type == break_)
                {
                    condition->boolean = false;
                    break;
                }
                if (ret && ret->type == continue_)
                    break;
                i++;
            }
            if (condition->boolean == false)
                break;
            condition = eval(node->left);
        }
        return (condition); // to be verified
    }
    // operator
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s' in operator 1\n", left->name);
        if (right->type == identifier_)
            ft_printf(err, "Undeclared variable '%s' in operator 2\n", right->name);
        Value *ret = calloc(1, sizeof(Value));
        if (left->type == number_ && right->type == number_)
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

            ret->is_float = left->is_float || right->is_float;
            ret->type = number_;
            ret->number = number;
            return (ret);
        }
        else if (node->token->type == add_ && left->type == characters_ && right->type == characters_)
        {
            ret->type = characters_;
            ret->is_char = left->is_char && right->is_char;
            ret->characters = calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            ft_strcpy(ret->characters, left->characters);
            ft_strcpy(ret->characters + ft_strlen(ret->characters), right->characters);
            return (ret);
        }
        else if (node->token->type == add_ && (left->type == array_ && right->type == array_))
        {
            ret->type = array_;
            ret->array_len = left->array_len + right->array_len;
            ret->array = calloc(ret->array_len + 1, sizeof(Token *));
            memcpy(ret->array, left->array, left->array_len * sizeof(Token *));
            memcpy(&ret->array[left->array_len], right->array, right->array_len * sizeof(Token *));
            return (ret);
        }
        else
        {
            ft_printf(err, "Error 1: in eval in '%t'\n       left: '%v'\n       right: '%v'\n", node->token->type, left, right);
        }
        return (ret);
    }
    case add_assign_:
    case sub_assign_:
    case mul_assign_:
    case div_assign_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s' operator 3\n", left->name);
        if (right->type == identifier_)
            ft_printf(err, "Undeclared variable '%s' operator 4\n", right->name);
        Value *ret = calloc(1, sizeof(Value));
        ft_printf(out, "do %t between %v and %v\n", node->token->type, left, right);
        if (left->type == number_ && right->type == number_)
        {
            long double number = 0.0;
            if (node->token->type == add_assign_)
                number = left->number + right->number;
            if (node->token->type == sub_assign_)
                number = left->number - right->number;
            if (node->token->type == mul_assign_)
                number = left->number * right->number;
            if (node->token->type == div_assign_)
                number = left->number / right->number;
            ret->is_float = left->is_float || right->is_float;
            ret->number = number;
            left->number = ret->number;
        }
        else if (node->token->type == add_assign_ && left->type == characters_ && right->type == characters_)
        {
            ret->type = left->is_char || right->is_char;
            ret->characters = calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            ft_strcpy(ret->characters, left->characters);
            ft_strcpy(ret->characters + ft_strlen(ret->characters), right->characters);
            left->characters = ret->characters;
        }
        else if (node->token->type == add_assign_ && (left->type == array_ && right->type == array_))
        {
            ret->type = array_;
            ret->array_len = left->array_len + right->array_len;
            ret->array = calloc(ret->array_len + 1, sizeof(Token *));
            memcpy(ret->array, left->array, left->array_len * sizeof(Token *));
            memcpy(&ret->array[left->array_len], right->array, right->array_len * sizeof(Token *));
            left->array = ret->array;
        }
        else
        {
            ft_printf(err, "Error 2: in eval in '%t'\n       left: '%v'\n       right: '%v'\n", node->token->type, left, right);
        }
        break;
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
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s' in comparision 1\n", left->name);
        if (right->type == identifier_)
            ft_printf(err, "Undeclared variable '%s' in comparision 2\n", right->name);
        Value *ret = calloc(1, sizeof(Value));
        if (left->type == number_ && right->type == number_)
        {
            switch (node->token->type)
            {
            case equal_:
                ret->boolean = left->number == right->number;
                break;
            case not_equal_:
                ret->boolean = left->number != right->number;
                break;
            case more_than_:
                ret->boolean = left->number > right->number;
                break;
            case less_than_:
                ret->boolean = left->number < right->number;
                break;
            case more_than_or_equal_:
                ret->boolean = left->number >= right->number;
                break;
            case less_than_or_equal_:
                ret->boolean = left->number <= right->number;
                break;
            default:
                break;
            }
            ret->type = boolean_;
            return (ret);
        }
        else if (node->token->type == equal_ && left->type == characters_ && right->type == characters_)
        {
            ret->type = boolean_;
            ret->boolean = ft_strcmp(left->characters, right->characters) == 0;
            return (ret);
        }
        else
        {
            ft_printf(err, "Error 3: in eval in '%t'\n       left: '%v'\n       right: '%v'\n", node->token->type, left, right);
        }
        break;
    }
    case and_:
    case or_:
    {
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        if (right->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", right->name);
        Value *ret = calloc(1, sizeof(Value));
        if (left->type != boolean_ || right->type != boolean_)
            ft_printf(err, "in %t operation, expected boolean values\n", node->token->type);

        switch (node->token->type)
        {
        case and_:
            ret->boolean = left->boolean && right->boolean;
            break;
        case or_:
            ret->boolean = left->boolean || right->boolean;
            break;
        default:
            ft_printf(err, "Error ...\n");
            break;
        }
        ret->type = boolean_;
        return (ret);
    }
    case dot_:
    {
        Value *left = eval(node->left);
        char *name = node->right->token->name;
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        if (left->type == obj_)
        {
            int i = 0;
            while (i < left->object_len)
            {
                if (strcmp(left->keys[i], name) == 0)
                {
                    ft_printf(out, "iterate return %v\n", left->object[i]);
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
        break;
    }
    case len_:
    {
        Value *left = eval(node->left);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        Value *ret = calloc(1, sizeof(Value));
        ret->type = number_;
        if (left->type == characters_)
            ret->number = (double)ft_strlen(left->characters);
        else if (left->type == array_)
            ret->number = left->array_len;
        else
            ft_printf(err, "data type '%t' has no attribute '%t'\n", left->type, len_);
        return ret;
        break;
    }
    case output_:
    {
        Node **head = node->token->array_head;
        int i = 0;
        // ft_printf(out, "Call ouput and print %d value\n", node->token->array_len);
        while (i < node->token->array_len)
        {
            output(eval(head[i]));
            i++;
        }
        return (node->token);
    }
    case func_dec:
    {
        Value *func_dec = new_func(node->token);
        ft_printf(out, "declare function with %d variable \n", node->left->token->array_len);
        func_dec->var_index = 0;
        func_dec->PREV_VARIABLES = CURR_VARIABLES;
        CURR_VARIABLES = func_dec->VARIABLES;
        curr_var_index = &func_dec->var_index;
        int i = 0;
        Node **params = node->left->token->array_head;
        while (i < node->left->token->array_len)
        {
            Value *x = eval(params[i]);
            ft_printf(out, "===> %v\n", x);
            i++;
        }
        CURR_VARIABLES = VARIABLES;
        curr_var_index = &var_index;
        visualize_variables();
        // exit(0);
        break;
    }
    case func_call:
    {
        // get func_run
        Value *func_run = get_func(node->token->name);
        Value *ret = NULL;
        ft_printf(out, "func: %v\n", func_run);

        // get params from fun_call
        Value *params = eval(node->left);
        ft_printf(out, "| with params: %v |\n\n", params);

        // assign params to func_run in VARIABLES
        CURR_VARIABLES = func_run->VARIABLES;
        curr_var_index = &func_run->var_index; // verify if it should get back to zero at end
        int i = 0;
        while (CURR_VARIABLES[i])
        {
            char *name = CURR_VARIABLES[i]->name;
            memcpy(CURR_VARIABLES[i], params->array[i], sizeof(Token));
            CURR_VARIABLES[i]->name = name;
            i++;
        }
        // call function
        Node **bloc_head = func_run->bloc_head;
        i = 0;
        while (bloc_head[i])
        {
            if(bloc_head[i]->token->type == return_)
            {
                // ft_printf(out, "found return in func_call -> %v\n", eval(bloc_head[i]->left));
                // exit(0);
                return(eval(bloc_head[i]->left));
            }
            Value *ret = eval(bloc_head[i]);
            ft_printf(out, "ret -> %v\n",ret);
            // if (ret && ret->type == return_)
            // {
            //     ft_printf(out, "return from function\n");
            //     exit(0);
            // }
            i++;
        }
        // reset setting
        CURR_VARIABLES = VARIABLES;
        curr_var_index = &var_index;
        break;
    }
    default:
        ft_printf(err, "Error: eval received unknown type '%t' \n", node->token->type);
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
    fp = fopen("file.txt", "r");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    text = calloc(file_size + 1, sizeof(char));
    fseek(fp, 0, SEEK_SET);
    fread(text, file_size, sizeof(char), fp);
    fclose(fp);

    // start compilling
    if (DEBUG)
        ft_printf(out, "%s\n", text);
    build_tokens();
    tmp_tk = tk_pos + 1;
    tk_pos = 0;
    line = -1;
    execute();
    if (DEBUG)
        visualize_variables();
}
