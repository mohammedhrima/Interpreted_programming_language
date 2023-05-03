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
typedef enum Type Type;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(Type type);
Token *evaluate(Node *node);

enum Type
{
    no_type,
    none_,
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
    modulo_,
    add_assign_,
    sub_assign_,
    mul_assign_,
    div_assign_,
    modulo_assign_,
    iteration_,
    dot_,
    attribute_,
    // len_,
    func_dec,
    func_call,
    // is attribute
    // isalpha_,
    // isnumber_,
    // isupper_,
    // islower_,
    // to_lower_,
    // to_upper_,
    // functions attributes
    indexof_,
    type_,
    ref_,
    inc_,
    dec_,
    // bloc_,
};

struct Token
{
    char *name;
    Type type;
    int level;
    int line;
    union
    {
        // number value
        struct
        {
            long double number;
            bool is_float; // integer, float
        };
        // characters value
        struct
        {
            char *characters;
            bool is_char;
        };
        // array value
        struct
        {
            Node **array_head;
            Token **array; // end it with NULL ptr
        };
        // object
        struct
        {
            Node **object_head;
            char **keys;    // ends it with NULL ptr
            Token **object; // ends it with NULL ptr
        };
        // block
        Node **bloc_head;
        // data type value
        Type value_type;
        // boolean value
        bool boolean;
    };
};

// alpha tokens
// will be gotten as identifiers
// then I compare them to the values here to check if it's one of them to change its type
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
    {"break", break_},
    {"continue", continue_},
    {"return", return_},
    // {"isalpha", isalpha_},
    // {"isnumber", isnumber_},
    // {"tolower", to_lower_},
    // {"toupper", to_upper_},
    // {"islower", islower_},
    // {"isupper", isupper_},
    {"indexof", indexof_},
    // {"type", type_},
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
    {"<=", less_than_or_equal_},
    {">=", more_than_or_equal_},
    {"<", less_than_},
    {">", more_than_},
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
    {"%", modulo_},
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
int exe_pos;

struct
{
    Token *VARIABLES[500]; // VARIABLES[var_index]; // it's better to use malloc here
    int var_index;
} VARIABLES_LEVELS[100]; // VARIABLES_LEVELS[var_level]
int var_level;
int max_level;

void access_next_scoop()
{
    max_level++;
    var_level++;
}
// free memory here, buuuuut be carefull
void exit_current_scoop()
{
    VARIABLES_LEVELS[var_level].var_index = 0;
    int i = 0;
    while (i < VARIABLES_LEVELS[var_level].var_index)
    {
        VARIABLES_LEVELS[var_level].VARIABLES[i] = NULL;
        i++;
    }
    var_level--;
}
// add max level because I can go further on levels
// without declaring variables in a middle step
// but declrae it in text one

Node *FUNCTIONS[500];
int func_index;

int line = 1;
int level;
int start;

// character methods
int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}
int ft_isupper(int c)
{
    return (c >= 'A' && c <= 'Z');
}
int ft_islower(int c)
{
    return (c >= 'a' && c <= 'z');
}
int ft_isalpha(int c)
{
    return (ft_islower(c) || ft_isupper(c));
}
int ft_isalnum(int c)
{
    return (ft_isalpha(c) || ft_isdigit(c));
}
int to_lower(int c)
{
    if (ft_isupper(c))
        return (c + 32);
    if (ft_islower(c))
        return (c);
    return (0);
}
int to_upper(int c)
{
    if (ft_islower(c))
        return (c - 32);
    if (ft_isupper(c))
        return (c);
    return (0);
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
// ptr methods
int Value_len(Value **ptr)
{
    int i = 0;

    while (ptr && ptr[i])
        i++;
    return (i);
}

// ft_printf
void ft_putchar(int fd, char c)
{
    write(fd, &c, sizeof(char));
}
void ft_putstr(int fd, char *str)
{
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
    if (decimal_places)
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
                            {
                                ft_putfloat(fd, token->number, 0);
                            }
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
                            for (int i = 0; token->array && token->array[i]; i++)
                                ft_printf(fd, "         %k\n", token->array[i]);
                            ft_putstr(fd, "         ");
                            break;
                        }
                        case obj_:
                        {
                            ft_putstr(fd, "value: \n");
                            for (int i = 0; token->object && token->object[i]; i++)
                            {
                                ft_putstr(fd, "         ");
                                ft_putstr(fd, token->keys[i]);
                                ft_printf(fd, ": %k\n", token->object[i]);
                            }
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
                        if (token->line >= 0)
                        {
                            ft_putstr(fd, " in line [");
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
        case type_:
            ft_putstr(fd, type_to_string(token->value_type));
            break;
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
            for (int i = 0; token->array && token->array[i]; i++)
            {
                output(token->array[i]);
                if (token->array[i + 1])
                    ft_putstr(fd, ", ");
            }
            ft_putstr(fd, " ]");
            break;
        }
        case obj_:
        {
            ft_putstr(fd, " {\n");
            for (int i = 0; token->object && token->object[i]; i++)
            {
                ft_putstr(fd, "    ");
                ft_putstr(fd, token->keys[i]);
                ft_putstr(fd, ": ");
                output(token->object[i]);
                if (token->object[i + 1])
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
#if 1
    int i = 0;
    ft_printf(out, "\n\nvariables: \n");
    // var_level is changeable don't think about putting it here
    while (i <= max_level)
    {
        ft_printf(out, "    level %d:\n", i);
        int j = 0;
        while (j < VARIABLES_LEVELS[i].var_index)
        {
            ft_printf(out, "        %k\n", VARIABLES_LEVELS[i].VARIABLES[j]);
            j++;
        }
        i++;
    }
    // ft_printf(out, "\nfunctions: \n");
    // i = 0;
    // while (FUNCTIONS[i])
    // {
    //     if (FUNCTIONS[i])
    //         ft_printf(out, "     %k\n", FUNCTIONS[i]);
    //     ft_printf(out, "        with variables\n");
    //     int j = 0;
    //     while (FUNCTIONS[i]->VARIABLES[j])
    //     {
    //         ft_printf(out, "               %k\n", FUNCTIONS[i]->VARIABLES[j]);
    //         j++;
    //     }
    //     i++;
    // }
#endif
    ft_printf(out, "\n");
}

// parsing functions
char *type_to_string(Type type)
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
        {"COMPARE", equal_},
        {"NOT EQUAL", not_equal_},
        {"NOT", not_},
        {"MORE THAN", more_than_},
        {"LESS THAN", less_than_},
        {"MORE THAN OR EQUAL", more_than_or_equal_},
        {"LESS THAN OR EQUAL", less_than_or_equal_},
        {"FUNCTION DECLARATION", func_dec},
        {"FUNCTION CALL", func_call},
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
        {"DIVISION", modulo_},
        {"ITERATION", iteration_},
        {"ADD ASSIGN", add_assign_},
        {"SUB ASSIGN", sub_assign_},
        {"DIV ASSIGN", div_assign_},
        {"MUL ASSIGN", mul_assign_},
        {"MODULO ASSIGN", modulo_assign_},
        {"DOT", dot_},
        {"BREAK", break_},
        {"CONTINUE", continue_},
        {"RETURN", return_},
        {"ATTRIBUTE", attribute_},
        // {"ISALPHA", isalpha_},
        // {"ISNUMBER", isnumber_},
        // {"TO LOWER", to_lower_},
        // {"TO UPPER", to_upper_},
        // {"IS LOWER", islower_},
        // {"IS UPPER", isupper_},
        {"indexof", indexof_},
        {"REF", ref_},
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

//  Tokenizing
char *tab_space = "    ";
void build_tokens()
{
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
        if (ft_strncmp(&text[txt_pos], "\"\"\"", ft_strlen("\"\"\"")) == 0) // extra comments X'D
        {
            txt_pos += ft_strlen("\"\"\"");
            while (text[txt_pos] && ft_strncmp(&text[txt_pos], "\"\"\"", ft_strlen("\"\"\"")))
                txt_pos++;
            txt_pos += ft_strlen("\"\"\"");
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
            while (ft_isalnum(text[txt_pos]) || text[txt_pos] == '_')
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

// Build nodes
Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Token));
    new->token = token;
    ft_printf(out, "new node: %k\n", new->token);
    return (new);
}

void skip(Type type)
{
    if (tokens[exe_pos]->type != type)
        ft_printf(err, "error current type is '%t' and want to skip '%t in pos '%d''\n", tokens[exe_pos]->type, type, exe_pos);
    exe_pos++;
}

Node **bloc(int level); // bloc of code
Node *expr();           // expr
Node *assign();         // = += -= *= /=
Node *conditions();     // while, if, elif, else
Node *logic_or();       // || or
Node *logic_and();      // && and
Node *equality();       // ==  !=
Node *comparison();     // < > <= >=
Node *add_sub();        // + -
Node *mul_div();        // * /
Node *sign();           // sign  -
Node *prime();          // primary

// bloc
Node **bloc(int lvl) // expected level
{
    if (tokens[exe_pos]->level < lvl)
        ft_printf(err, "Expected tab\n");
    int len = 0;

    Node **list = calloc(len + 1, sizeof(Node *));
    list[len] = expr();
    len++;
    list = realloc(list, (len + 1) * sizeof(Node *));
    while (tokens[exe_pos]->type != eof_ && tokens[exe_pos]->level == lvl)
    {
        list[len] = expr();
        len++;
        list = realloc(list, (len + 1) * sizeof(Node *));
        // ft_printf(out, "bloc\n");
    }
    list[len] = NULL;
    return list;
}

// check
bool check(Type to_find, ...)
{
    va_list ap;
    va_start(ap, to_find);
    while (1)
    {
        Type type = va_arg(ap, Type);
        if (type == to_find)
            return true;
        if (type == 0)
            break;
    }
    return false;
}

// expression
Node *expr()
{
    return assign();
}

// = += -= *= /= %=
Node *assign()
{
    Node *left = conditions();
    if (check(tokens[exe_pos]->type, assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, modulo_assign_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->left = left;
        ft_printf(out, "left type: %k\n", node->left->token);
        node->right = conditions();
        ft_printf(out, "right type: %k\n", node->right->token);
        left = node;
    }
    return left;
}

// while, if, elif, else
Node *conditions()
{
    if (check(tokens[exe_pos]->type, if_, elif_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        int curr_level = tokens[exe_pos]->level;
        skip(tokens[exe_pos]->type);

        // left is condition
        node->left = expr();
        if (node->left == NULL)
            ft_printf(err, "Expected condition\n");
        skip(dots_);

        // current is bloc
        node->token->bloc_head = bloc(curr_level + 1);
        if (tokens[exe_pos]->type == elif_)
        {
            node->right = conditions();
        }
        if (tokens[exe_pos]->type == else_)
        {
            ft_printf(out, "if level is %d\nelse level is %d\n", curr_level, tokens[exe_pos]->level);
            // exit(0);
            node->right = new_node(tokens[exe_pos]);
            skip(tokens[exe_pos]->type);
            skip(dots_);
            node->right->token->bloc_head = bloc(curr_level + 1);
        }
        return node;
    }

    else if (tokens[exe_pos]->type == while_)
    {
        Node *node = new_node(tokens[exe_pos]);
        int curr_level = tokens[exe_pos]->level;
        skip(tokens[exe_pos]->type);

        // left is condition
        node->left = expr();
        if (node->left == NULL)
            ft_printf(err, "Expected condition\n");
        skip(dots_);

        // current is bloc
        node->token->bloc_head = bloc(curr_level + 1);
        return node;
    }
    return logic_or();
}

// || or
Node *logic_or()
{
    Node *left = logic_and();
    while (tokens[exe_pos]->type == or_)
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(or_);
        node->left = left;
        node->right = logic_and();
        left = node;
    }
    return left;
}

// && and
Node *logic_and()
{
    Node *left = equality();
    while (tokens[exe_pos]->type == and_)
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(and_);
        node->left = left;
        node->right = equality();
        left = node;
    }
    return left;
}

// ==  !=
Node *equality()
{
    Node *left = comparison();
    if (check(tokens[exe_pos]->type, equal_, not_equal_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->left = left;
        node->right = comparison();
        left = node;
    }
    return left;
}

// < > <= >=
Node *comparison()
{
    Node *left = add_sub();
    if (check(tokens[exe_pos]->type, less_than_, more_than_, less_than_or_equal_, more_than_or_equal_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
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
    while (check(tokens[exe_pos]->type, add_, sub_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
    return left;
}

// * / %
Node *mul_div()
{
    Node *left = sign();
    while (check(tokens[exe_pos]->type, mul_, div_, modulo_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->left = left;
        node->right = sign();
        left = node;
    }
    return left;
}

// sign -
Node *sign()
{
    if (tokens[exe_pos]->type == sub_)
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(sub_);
        node->token->type = mul_;
        node->left = new_node(new_token(number_));
        node->left->token->number = -1;
        node->right = prime();
        return node;
    }
    return prime();
}

// primary
Node *prime()
{
    Node *node = NULL;
    Node *left = NULL;
    // function declaration
    if (tokens[exe_pos]->type == func_dec)
    {
        ft_printf(out, "found function declaration \n");
        // starts with func key word
        skip(func_dec);

        node = new_node(tokens[exe_pos]);
        int curr_level = tokens[exe_pos]->level;
        skip(identifier_);
        node->token->type = func_dec;

        node->left = new_node(tokens[exe_pos]);
        skip(lparent_);
        int i = 0;
        Node **params = calloc(i + 1, sizeof(Node *));
        if (tokens[exe_pos]->type != rparent_) // enter if there is params to hold
        {
            params[i] = new_node(tokens[exe_pos]); // to be checked after
            skip(identifier_);
            ft_printf(out, "get param %d -> %v\n", i, params[i]->token);
            // expect identifier
            i++;
            params = realloc(params, (i + 1) * sizeof(Node *));
            while (tokens[exe_pos]->type != rparent_ && tokens[exe_pos]->type != eof_)
            {
                skip(comma_);
                params[i] = new_node(tokens[exe_pos]);
                skip(identifier_);

                ft_printf(out, "get param %d -> %v\n", i, params[i]->token);
                // expect identifier
                i++;
                params = realloc(params, (i + 1) * sizeof(Node *));
            }
        }
        params[i] = NULL;
        skip(rparent_);

        node->left->token->array_head = params;
        // exit(0);
        // expect :
        skip(dots_);
        // current is bloc
        node->token->bloc_head = bloc(curr_level + 1);
        return node;
    }
    if (tokens[exe_pos]->type == identifier_)
    {
        Node *node = new_node(tokens[exe_pos]);
        left = node;
        skip(identifier_);
        if (tokens[exe_pos]->type == lparent_)
        {
            node->token->type = func_call;
            node->left = new_node(tokens[exe_pos]); // params
            skip(lparent_);
            int i = 0;
            Node **params = calloc(i + 1, sizeof(Node *));
            if (tokens[exe_pos]->type != rparent_) // enter if there is params to hold
            {
                params[i] = expr(); // to be checked after
                i++;
                params = realloc(params, (i + 1) * sizeof(Node *));
                while (tokens[exe_pos]->type != rparent_ && tokens[exe_pos]->type != eof_)
                {
                    skip(comma_);
                    params[i] = expr();
                    i++;
                    params = realloc(params, (i + 1) * sizeof(Node *));
                }
            }
            params[i] = NULL;
            skip(rparent_);
            node->left->token->array_head = params;
            left = node;

            // left = node;
        }
        // else , I commented it because I can iterate over function
#if 0
        while (check(tokens[exe_pos]->type, lbracket_, dot_, 0))
        {
            // left will be element to iterate over it
            // right element to iterate with it
            if (tokens[exe_pos]->type == lbracket_)
            {
                node = new_node(tokens[exe_pos]);
                skip(lbracket_);
                node->token->type = iteration_;
                node->left = left;    // element to iterate over it
                node->right = expr(); // index (number) , I'm thinking about using words also like javascript does
                skip(rbracket_);
                left = node;
            }
            if (tokens[exe_pos]->type == dot_)
            {
                node = new_node(tokens[exe_pos]);
                skip(dot_);
                node->left = left;     // element to iterate over it
                node->right = prime(); // expect identifier, indexof ...
                ft_printf(out, "left: %v\nright: %v\n", node->left->token, node->right->token);
                left = node;
                // ft_printf(out, "left: %v\nright: %v\n", node->left->token, node->right->token);
                // exit(0);
                /*
                if is indexof give it who you are on left !!!
                oooor do a smart move and handle it in evaluating
                check if right key is index of
                then check you data type and left of indexof
                and do your magic
                */
                // left = node;
            }
#endif
        while (check(tokens[exe_pos]->type, dot_, lbracket_))
        {
            while (tokens[exe_pos]->type == dot_)
            {
                node = new_node(tokens[exe_pos]);
                skip(dot_);
                node->left = left;                       // element to iterate over it
                node->right = new_node(tokens[exe_pos]); // expect identifier, indexof ...
                skip(identifier_);
                node->right->token->type = attribute_;
                ft_printf(out, "left: %v\nright: %v\n", node->left->token, node->right->token);
                left = node;
            }
            while (tokens[exe_pos]->type == lbracket_)
            {
                node = new_node(tokens[exe_pos]);
                skip(lbracket_);
                node->left = left; // element to iterate over it
                node->right = expr();
                skip(rbracket_);
                node->token->type = iteration_;
                // ft_printf(out, "left: %v\nright: %v\n", node->left->token, node->right->token);
                left = node;
            }
        }

        return left;
    }
    // indexof
    if (tokens[exe_pos]->type == indexof_)
    {
        node = new_node(tokens[exe_pos]);
        skip(indexof_);
        skip(lparent_);
        node->left = expr();
        // expected characters or number
        // but number can be 1 + 2, and 1+2 is expr that will turns to number
        skip(rparent_);
        return node;
    }
    // output function
    if (tokens[exe_pos]->type == output_)
    {
        node = new_node(tokens[exe_pos]);
        Node **list = NULL;

        skip(output_);
        skip(lparent_);
        if (tokens[exe_pos]->type == rparent_)
            ft_printf(err, "expected something to output\n");
        else
        {
            int i = 0;
            list = calloc(i + 1, sizeof(Node *));
            list[i] = assign();
            i++;
            list = realloc(list, (i + 1) * sizeof(Node *));
            while (tokens[exe_pos]->type != rparent_ && tokens[exe_pos]->type != eof_)
            {
                skip(comma_);
                list[i] = assign();
                i++;
                list = realloc(list, (i + 1) * sizeof(Node *));
            }
            list[i] = NULL;
        }
        skip(rparent_);
        node->token->array_head = list;
        return node;
    }
    // array
    if (tokens[exe_pos]->type == lbracket_)
    {
        node = new_node(tokens[exe_pos]);
        Node **list = NULL;

        skip(lbracket_);
        node->token->type = array_;
        int i = 0;
        if (tokens[exe_pos]->type != rbracket_) // enter if array is not empty
        {
            list = calloc(i + 1, sizeof(Node *));
            list[i] = assign();
            i++;
            list = realloc(list, (i + 1) * sizeof(Node *));
            while (tokens[exe_pos]->type != rbracket_ && tokens[exe_pos]->type != eof_)
            {
                skip(comma_);
                list[i] = assign();
                i++;
                list = realloc(list, (i + 1) * sizeof(Node *));
            }
            list[i] = NULL;
        }
        skip(rbracket_);
        node->token->array_head = list;
        return node;
    }
    // object
    // check if you can optimize object
    if (tokens[exe_pos]->type == lcbracket_)
    {
        node = new_node(tokens[exe_pos]);
        skip(lcbracket_);
        node->token->type = obj_;
        int i = 0;
        Node **list = NULL;
        char **keys = NULL;
        if (tokens[exe_pos]->type != rcbracket_) // enter if object is not empty
        {
            list = calloc(i + 1, sizeof(Node *));
            keys = calloc(i + 1, sizeof(char *));

            Token *identifier = tokens[exe_pos]; // expect identifier
            skip(identifier_);
            skip(dots_); // expect dots
            keys[i] = identifier->name;
            list[i] = add_sub();
            i++;
            list = realloc(list, (i + 1) * sizeof(Node *));
            keys = realloc(keys, (i + 1) * sizeof(char *));
            while (tokens[exe_pos]->type != rcbracket_ && tokens[exe_pos]->type != eof_)
            {
                skip(comma_);
                identifier = tokens[exe_pos]; // expect identifier
                skip(identifier_);
                skip(dots_); // expect dots
                keys[i] = identifier->name;
                list[i] = add_sub();
                i++;
                list = realloc(list, (i + 1) * sizeof(Node *));
                keys = realloc(keys, (i + 1) * sizeof(char *));
            }
        }
        keys[i] = NULL;
        list[i] = NULL;
        skip(rcbracket_);
        node->token->keys = keys;
        node->token->object_head = list;
        return node;
    }
    if (tokens[exe_pos]->type == lparent_)
    {
        skip(lparent_);
        node = expr();
        skip(rparent_);
        return node;
    }
    if (check(tokens[exe_pos]->type, type_, number_, characters_, boolean_, break_, continue_, 0))
    {
        node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        left = node;
#if 1
        // try to iterate over string even if it's not define somewhere
        // else , I commented it because I can iterate over function
        if (tokens[exe_pos]->type == dot_)
        {
            while (tokens[exe_pos]->type == dot_)
            {
                node = new_node(tokens[exe_pos]);
                skip(dot_);
                node->left = left;                       // element to iterate over it
                node->right = new_node(tokens[exe_pos]); // expect identifier, indexof ...
                skip(identifier_);
                node->right->token->type = attribute_;
                // ft_printf(out, "left: %v\nright: %v\n", node->left->token, node->right->token);
                left = node;
            }
            // exit(0);
        }
#endif
        return node;
    }
    if (tokens[exe_pos]->type == return_)
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->left = expr();
        return (node);
    }
    // if (tokens[exe_pos]->type == inc_ || tokens[exe_pos]->type == dec_)
    // {
    //     // that
    // }
    return NULL;
}

// Evaluate
Token *new_variable(Token *var)
{
    int i = VARIABLES_LEVELS[var_level].var_index;

    VARIABLES_LEVELS[var_level].VARIABLES[i] = var;
    VARIABLES_LEVELS[var_level].var_index++;
    return (var);
}

Token *get_var(char *name)
{
    int curr_level = var_level;
    while (curr_level >= 0)
    {
        // ft_printf(out, "level %d \n", curr_level);
        for (int i = 0; VARIABLES_LEVELS[curr_level].VARIABLES[i]; i++)
        {
            if (ft_strcmp(VARIABLES_LEVELS[curr_level].VARIABLES[i]->name, name) == 0)
            {
                ft_printf(out, "found in level %d\n", curr_level);
                return (VARIABLES_LEVELS[curr_level].VARIABLES[i]);
            }
        }
        curr_level--;
    }
    return NULL;
}

// Function
Node *new_func(Node *func)
{
    ft_printf(out, "new %v\n", func->token);
    FUNCTIONS[func_index] = func;
    func_index++;
    return (func);
}

Node *get_func(char *name)
{
    for (int i = 0; i < func_index; i++)
    {
        if (ft_strcmp(FUNCTIONS[i]->token->name, name) == 0)
        {
            return (FUNCTIONS[i]);
        }
    }
    ft_printf(err, "function not found %s\n", name);
    return NULL;
}

Value *evaluate(Node *node)
{
    switch (node->token->type)
    {
    // assignement
    case assign_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
        ft_printf(out, "do assignement between %v and %v\n", left, right);
        // exit(0);
        // skip this error for function because i can set global variable to idenitifer from params
        // if (right->type == identifier_)
        //     ft_printf(err, "Error , Undeclared variable: %s\n", right->name);

        // if (left->type != identifier_ && left->type != right->type)
        //     ft_printf(err, "can't assign '%s' type %t to '%s' type %t \n", left->name, left->type, right->name, right->type);

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
            memcpy(left, right, sizeof(Value)); // to be changed after, change only for characters, I guess !!!
        left->name = name;
        visualize_variables();
        // exit(0);
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
    // values and some keywords
    case attribute_:
    case number_:
    case characters_:
    case boolean_:
    case break_:
    case continue_:
    case indexof_:
    case type_:
    {
        return (node->token);
    }
    // array
    case array_:
    {
        Node **head = node->token->array_head;
        int i = 0;

        Token **array = calloc(i + 1, sizeof(Token *));
        while (head[i])
        {
            Value *to_assign = evaluate(head[i]);
            if (to_assign->type == identifier_)
                ft_printf(err, "key '%s', has no valid value\n", to_assign->name);
            ft_printf(out, "value : %v\n", to_assign);
            array[i] = to_assign;
            i++;
            array = realloc(array, (i + 1) * sizeof(Token *));
            array[i] = NULL;
        }
        node->token->array = array;
        return (node->token);
    }
    // object
    case obj_:
    {
        Node **head = node->token->object_head;
        int i = 0;

        Token **object = calloc(i + 1, sizeof(Token *));
        while (head[i])
        {
            Value *to_assign = evaluate(head[i]);
            if (to_assign->type == identifier_)
                ft_printf(err, "key '%s', has no valid value\n", to_assign->name);
            ft_printf(out, "value : %v\n", to_assign);
            object[i] = to_assign;
            i++;
            object = realloc(object, (i + 1) * sizeof(Token *));
            object[i] = NULL;
        }
        node->token->object = object;
        return (node->token);
    }
    // math operator
    case add_:
    case sub_:
    case mul_:
    case div_:
    case modulo_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);

        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        if (right->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", right->name);
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
            if (node->token->type == modulo_)
                number = (long)left->number % (long)right->number;

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
            int left_len = Value_len(left->array);
            int right_len = Value_len(right->array);
            ret->type = array_;
            ret->array = calloc(left_len + right_len + 1, sizeof(Token *));
            memcpy(ret->array, left->array, left_len * sizeof(Token *));
            memcpy(&ret->array[left_len], right->array, right_len * sizeof(Token *));
            return (ret);
        }
        else
            ft_printf(err, "Error 1: can't do '%t' between '%t' and '%t'\n", node->token->type, left->type, right->type);
        break;
    }
    // math assign operator
    case add_assign_:
    case sub_assign_:
    case mul_assign_:
    case div_assign_:
    case modulo_assign_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        if (right->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", right->name);
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
            if (node->token->type == modulo_assign_)
                number = (long)left->number % (long)right->number;

            ret->is_float = left->is_float || right->is_float;
            if (node->token->type == modulo_assign_)
                ret->is_float = false;
            ret->type = number_;
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
            int left_len = Value_len(left->array);
            int right_len = Value_len(right->array);
            ret->type = array_;
            ret->array = calloc(left_len + right_len + 1, sizeof(Token *));
            memcpy(ret->array, left->array, left_len * sizeof(Token *));
            memcpy(&ret->array[left_len], right->array, right_len * sizeof(Token *));
            left->array = ret->array;
        }
        else
        {
            ft_printf(err, "Error 2: can't do '%t' between '%t' and '%t'\n", node->token->type, left->type, right->type);
        }
        break;
    }
    // statements
    case if_:
    case elif_:
    {
        ft_printf(out, "enter if\n");
        Value *condition = evaluate(node->left);
        Value *ret = NULL;
        if (condition->type != boolean_)
            ft_printf(err, "Error in if_ elif_, exepected boolean value\n");
        // exit(0);
        if (condition->boolean)
        {
            Node **bloc_head = node->token->bloc_head;
            int i = 0;
            while (bloc_head[i])
            {
                ret = evaluate(bloc_head[i]);
                i++;
            }
        }
        else if (node->right)
        {
            ret = evaluate(node->right);
        }
        return (ret); // to be verified
    }
    case else_:
    {
        Node **bloc_head = node->token->bloc_head;
        Value *ret = NULL;
        int i = 0;
        while (bloc_head[i])
        {
            ret = evaluate(bloc_head[i]);
            i++;
        }
        ft_printf(out, "else return %v\n", ret);
        return (ret);
    }
    case while_:
    {
        Value *condition = evaluate(node->left);
        Value *ret;
        if (condition->type != boolean_)
            ft_printf(err, "Error in while_, exepected boolean value\n");
        while (condition->boolean)
        {
            Node **bloc_head = node->token->bloc_head;
            int i = 0;
            while (bloc_head[i])
            {
                ret = evaluate(bloc_head[i]);
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
            condition = evaluate(node->left);
        }
        return (ret);
    }
    // logic operator
    case equal_:
    case not_equal_:
    case more_than_:
    case less_than_:
    case more_than_or_equal_:
    case less_than_or_equal_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
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
            ft_printf(err, "Error 3: can't do '%t' between '%t' and '%t'\n", node->token->type, left->type, right->type);
        }
        break;
    }
    case and_:
    case or_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
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
    case output_:
    {
        Node **head = node->token->array_head;
        int i = 0;
        while (head[i])
        {
            output(evaluate(head[i]));
            i++;
        }
        return (node->token);
    }
    case func_dec:
    {
        // this step is only to check if there is error inside fucntion
        ft_printf(out, "eval func_dec\n");
        Node *func_dec = new_func(node);
        access_next_scoop();
        int i = 0;
        Node **params = node->left->token->array_head;
        while (params[i])
        {
            if (params[i]->token->type != identifier_)
                ft_printf(err, "Error in function declaration\n");
            new_variable(params[i]->token);
            // ft_printf(out, "===> %v\n", x);
            i++;
        }
        ft_printf(out, "after getting params -> %d\n", i);

        // code bloc
        // Node **bloc = func_dec->token->bloc_head;
        // i = 0;
        // while (bloc[i])
        // {
        //     // don't evaluate because you may change some original value
        //     // of global variables
        //     Value *x = evaluate(bloc[i]);
        //     i++;
        // }
        // visualize_variables();
        exit_current_scoop();
        break;
    }
    case func_call:
    {
        ft_printf(out, "call function %s\n", node->token->name);
        // find function
        Node *existed_func = get_func(node->token->name);

        Node **existed_params = existed_func->left->token->array_head;
        Node **new_params = node->left->token->array_head;
        // access next scoop
        // access_next_scoop();
        // evaluate params of original function, and set hem to those in call
        int i = 0;
        while (existed_params[i])
        {
            // if (existed_params[i]->token->type != identifier_)
            // ft_printf(err, "Error in function call\n");
            new_variable(existed_params[i]->token);
            // ft_printf(out, "===> %v\n", x);
            i++;
        }
        i = 0;
        while (new_params[i])
        {
            ft_printf(out, "assign\n        %v\nto\n          %v\n", existed_params[i]->token, evaluate(new_params[i]), sizeof(Token));
            char *name = existed_params[i]->token->name;
            
            // if(new_params[i]->token->type == identifier_)
            
            memcpy(existed_params[i]->token, evaluate(new_params[i]), sizeof(Token));
            existed_params[i]->token->name = name;
            // ft_printf(out, "===> %v\n", x);
            i++;
        }

        // also check number of params

        // start evaluating bloc
        Node **bloc = existed_func->token->bloc_head;
        Value *x = NULL;
        i = 0;
        while (bloc[i])
        {
            x = evaluate(bloc[i]);
            i++;
        }
        return x;
        visualize_variables();
        // exit_current_scoop();
        // exit(0);
        break;
    }
    case return_:
    {
        return (evaluate(node->left));
    }
    case dot_:
    {
        // len, isnumber, isalpha, indexof ...
        // it's better to check left type then check right attribute
        // attribute with key (identifier)
        // or with characters !!!
        Value *left = evaluate(node->left); // variable
        Value *right = evaluate(node->right);

        ft_printf(out, "left: %v\nright: %v\n", left, right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        if (right->type == type_)
        {
            Value *ret = calloc(1, sizeof(Value));
            ret->type = type_;
            ret->value_type = left->type;
            return ret;
        }
        if (left->type == obj_)
        {
            int i = 0;
            while (left->keys && left->keys[i])
            {
                if (strcmp(left->keys[i], right->name) == 0)
                {
                    ft_printf(out, "iterate return %v\n", left->object[i]);
                    left = left->object[i];
                    return left;
                }
                i++;
            }
            ft_printf(err, "Error: OBJ has no attribute '%s'\n", right->name);
        }
        if (left->type == characters_)
        {
            int i = 0;
            if (ft_strcmp(right->name, "len") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = number_;
                ret->number = (double)ft_strlen(left->characters);
                return ret;
            }
            if (ft_strcmp(right->name, "toup") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = characters_;
                while (left->characters && left->characters[i])
                {
                    ret->characters[i] = to_upper(left->characters[i]);
                    i++;
                }
                return ret;
            }
            if (ft_strcmp(right->name, "tolow") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = characters_;
                while (left->characters && left->characters[i])
                {
                    ret->characters[i] = to_lower(left->characters[i]);
                    i++;
                }
                return ret;
            }
            if (ft_strcmp(right->name, "tonum") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = number_;
                // to verify after
                ret->number = atof(left->characters);
                return ret;
            }
            if (ft_strcmp(right->name, "isup") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = boolean_;
                while (left->characters && left->characters[i])
                {
                    if (!ft_isupper(left->characters[i]))
                    {
                        ret->boolean = false;
                        return ret;
                    }
                    i++;
                }
                ret->boolean = true;
                return ret;
            }
            if (ft_strcmp(right->name, "islow") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = boolean_;
                while (left->characters && left->characters[i])
                {
                    if (!ft_islower(left->characters[i]))
                    {
                        ret->boolean = false;
                        return ret;
                    }
                    i++;
                }
                ret->boolean = true;
                return ret;
            }
            if (ft_strcmp(right->name, "ischar") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = boolean_;
                while (left->characters && left->characters[i])
                {
                    if (!ft_isalpha(left->characters[i]))
                    {
                        ret->boolean = false;
                        return ret;
                    }
                    i++;
                }
                ret->boolean = true;
                return ret;
            }
            if (ft_strcmp(right->name, "isnum") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = boolean_;
                ret->boolean = true;
                while (left->characters && left->characters[i] && ft_isdigit(left->characters[i]))
                    i++;
                if (left->characters && left->characters[i] == '.')
                    i++;
                while (left->characters && left->characters[i] && ft_isdigit(left->characters[i]))
                    i++;
                if (left->characters[i])
                    ret->boolean = false;
                return ret;
            }
            else
                ft_printf(err, "%s has no attribute %s\n", left->name, right->name);
        }
        // if (left->type == number_)
        // {
        //     if (ft_strcmp(right->name, "tochar") == 0)
        //     {
        //     }
        //     else
        //         ft_printf(err, "%s has no attribute %s\n", left->name, right->name);
        // }
        if (left->type == array_)
        {
            if (ft_strcmp(right->name, "len") == 0)
            {
                Value *ret = calloc(1, sizeof(Value));
                ret->type = number_;
                long i = 0;
                while (left->array && left->array[i])
                    i++;
                ret->number = i;
                return ret;
            }
            else
                ft_printf(err, "%s has no attribute %s\n", left->name, right->name);
        }
        ft_printf(err, "Error in dot\n");
        break;
    }
    case iteration_:
    {
        // it's better to check left type then check right attribute
        // iteration over characters and arrays
        // iteration with number
        Value *left = evaluate(node->left);   // variable
        Value *right = evaluate(node->right); // expect number

        ft_printf(out, "left: %v\nright: %v\n", left, right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        if (right->type != number_)
            ft_printf(err, "can't iterate over %t with %t\n", left->type, right->type);
        if (left->type == characters_)
        {
            long i = 0;
            Value *ret = calloc(1, sizeof(Value));
            ret->type = characters_;
            ret->is_char = true;
            while (left->characters && left->characters[i])
            {
                if (i == right->number)
                {
                    ret->characters = left->characters + (long)right->number;
                    return ret;
                }
                i++;
            }
            ft_printf(err, "Index out of range\n");
        }
        if (left->type == array_)
        {
            long i = 0;
            while (left->array && left->array[i])
            {
                if (i == right->number)
                    return left->array[i];
                i++;
            }
            ft_printf(err, "Index out of range\n");
        }
        ft_printf(err, "Can't iterate over %t\n", left->type);
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
    while (tokens[exe_pos]->type != eof_)
        evaluate(expr());
}

int main(int argc, char **argv)
{
    if (argc == 1)
        ft_printf(err, "expected file to run argc: %d\n", argc);
    FILE *fp = NULL;
    long file_size = 0;

    // open file and read it and feed the text
    fp = fopen(argv[1], "r");
    if (fp == NULL)
        ft_printf(err, "Error openning file\n");
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
    tk_pos++; // verify it after

#if 1
    line = -1;
    execute();
    if (DEBUG)
        visualize_variables();
#endif
}