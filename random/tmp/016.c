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
#define DEBUG false

// typedefs
typedef struct Node Node;
typedef struct Token Token;
typedef struct Token Value;
typedef struct list list;
typedef enum Type Type;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(Type type);
Token *evaluate(Node *node);
void visualize_variables(void);

enum Type
{
    zero_,
    // like none data type
    void_,
    // data types
    identifier_, characters_, boolean_, number_, array_, obj_, type_,
    // asssigns
    assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, mod_assign_,
    // statements
    while_, if_, elif_, else_,
    // end statement
    dots_, comma_,
    // parents, brackets
    lparent_, rparent_, lbracket_, rbracket_, lcbracket_, rcbracket_,
    // built in functions
    output_, input_,
    // math operator
    add_, sub_, mul_, div_, mod_, less_than_, more_than_, less_than_or_equal_, more_than_or_equal_,
    // logic operator
    and_, or_, not_, equal_, not_equal_,
    // functions
    func_dec_, func_call_,
    // key words
    break_, continue_, return_,
    // attribute
    dot_, attribute_, att_type_, key_iter, index_iter,
    // built in attributes
    indexof_,
    // EOF
    eof_,
};

struct Token
{
    char *name;
    Type type;
    int column;
    int line;
    int level;
    union
    {
        // number
        struct {
            long double number;
            bool is_float;
        };
        // characters
        struct
        {
            char *characters;
            bool is_char;
        };
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
        };
        // params value
        struct
        {
            Node **params_head;
            Token **params;
            int params_len;
        };
        // block
        Node **bloc_head;
        // data type value
        Type value_type;
        // boolean value
        bool boolean;
    };
};

// alpha tokens, will be gotten as identifiers
// then I compare them to the values here to check if it's one of them to change its type
// else it will be stored as identifier
struct
{
    char *name;
    Type type;
} alpha_tokens[] = {
    // data types
    {"true", boolean_}, {"false", boolean_}, {"void", void_},
    // statements
    {"if", if_}, {"elif", elif_}, {"else", else_}, {"while", while_},
    // built in functions
    {"output", output_}, {"input", input_},
    // logic operators
    {"and", and_}, {"or", or_}, {"not", not_}, {"is", equal_}, {"isnot", not_equal_},
    // key words
    {"break", break_}, {"continue", continue_}, {"return", return_},
    // functions
    {"func", func_dec_},
    // func attributes
    {"indexof", indexof_},
    {0, 0},
};

struct
{
    char *name;
    Type type;
} operators_tokens[] = {
    // logic operators
    {"&&", and_}, {"||", or_}, {"!", not_}, {"==", equal_}, {"!=", not_equal_},
    // assignements
    {"+=", add_assign_}, {"-=", sub_assign_}, {"*=", mul_assign_}, {"/=", div_assign_},
    // math operators
    {"<=", less_than_or_equal_}, {">=", more_than_or_equal_}, {"<", less_than_},
    {">", more_than_}, {"=", assign_},
    // parents, brackets
    {"(", lparent_}, {")", rparent_}, {"[", lbracket_}, {"]", rbracket_},
    {"{", lcbracket_}, {"}", rcbracket_}, {",", comma_},
    // math operators
    {"+", add_}, {"-", sub_}, {"*", mul_}, {"/", div_},
    {"%", mod_}, {":", dots_}, {".", dot_}, {0, 0},
};

struct
{
    char *special;
    char replace;
} special_characters[] = {{"\\n", '\n'}, {"\\\"", '\"'}, {"\\\'", '\''}, {"\\\\", '\\'}, {0, 0}};

// character methods
int ft_isnum(int c) { return (c >= '0' && c <= '9'); }
int ft_isupper(int c) { return (c >= 'A' && c <= 'Z'); }
int ft_islower(int c) { return (c >= 'a' && c <= 'z'); }
int ft_isalpha(int c) { return (ft_islower(c) || ft_isupper(c)); }
int ft_isalphanum(int c) { return (ft_isalpha(c) || ft_isnum(c)); }
int to_lower(int c) { return ft_isupper(c) ? (c + 32) : ft_islower(c) ? (c) : (0); }
int to_upper(int c) { return ft_islower(c) ? (c - 32) : ft_isupper(c) ? (c): (0); }

// string methods
int ft_strlen(char *str){ int i = 0; while (str && str[i]) i++; return i;}
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
    size_t i = 0;
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
    size_t i = 0;
    while (s2[i] && s1[i] && (unsigned char)s1[i] == (unsigned char)s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
// ptr methods
int Value_len(Value **ptr) { int i = 0; while (ptr && ptr[i]) i++; return (i);}

// ft_printf
void ft_putchar(int fd, char c) { write(fd, &c, sizeof(char));}
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
    if (num < 10) ft_putchar(fd, num + '0');
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
        float_part *= 10;
        decimal_places--;
    }
    ft_putnbr(fd, int_part);
    if (decimal_places)
    {
        ft_putchar(fd, '.');
        ft_putnbr(fd, (long)round(float_part));
    }
    else is_float = false;
    return is_float;
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
            token->is_float = ft_putfloat(fd, token->number, 6);
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
            ft_putstr(fd, " { ");
            for (int i = 0; token->object && token->object[i]; i++)
            {
                ft_putstr(fd, token->keys[i]);
                ft_putstr(fd, ": ");
                output(token->object[i]);
                if (token->object[i + 1])
                    ft_putstr(fd, ",");
            }
            ft_putstr(fd, " }");
            break;
        }
        default:
        {
            ft_putstr(err, "Error in output can't output ");
            ft_putstr(err, type_to_string(token->type));
            ft_putstr(err, "\n");
            exit(1);
        }
        }
    }
    else
        ft_putstr(fd, "NULL");
    ft_putstr(fd, "\033[0m");
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
                    ft_putnbr(fd, va_arg(ap, long));
                if (fmt[i] == 'f')
                {
                    ft_putfloat(fd, va_arg(ap, long double), 6);
                }
                if (fmt[i] == 'c')
                    ft_putchar(fd, va_arg(ap, int));
                if (fmt[i] == 's')
                    ft_putstr(fd, va_arg(ap, char *));
                if (fmt[i] == 't')
                    ft_putstr(fd, type_to_string(va_arg(ap, Type)));
                if (fmt[i] == 'k' || fmt[i] == 'v')
                {
                    Token *token = va_arg(ap, Token *);
                    if (token)
                    {
                        ft_putstr(fd, type_to_string(token->type));
                        if (token->name)
                            ft_printf(fd, " with name: %s, ", token->name);
                        else
                            ft_putchar(fd, ' ');
                        switch (token->type)
                        {
                        case characters_:
                        {
                            ft_putstr(fd, "value: ");
                            output(token);
                            break;
                        }
                        case number_:
                        {
                            ft_putstr(fd, "value: ");
                            output(token);
                            break;
                        }
                        case boolean_:
                        {
                            ft_putstr(fd, "value: ");
                            output(token);
                            break;
                        }
                        case array_:
                        {
                            ft_putstr(fd, "value: \n");
                            ft_putstr(fd, "        ");
                            output(token);
                            break;
                        }
                        case obj_:
                        {
                            ft_putstr(fd, "value: \n");
                            for (int i = 0; token->object && token->object[i]; i++)
                                ft_printf(fd, "         %s : %k\n", token->keys[i], token->object[i]);
                            ft_putstr(fd, "        ");
                            break;
                        }
                        case void_:
                        {
                            ft_putstr(fd, "no value\n");
                            break;
                        }
                        default:
                        {
                            if(type_to_string(token->type) == NULL)
                            {
                                ft_putstr(err, "ft_printf didn't know this token type: ");
                                ft_putstr(err, type_to_string(token->type));
                                exit(1);
                            }
                        }
                        }
                        if (token->line >= 0)
                            ft_printf(out, " in line [%d]", token->line);
                        ft_printf(out, ", in column [%d]", token->column);
                        ft_printf(out, ", in level [%d]", token->level);
                    }
                    else
                        ft_putstr(fd, "(null token)");
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

// parsing functions
char *type_to_string(Type type)
{
    struct
    {
        char *string;
        Type type;
    } Types[] = {
        // data types
        {"IDENTIFIER", identifier_}, {"CHARACTERS", characters_}, {"BOOLEAN", boolean_},
        {"NUMBER", number_}, {"ARRAY", array_}, {"OBJ", obj_}, {"TYPE", type_},{"VOID", void_},
        // assigns
        {"ASSIGNEMENT", assign_}, {"ADD ASSIGN", add_assign_}, {"SUB ASSIGN", sub_assign_},
        {"MUL ASSIGN", mul_assign_}, {"DIV ASSIGN", div_assign_}, {"MOD ASSIGN", mod_assign_},
        // statements
        {"WHILE", while_}, {"IF", if_}, {"ELIF", elif_}, {"ELSE", else_},
        // end statements
        {"DOTS", dots_}, {"COMMA", comma_},
        // parents,  brackets
        {"LEFT PARENT", lparent_}, {"RIGHT PARENT", rparent_}, {"LEFT BRACKET", lbracket_}, 
        {"RIGHT BRACKET", rbracket_}, {"LEFT CURLY BRACKET", lcbracket_}, {"RIGHT CURLY BRACKET", rcbracket_},
        // built in functions
        {"INPUT", input_}, {"OUTPUT", output_},
        // math operators
        {"ADDITION", add_}, {"SUBSTRACTION", sub_}, {"MULTIPLACTION", mul_},
        {"DIVISION", div_}, {"MODULO", mod_}, {"MORE THAN", more_than_}, {"LESS THAN", less_than_},
        {"MORE THAN OR EQUAL", more_than_or_equal_}, {"LESS THAN OR EQUAL", less_than_or_equal_},
        // logic operators
        {"AND", and_}, {"OR", or_}, {"NOT", not_},
        {"COMPARE", equal_}, {"NOT EQUAL", not_equal_},
        // functions
        {"FUNCTION DECLARATION", func_dec_}, {"FUNCTION CALL", func_call_},
        // key words
        {"BREAK", break_}, {"CONTINUE", continue_}, {"RETURN", return_},
        // attributes
        {"DOT", dot_}, {"ATTRIBUTE NODE", attribute_} ,{"ATTRIBUTE TYPE", att_type_},
        // built in attributes
        {"indexof", indexof_},
        // EOF
        {"EOF", eof_},
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

// tokenizing
char *text;
int txt_pos;
Token *tokens[500];
int tk_pos;
int line = 1;
int column;
int start;
int level;

Token *new_token(Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->line = line;
    new->column = column;
    switch (type)
    {
    case identifier_:
    {
        type = 0;
        char *value = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(value, text + start, txt_pos - start);
        for (int i = 0; alpha_tokens[i].name; i++)
            if (ft_strcmp(value, alpha_tokens[i].name) == 0)
            {
                type = alpha_tokens[i].type;
                break;
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
        while (ft_isnum(text[start]))
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
        while (ft_isnum(text[start]))
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
            column = 0;
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
            ft_printf(out, "found tab\n");
            column++;
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
                column++;
            continue;
        }
        if (ft_isalpha(text[txt_pos]))
        {
            start = txt_pos;
            while (ft_isalphanum(text[txt_pos]) || text[txt_pos] == '_')
                txt_pos++;
            Token *token = new_token(identifier_);
            if(token->type == return_)
            {
                column++;
                // ft_printf(out, "found return  \n");
                // exit(0);
            }
            continue;
        }
        if (ft_isnum(text[txt_pos]))
        {
            start = txt_pos;
            while (ft_isnum(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
                txt_pos++;
            while (ft_isnum(text[txt_pos]))
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

// building Nodes
struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Token));
    new->token = token;
    ft_printf(out, "new node: %k\n", new->token);
    return (new);
}

int exe_pos;
void skip(Type type)
{
    if (tokens[exe_pos]->type != type)
        ft_printf(err, "error current type is '%t' and want to skip '%t in pos '%d''\n", tokens[exe_pos]->type, type, exe_pos);
    exe_pos++;
}

Node **bloc(int level); // bloc of code
Node *expr();           // expression
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
Node **bloc(int column) // expected level
{
    // exit(0);
    if (tokens[exe_pos]->column < column)
        ft_printf(err, "Expected tab\n");
    int len = 0;

    Node **list = calloc(len + 1, sizeof(Node *));
    list[len] = expr();
    len++;
    list = realloc(list, (len + 1) * sizeof(Node *));
    while (tokens[exe_pos]->type != eof_ && tokens[exe_pos]->column == column)
    {
        list[len] = expr();
        len++;
        list = realloc(list, (len + 1) * sizeof(Node *));
    }
    list[len] = NULL;
    return list;
}

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

Node *expr()
{
    if (tokens[exe_pos]->type == return_)
    {
        Node *node = new_node(tokens[exe_pos]);
        int ret_col = tokens[exe_pos]->column;
        skip(return_);
        node->token->bloc_head = bloc(ret_col + 1);
        return node;
    }
    return assign();
}
// = += -= *= /= %=
Node *assign()
{
    Node *left = conditions();
    if (check(tokens[exe_pos]->type, assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, mod_assign_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->left = left;
        ft_printf(out, "left type: %k\n", node->left->token);
        node->right = conditions();
        ft_printf(out, "right type: %k\n", node->right->token);
        return node;
    }

    return left;
}
// while, if, elif, else
Node *conditions()
{
    if (check(tokens[exe_pos]->type, if_, elif_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        int curr_level = tokens[exe_pos]->column;
        skip(tokens[exe_pos]->type);

        // left is condition
        node->left = logic_or();
        if (node->left == NULL)
            ft_printf(err, "Expected condition\n");
        skip(dots_);

        // if bloc to execute
        node->token->bloc_head = bloc(curr_level + 1);
        if (tokens[exe_pos]->type == elif_)
        {
            node->right = conditions();
            // edit level here of node->right
        }
        if (tokens[exe_pos]->type == else_)
        {
            node->right = new_node(tokens[exe_pos]);
            // edit level here of node->right
            skip(tokens[exe_pos]->type);
            skip(dots_);
            node->right->token->bloc_head = bloc(curr_level + 1);
        }
        return node;
    }
    else if (tokens[exe_pos]->type == while_)
    {
        Node *node = new_node(tokens[exe_pos]);
        int curr_level = tokens[exe_pos]->column;
        skip(tokens[exe_pos]->type);

        // left is condition
        node->left = logic_or();
        if (node->left == NULL)
            ft_printf(err, "Expected condition\n");
        skip(dots_);

        // while bloc to execute
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
        return node;
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
        return node;
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
        return node;
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
        return node;
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
        return node;
    }
    return left;
}
// * / %
Node *mul_div()
{
    Node *left = sign();
    while (check(tokens[exe_pos]->type, mul_, div_, mod_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->left = left;
        node->right = sign();
        return node;
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

Node *prime()
{
    // function declaration
    if (tokens[exe_pos]->type == func_dec_)
    {
        ft_printf(out, "found function declaration \n");
        skip(func_dec_);
        Node *node = new_node(tokens[exe_pos]);
        int curr_level = tokens[exe_pos]->column;
        skip(identifier_);
        node->token->type = func_dec_;

        // function parameters on left
        node->left = new_node(tokens[exe_pos]);
        skip(lparent_);
        int i = 0;
        Node **params = calloc(i + 1, sizeof(Node *));
        if (tokens[exe_pos]->type != rparent_) // enter if there is params to hold
        {
            params[i] = new_node(tokens[exe_pos]);
            skip(identifier_);
            i++;
            params = realloc(params, (i + 1) * sizeof(Node *));
            while (tokens[exe_pos]->type != rparent_ && tokens[exe_pos]->type != eof_)
            {
                skip(comma_);
                params[i] = new_node(tokens[exe_pos]);
                skip(identifier_);
                i++;
                params = realloc(params, (i + 1) * sizeof(Node *));
            }
        }
        params[i] = NULL;
        skip(rparent_);

        node->left->token->array_head = params;
        // expect :
        skip(dots_);
        // current is bloc of code to execute
        node->token->bloc_head = bloc(curr_level + 1);
        return node;
    }
    // parentheses
    if (tokens[exe_pos]->type == lparent_)
    {
        skip(lparent_);
        Node *node = expr();
        skip(rparent_);
        return node;
    }
    // we can't iterate over type_ number_ break_ continue_
    if (check(tokens[exe_pos]->type, type_, boolean_, number_, break_, continue_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        return node;
    }
    // we can iterate over string
    if (check(tokens[exe_pos]->type, characters_, 0))
    {
        Node *left = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        // it's possible to iterate over characters
        while(tokens[exe_pos]->type == dot_ || tokens[exe_pos]->type == lbracket_)
        {
            // ft_printf(err, "%t\n", tokens[exe_pos]->type);
            while (tokens[exe_pos]->type == lbracket_)
            {
                Node *node = new_node(tokens[exe_pos]);
                skip(lbracket_);
                node->left = left;      // element to iterate over it
                node->right = expr();  
                skip(rbracket_);
                node->token->type = attribute_;
                left = node;
            }
            while (tokens[exe_pos]->type == dot_)
            {
                Node *node = new_node(tokens[exe_pos]);
                skip(dot_);
                node->left = left;    // element to iterate over it
                node->right = expr();
                node->token->type = attribute_;
                left = node;
            }
        }
        return left;
    }
    // array
    if (tokens[exe_pos]->type == lbracket_)
    {
        Node *node = new_node(tokens[exe_pos]);
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
    // object, check if you can optimize it
    if (tokens[exe_pos]->type == lcbracket_)
    {
        Node *node = new_node(tokens[exe_pos]);
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
                skip(dots_); // expect dots :
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
    // output function
    if (tokens[exe_pos]->type == output_)
    {
        Node *node = new_node(tokens[exe_pos]);
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
    // indexof
    if (tokens[exe_pos]->type == indexof_)
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(indexof_);
        skip(lparent_);
        node->right = expr(); // maybe it shouldn't be expr 
        skip(rparent_);
        return node;
    }
   
    // identifier
    if(check(tokens[exe_pos]->type, identifier_))
    {
        Node *node = new_node(tokens[exe_pos]);
        Node *left = node;
        skip(identifier_);
        if (tokens[exe_pos]->type == lparent_)
        {
            node->token->type = func_call_;
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
            node->left->token->array_len = i;
            left = node;
        }
        
        // iterate over identifier than can be array or stirng in future
        // or iterate over function in case function call
        while(tokens[exe_pos]->type == dot_ || tokens[exe_pos]->type == lbracket_)
        {
            // ft_printf(err, "%t\n", tokens[exe_pos]->type);
            while (tokens[exe_pos]->type == lbracket_)
            {
                Node *node = new_node(tokens[exe_pos]);
                skip(lbracket_);
                node->left = left;      // element to iterate over it
                node->right = expr();
                node->right->token->type = att_type_;
                skip(rbracket_);
                node->token->type = attribute_;
                left = node;
            }
            while (tokens[exe_pos]->type == dot_)
            {
                Node *node = new_node(tokens[exe_pos]);
                skip(dot_);
                node->left = left;    // element to iterate over it
                node->right = expr();
                node->right->token->type = att_type_;
                node->token->type = attribute_;
                left = node;
            }
        }
        return left;
    }
    
    return NULL;
}

// Evaluate
// Variables, Functions
struct list
{
    int index;
    Token *variables[200]; // to be modify after
    list *next;
    list *prev;
};

list *current;
Node *FUNCTIONS[500];
int func_index;

void access_next_scoop()
{
    current->next = calloc(1 ,sizeof(list));
    list *tmp = current;
    current = current->next;
    current->prev = tmp;
    level++;   
}
// free memory here, buuuuut be carefull
void exit_current_scoop()
{
    // memset(current->variables, 0, 200 * sizeof(Token*)); // free it after
    if(current->prev == NULL)
        ft_printf(err, "Error in exiting level\n");
    current = current->prev;
    level--;
}

Token *new_variable(Token *var)
{
    ft_printf(out, "index: %d\n", current->index);
#if 0
    Token *new = var;
#else
    Token *new = calloc(1,sizeof(Token));
    memcpy(new, var, sizeof(Token));
#endif
    current->variables[current->index] = new;
    new->level = level;
    current->index++;
    return (new);
}

list *tmp;
Token *get_var(char *name)
{
    ft_printf(out, "get %s from level %d\n",name, level);
      visualize_variables();
    tmp = current;
    while(tmp)
    {
        for(int i = tmp->index - 1; i >= 0; i--)
        {
            if(ft_strcmp(tmp->variables[i]->name, name) == 0)
            {
                ft_printf(out, "found var %v\n",tmp->variables[i]);
                return tmp->variables[i];
            }
        }
        tmp = tmp->prev;
    }
    ft_printf(out, "not found\n");
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
        // ft_printf(out, "do assignement between type %t and type  %t\n", node->left->token->type, node->right->token->type);
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
        ft_printf(out, "do assignement between %v and %v\n", left, right);
        // if(right->type == return_)
        // {
        //     Node **ret_head = right->bloc_head;
        //     Value *ret = NULL;
        //     ft_printf(out, "Evaluate bloc in function\n");
        //     int j = 0;
        //     while(ret_head[j])
        //     {
        //         ret = evaluate(ret_head[j]);
        //         j++;
        //     }
        //     right = ret;
        //     ft_printf(out, "right now is %v\n", right);
        // }
        // skip this error for function because i can set global variable to idenitifer from params
        if (right->type == identifier_)
            ft_printf(err, "Error , Undeclared variable: %s\n", right->name);
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
            memcpy(left, right, sizeof(Value)); // to be changed after, change only for characters, I guess !!!
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
    // values and some keywords
    case number_:
    case characters_:
    case boolean_:
    case break_:
    case continue_:
    case indexof_:
    case att_type_:
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
    case mod_:
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
            if (node->token->type == mod_)
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
    case mod_assign_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
        // if(right->type == return_)
        // {
        //     Node **ret_head = right->bloc_head;
        //     Value *ret = NULL;
        //     ft_printf(out, "Evaluate bloc in funtion\n");
        //     int j = 0;
        //     while(ret_head[j])
        //     {
        //         ret = evaluate(ret_head[j]);
        //         j++;
        //     }
        //     right = ret;
        //     ft_printf(out, "right now is %v\n", right);
        // }
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
            if (node->token->type == mod_assign_)
                number = (long)left->number % (long)right->number;
            ret->is_float = left->is_float || right->is_float;
            if (node->token->type == mod_assign_)
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
        if (condition->boolean)
        {
            Node **bloc_head = node->token->bloc_head;
            int i = 0;
            while (bloc_head[i])
            {
#if 1
            if(bloc_head[i]->token->type == return_)    
            {
                ft_printf(out, "found return in if_ elif_\n");
                // exit(0);
                return bloc_head[i]->token;
            }
            ret = evaluate(bloc_head[i]);
#else
            ret = evaluate(bloc_head[i]);
            if(ret && ret->type == return_)
            {
                ft_printf(out ,"if, elif_ return %v\n", ret);
                return ret;
            }
#endif
                i++;
            }
        }
        else if (node->right)
        {
            return(evaluate(node->right)); // t obe veirfied after
        }
        ret = new_token(void_);
        ft_printf(out, "if return %v\n", ret);
        return (ret); // to be verified
    }
    case return_:
    {
        // ft_printf(err, "Evaluate return\n");
        return (node->token);
    }
    case else_:
    {
#if 1
        Node **bloc_head = node->token->bloc_head;
        Value *ret = NULL;
        int i = 0;
        while (bloc_head[i])
        {
#if 1
            if(bloc_head[i]->token->type == return_)    
            {
                return bloc_head[i]->token;
            }
            ret = evaluate(bloc_head[i]);
#else
            ret = evaluate(bloc_head[i]);
            if(ret && ret->type == return_)
            {
                ft_printf(out ,"else return %v\n", ret);
                return ret;
            }
#endif
            i++;
        }
#endif
        ret = new_token(void_);
        // ft_printf(err, "else return ...\n");
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
                if(bloc_head[i]->token->type == return_)    
                {
                    return bloc_head[i]->token;
                }
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
            // verify return_ it may cause problems or something
            output(evaluate(head[i]));
            i++;
        }
        return (node->token);
    }
    case func_dec_:
    {
        ft_printf(out, "eval func_dec\n");
        Node *func_dec = new_func(node);
        break;
    }
    case func_call_:
    {
        // build a copy for the full function
        ft_printf(out, "call function %s\n", node->token->name);

        // find function
        Node *existed_func = get_func(node->token->name);
        Node **existed_params = existed_func->left->token->array_head;
        Node **new_params = node->left->token->array_head;

        // access next scoop
        // evaluate params of original function, and set hem to those in call
        int i = 0;
        access_next_scoop();
        while (existed_params[i])
        {
            Token *value = evaluate(new_params[i]);
            value =  new_variable(value);
            value->name = existed_params[i]->token->name;
            i++;
        }
        visualize_variables();
        Node **bloc_head = existed_func->token->bloc_head;
        Value *ret = NULL;
        i = 0;
        while (bloc_head[i])
        {
#if 0
            if(bloc_head[i]->token->type == return_)    
            {
                return bloc_head[i]->token;
            }
            ret = evaluate(bloc_head[i]);
#else
            ret = evaluate(bloc_head[i]);
            if(ret && ret->type == return_)
            {
                Node **ret_head = ret->bloc_head;
                // Value *ret = NULL;
                ft_printf(out, "Evaluate bloc in function\n");
                int j = 0;
                while(ret_head[j])
                {
                    ret = evaluate(ret_head[j]);
                    j++;
                }
                // ft_printf(out ,"function return %v\n", ret);
                exit_current_scoop();
                return ret;
            }
#endif
            i++;
        }
        // visualize_variables();
        exit_current_scoop();
        ft_printf(out ,"-> function return %v\n", ret);
        ret = new_token(void_);
        return ret;
        break;
    }
    // handle indexof here
    case attribute_:
    {
        // len, isnumber, isalpha, indexof ...
        // it's better to check left type then check right attribute
        // iteration over characters and arrays
        // iteration with number
        // attribute with key (identifier)
        // or with characters !!!

        Value *left = evaluate(node->left);   // variable
        Value *right = evaluate(node->right);
        // if(right->number)
        ft_printf(out, "left: %k\nright: %k\n", left, right);
        // exit(0);
        char *key = NULL;
        int index = 0;
        Type type = void_;

        if(right->type == characters_)
        {
            key = right->characters;
            type = key_iter;
        }
        else if(right->type == att_type_)
        {
            key = right->name;
            type = key_iter;
        }
        else if(right->type == number_)
        {
            index = (long)right->number;
            type = index_iter;
        }
        else
        {
            ft_printf(err, "Error in iteration\n");
        }
        ft_printf(out, "left: %v\nright: %v\n", left, right);
        if (left->type == identifier_)
            ft_printf(err, "Undeclared variable '%s'\n", left->name);
        
        if(type == key_iter)
        {
            if (left->type == obj_)
            {
                int i = 0;
                while (left->keys && left->keys[i])
                {
                    if (strcmp(key, left->keys[i]) == 0)
                    {
                        Value *ret = calloc(1, sizeof(Value));
                        ft_printf(out, "iterate return %v\n", left->object[i]);
                        memcpy(ret, left->object[i], sizeof(Value));
                        return left;
                    }
                    i++;
                }
                ft_printf(err, "Error: %s has no attribute '%s'\n",left->name, right->name);
            }
            if (left->type == characters_)
            {
                int i = 0;
                if (ft_strcmp(key, "len") == 0)
                {
                    Value *ret = calloc(1, sizeof(Value));
                    ret->type = number_;
                    ret->number = (double)ft_strlen(left->characters);
                    return ret;
                }
                if (ft_strcmp(key, "toup") == 0)
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
                if (ft_strcmp(key, "tolow") == 0)
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
                if (ft_strcmp(key, "tonum") == 0)
                {
                    Value *ret = calloc(1, sizeof(Value));
                    ret->type = number_;
                    // to verify after
                    ret->number = atof(left->characters);
                    return ret;
                }
                if (ft_strcmp(key, "isup") == 0)
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
                if (ft_strcmp(key, "islow") == 0)
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
                if (ft_strcmp(key, "ischar") == 0)
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
                if (ft_strcmp(key, "isnum") == 0)
                {
                    Value *ret = calloc(1, sizeof(Value));
                    ret->type = boolean_;
                    ret->boolean = true;
                    while (left->characters && left->characters[i] && ft_isnum(left->characters[i]))
                        i++;
                    if (left->characters && left->characters[i] == '.')
                        i++;
                    while (left->characters && left->characters[i] && ft_isnum(left->characters[i]))
                        i++;
                    if (left->characters[i])
                        ret->boolean = false;
                    return ret;
                }
                ft_printf(err, "%s has no attribute %s\n", left->name, right->name);
            }
            if (left->type == array_)
            {
                if (ft_strcmp(key, "len") == 0)
                {
                    Value *ret = calloc(1, sizeof(Value));
                    ret->type = number_;
                    long i = 0;
                    while (left->array && left->array[i])
                       i++;
                    ret->number = i;
                    return ret;
                }
            }
            ft_printf(err, "%v has no attribute %s\n", left, key);
        }
        if(type == index_iter)
        {
            // exit(0);
            if (left->type == characters_)
            {
                long i = 0;
                Value *ret = calloc(1, sizeof(Value));
                ret->type = characters_;
                ret->is_char = true;
                if (i < ft_strlen(left->characters))
                {
                    // exit(0);
                    ret->characters = left->characters + index;
                    return ret;
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
        }
        break;
    }
    default:
        ft_printf(err, "Error: eval received unknown type '%t' \n", node->token->type);
    }
    return NULL;
}

// Debuging functions
void visualize_variables(void)
{
#if 1
    int i = 0;
    ft_printf(out, "\n\nvariables: \n");
    list *tmp = current;
    while(tmp->prev)
        tmp = tmp->prev;
    // var_level is changeable don't think about putting it here
    while (tmp)
    {
        ft_printf(out, "    level %d, has %d variables:\n", i, tmp->index);
        int j = 0;
        while (j < tmp->index)
        {
            ft_printf(out, "        %k\n", tmp->variables[j]);
            j++;
        }
        tmp = tmp->next;
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

// execute
void execute()
{
    current = calloc(1, sizeof(list));
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
    // exit(0);

#if 1
    line = -1;
    execute();
    if (DEBUG)
        visualize_variables();
#endif
}