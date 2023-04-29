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
typedef struct Value Value;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(int type);
Value *eval(Node *node);

typedef enum
{
    eof_,
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
    tab_,
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
    div_
} Type;

struct Value
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
            Value *array;
            int array_len;
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

struct Token
{
    Value value;
    Type type;
};

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
    {"end ", end_statement_},
    {0, 0},
};

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
int tmp_pos;
Value *VARIABLES[500];
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
    // print_space(out, 3 - (int)len_of_num((long)line), '0');
    // ft_putnbr(fd, line);
    // ft_putstr(fd, "| ");
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
                    switch (token->type)
                    {
                    case identifier_:
                        ft_putstr(fd, " with name: '");
                        ft_putstr(fd, token->value.name);
                        ft_putstr(fd, "'");
                        break;
                    case characters_:
                    case character_:
                        ft_putstr(fd, " with value: '");
                        ft_putstr(fd, token->value.characters);
                        ft_putstr(fd, "'");
                        break;
                    case integer_:
                        ft_putstr(fd, " with value: '");
                        ft_putfloat(fd, token->value.number, 0);
                        ft_putstr(fd, "'");
                        break;
                    case float_:
                        ft_putstr(fd, " with value: '");
                        ft_putfloat(fd, token->value.number, 6);
                        ft_putstr(fd, "'");
                        break;
                    case boolean_:
                        ft_putstr(fd, " with value: '");
                        if (token->value.boolean)
                            ft_putstr(fd, "true");
                        else
                            ft_putstr(fd, "false");
                        break;
                    case array_:
                        ft_putstr(fd, "with value: '");
                        for (int i = 0; i < token->value.array_len; i++)
                        {
                            ft_putstr(fd, "\n     ");
                            ft_printf(fd, "%v", token->value.array[i]);
                        }
                        ft_putstr(fd, "\n     with size: ");
                        ft_putnbr(fd, token->value.array_len);
                        break;
                    case assign_:
                    case equal_:
                    case new_line_:
                    case if_:
                    case end_statement_:
                    case dots_:
                    case eof_:
                    case more_than_:
                    case sub_:
                    case add_:
                    case lbracket_:
                    case rbracket_:
                    case lcbracket_:
                    case rcbracket_:
                    case comma_:
                    case mul_:
                        break;
                    default:
                        ft_putstr(err, "ft_printf didn't know this token type '");
                        ft_putstr(err, type_to_string(token->type));
                        ft_putstr(err, "'\n");
                        exit(0);
                    }
                    ft_putstr(fd, " in pos ");
                    ft_putnbr(fd, token->value.index);
                    ft_putstr(fd, "'");
                }
                else
                    ft_putstr(fd, "(null token)");
            }
            if (fmt[i] == 'v')
            {
                Value var = va_arg(ap, Value);

                ft_putstr(fd, "name: ");
                ft_putstr(fd, var.name);
                ft_putstr(fd, ", type: ");
                ft_putstr(fd, type_to_string(var.type));
                ft_putstr(fd, ", index: ");
                ft_putnbr(fd, (long)var.index);
                ft_putstr(fd, ", value: '");
                switch (var.type)
                {
                case identifier_:
                    break;
                case characters_:
                case character_:
                    ft_putstr(fd, var.characters);
                    break;
                case integer_:
                    ft_putfloat(fd, var.number, 0);
                    break;
                case float_:
                    ft_putfloat(fd, var.number, 6);
                    break;
                case boolean_:
                    if (var.boolean)
                        ft_putstr(fd, "true");
                    else
                        ft_putstr(fd, "false");
                    break;
                case array_:
                    for (int i = 0; i < var.array_len; i++)
                    {
                        ft_putstr(fd, "\n       fgafw        ");
                        ft_printf(fd, "%v", var.array[i]);

                        // exit(0);
                    }
                    ft_putstr(fd, "\n               with size: ");
                    ft_putnbr(fd, var.array_len);
                    ft_putstr(fd,"\n");
                    break;
                default:
                    ft_putstr(err, "ft_printf didn't know this variable type '");
                    ft_putstr(err, type_to_string(var.type));
                    ft_putstr(err, "'\n");
                    exit(0);
                }
                ft_putstr(fd, "'");
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
            ft_printf(out, "     %v\n", *VARIABLES[i]);
        i++;
    }
    ft_printf(out, "\n");
}
// parsing functions
char *type_to_string(int type)
{
    switch (type)
    {
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
    case lcbracket_:
        return "LEFT CURLY BRACKET";
    case rcbracket_:
        return "RIGHT CURLY BRACKET";
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
    case end_statement_:
        return "END STATEMENT";
    case dots_:
        return "DOTS";
    case new_line_:
        return "NEW LINE";
    case eof_:
        return "EOF";
    default:
        ft_putstr(err, "error not found in type_to_string '");
        ft_putnbr(err, type);
        ft_putstr(err, "'\n");
        exit(0);
    }
    return NULL;
}

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
        ft_printf(out, "new token %k\n", new);
        break;
    case characters_:
        new->value.type = type;
        new->value.characters = calloc(txt_pos - start + 1, sizeof(char));
        ft_strncpy(new->value.characters, text + start, txt_pos - start);
        ft_printf(out, "new token %k\n", new);
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
        ft_printf(out, "new token %k\n", new);
        break;
    case boolean_:
        new->value.type = type;
        ft_printf(out, "new token %k\n", new);
        break;
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
        ft_printf(out, "new token %k\n", new);
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
        int isboolean = ft_strcmp(text + txt_pos, "true");
        if (isboolean == 0 || isboolean == ' ' || isboolean == '\n')
        {
            token = new_token(boolean_);
            token->value.boolean = true;
            txt_pos += ft_strlen("true");
            continue;
        }
        isboolean = ft_strcmp(text + txt_pos, "false");
        if (isboolean == 0 || isboolean == ' ' || isboolean == '\n')
        {
            token = new_token(boolean_);
            token->value.boolean = false;
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
}

// build nodes
Node *expr();    //
Node *assign();  // =
Node *add_sub(); // + -
Node *mul_div(); // * /
Node *unary();   // sign + -
Node *prime();   // final value

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Token));
    new->token = token;
    ft_printf(out, "new node '%k'\n", new->token);
    return (new);
}

Value *new_array_node(int len)
{
    Value *new = calloc(1, sizeof(Value));
    new->type = array_;
    new->array = calloc(len + 2, sizeof(Value));
    int j = 0;
    tk_pos = start;
    while (tokens[tk_pos]->type != rbracket_)
    {
        new->array[j] = *eval(expr());
        if (tokens[tk_pos]->type == comma_)
            tk_pos++;
        ft_printf(out, "new[%d]: %v\n", j, new->array[j]);
        j++;
    }
    new->array_len = j;
    ft_printf(out, "%d\n", new->array_len);
    return (new);
}

void skip(Type type)
{
    if (tokens[tk_pos]->type != type)
        ft_printf(err, "error current type is '%t' and want to skip '%t'\n", tokens[tk_pos]->type, type);
    tk_pos++;
}

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
        minus->value.number = -1;
        minus->value.index = tmp_pos++;
        // mul token
        tokens[tk_pos]->type = mul_;
        Node *node = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        node->left = new_node(minus);
        node->right = prime();
        return (node);
    }
    return prime();
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
        // tokens[tk_pos]->type == array_ ||
        tokens[tk_pos]->type == end_statement_)
    {
        left = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
    }
    else if (tokens[tk_pos]->type == lbracket_)
    {
        tokens[tk_pos]->type = array_;
        left = new_node(tokens[tk_pos]);
        skip(tokens[tk_pos]->type);
        int len = 0;
        start = tk_pos;
        while (tokens[tk_pos] && tokens[tk_pos]->type != rbracket_)
        {
            // ft_printf(out, "type: %t\n", tokens[tk_pos]->type);
            expr();
            len++;
            if (tokens[tk_pos]->type != rbracket_)
                skip(comma_);
        }
        ft_printf(out, "start: %d, tk_pos: %d\n", start, tk_pos);
        Value *value = new_array_node(len);
        left->token->value.array = value->array;
        left->token->value.array_len = value->array_len;
        skip(rbracket_);
    }
    return left;
}

// evaluate
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
    VARIABLES[var_index++] = var;
    return (var);
}
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
        Value *right = eval(node->right);
        char *name = left->name;
        // ft_printf(out, "left: %v\n", *left);
        // ft_printf(out, "right: %v\n", *right);
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
    case boolean_:
    {
        ft_printf(out, "return %v\n", node->token->value);
        return (&node->token->value);
    }
    // operator
    case add_:
    {
        node->token->type = integer_;
        node->token->value.type = integer_;
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        node->token->value.number = left->number + right->number;
        return (&node->token->value);
    }
    case mul_:
    {
        node->token->type = integer_;
        node->token->value.type = integer_;
        Value *left = eval(node->left);
        Value *right = eval(node->right);
        node->token->value.number = left->number * right->number;
        return (&node->token->value);
    }
    default:
        ft_printf(err, "receive unkonwn type '%t' \n", node->token->type);
    }
    return NULL;
}

// execute
void execute()
{
    Node *curr = expr();
    while (curr && curr->token->type != eof_)
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
    build_tokens();
    tmp_pos = tk_pos;
    tk_pos = 0;
    execute();
    visualize_variables();
}