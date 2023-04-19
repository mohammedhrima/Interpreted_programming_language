// c headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>

// typedefs
typedef struct Var Var;
typedef struct Node Node;
typedef struct Token Token;
typedef struct Value Value;

// for the stupid implicit declaration error
void ft_printf(int fd, char *fmt, ...);
char *type_to_string(int type);
Var *eval(Node *node);

// macros
#define in STDIN_FILENO
#define out STDOUT_FILENO
#define err STDERR_FILENO

// tokens
typedef enum
{
    // be carefulll before doing any change here !!!
    eof_,
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
    none_,
    add_,
    sub_,
    mul_,
    div_
} Type;
struct Token
{
    char *content;
    Type type;
};
// they expect space after keyword
Token statements_tokens[] = {
    {"if", if_},
    {"while", while_},
    {"or", or_},
    {"and", and_},
    {"func", function_},
    {"while", while_},
    {0, 0},
};
// they expect nothing
Token operator_tokens[] = {
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
    {0, 0},
};
// they expect '('
Token functions_tokens[] = {
    {"output", output_},
    {"input", input_},
    {0, 0},
};

struct Value
{
    union
    {
        double number; // type integer, float
        char *string;  // type characters
        bool boolean;  // type boolean
        // type array
        struct
        {
            Value *elems;
            int array_size;
        };
        // type object
        struct
        {
            char **names;
            Value *values;
            int object_size;
        };
    };
    Type type;
};

struct Var
{
    char *name;
    Value value;
    int curr_index;
    bool temporary;
    Type type;
};
struct Node
{
    Node *left;
    Node *right;
    Node *next;
    char *content;
    Type type;
};

// global variables
Token *tokens[1000];
int tk_pos;
char *text;
int txt_pos;
int line;
int var_index;
Var *variables[500];
int tmp_index;
Var *temporaries[500];

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
            if (fmt[i] == 'v')
            {
                Var *variable = va_arg(ap, Var *);
                if (variable)
                {
                    ft_putstr(fd, "name: ");
                    ft_putstr(fd, variable->name);
                    ft_putstr(fd, ", type: ");
                    ft_putstr(fd, type_to_string(variable->type));
                    ft_putstr(fd, ", index: ");
                    ft_putnbr(fd, (long)variable->curr_index);
                    ft_putstr(fd, ", value: '");
                    switch (variable->type)
                    {
                    case characters_:
                        ft_putstr(fd, variable->value.string);
                        break;
                    case integer_:
                        ft_putnbr(fd, variable->value.number);
                        break;
                    case float_:
                        ft_putfloat(fd, variable->value.number, 6);
                        break;
                    case boolean_:
                        if (variable->value.boolean)
                            ft_putstr(fd, "true");
                        else
                            ft_putstr(fd, "false");
                        break;
                    case array_:
                        // ft_putstr(out, "is array ");
                        ft_putstr(out, "[");
                        int i = 1;
                        while (variable[i].type != none_)
                        {
                            if (variable[i].type == integer_)
                                ft_putnbr(fd, variable[i].value.number);
                            else if (variable[i].type == float_)
                                ft_putfloat(fd, variable[i].value.number, 6);
                            else if (variable[i].type == characters_)
                                ft_putstr(fd, variable[i].value.string);
                            else
                            {
                                ft_putstr(fd, "verify type '");
                                ft_putstr(fd, type_to_string(variable[i].type));
                                ft_putstr(fd, "'\n");
                                exit(0);
                            }
                            i++;
                            if (variable[i].type != none_)
                                ft_putstr(fd, ", ");
                        }
                        ft_putstr(out, "]");
                        break;
                    default:
                        ft_putstr(err, "ft_printf didn't know variable type\n");
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
    while (variables[i])
    {
        Var *variable = variables[i];
        if (variable)
            ft_printf(out, "     %v\n", variable);
        i++;
    }
    i = 0;
    ft_printf(out, "temporary variables: \n");
    while (temporaries[i])
    {
        Var *temporary = temporaries[i];
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
    case eof_:
        return "EOF";
    case variable_:
        return "VARIBALE";
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
        ft_printf(err, "type not found in type_to_string\n", type);
    }
    return NULL;
}

// news functions
void new_token(int start, Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->content = calloc(txt_pos - start + 1, sizeof(char));
    ft_strncpy(new->content, text + start, txt_pos - start);
    new->type = type;
    ft_printf(out, "new token with value '%s' and type '%s', put in position '%d' \n", new->content, type_to_string(new->type), tk_pos);
    tokens[tk_pos++] = new;
}
Node *new_node(Type type, char *content)
{
    Node *new = calloc(1, sizeof(Node));
    new->type = type;
    new->content = content;
    return (new);
}
Var *new_var(char *name, Type type, char *content)
{
    ft_printf(out, "new var received name:'%s' value:'%s' type:'%s'\n", name, content, type_to_string(type));
    Var *new = calloc(1, sizeof(Var));
    new->name = name;
    new->curr_index = var_index;
    new->type = type;
    if (type == characters_)
        new->value.string = content;
    else if (type == integer_)
    {
        new->type = integer_;
        double res = 0;
        int i = 0;
        if (content)
        {
            while (ft_isdigit(content[i]))
            {
                res = 10 * res + (content[i] - '0');
                i++;
            }
            if (content[i] == '.')
            {
                new->type = float_;
                i++;
            }
            double pres = 0.1;
            while (ft_isdigit(content[i]))
            {
                res = res + pres * (content[i] - '0');
                pres /= 10;
                i++;
            }
        }
        new->value.number = res;
    }
    else
        ft_printf(err, "Unknown type in new_var s:'%s', d:'%d'\n", type_to_string(type), type);
    variables[var_index++] = new;
    return new;
}

Var *new_array(char *name, Node *node)
{
    Value head;
    Value *curr_val = &head;

    Node *curr = node->next;
    while (curr)
    {
        Var *v = eval(curr);
        curr_val->next = &v->value;
        curr = curr->next;
        curr_val = curr_val->next;
    }
    curr->next = new_var("", none_, "");
    Var *v = calloc(1, sizeof(Var));
    v->value = *head.next;
    v->name = name;
    v->type = array_;
    v->curr_index = var_index;
    variables[var_index++] = v;
    return v;
}

// protect it from unkown chracters
void build_tokens()
{
    int start = 0;
    while (text[txt_pos])
    {
        while (ft_isspace(text[txt_pos]))
            txt_pos++;
        if (text[txt_pos] == '\0')
            break;
        int type = 0;
        start = txt_pos;
        for (int i = 0; statements_tokens[i].type; i++)
        {
            if (ft_strncmp(statements_tokens[i].content, text + txt_pos, ft_strlen(statements_tokens[i].content)) == 0 && ft_isspace(text[txt_pos + ft_strlen(statements_tokens[i].content)]))
            {
                type = statements_tokens[i].type;
                txt_pos += ft_strlen(statements_tokens[i].content);
                break;
            }
        }
        if (type)
        {
            new_token(start, type);
            continue;
        }
        start = txt_pos;
        for (int i = 0; operator_tokens[i].type; i++)
        {
            if (ft_strncmp(operator_tokens[i].content, text + txt_pos, ft_strlen(operator_tokens[i].content)) == 0)
            {
                type = operator_tokens[i].type;
                txt_pos += ft_strlen(operator_tokens[i].content);
                break;
            }
        }
        if (type)
        {
            new_token(start, type);
            continue;
        }
        start = txt_pos;
        for (int i = 0; functions_tokens[i].type; i++)
        {
            if (ft_strncmp(functions_tokens[i].content, text + txt_pos, ft_strlen(functions_tokens[i].content)) == 0)
            {
                type = functions_tokens[i].type;
                txt_pos += ft_strlen(functions_tokens[i].content);
                break;
            }
        }
        if (type)
        {
            new_token(start, type);
            continue;
        }
        if (ft_isalpha(text[txt_pos]))
        {
            start = txt_pos;
            while (ft_isalnum(text[txt_pos]))
                txt_pos++;
            new_token(start, variable_);
            continue;
        }
        else if (ft_isdigit(text[txt_pos]))
        {
            start = txt_pos;
            Type number_type = integer_;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
            {
                number_type = float_;
                txt_pos++;
            }
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            new_token(start, number_type);
            continue;
        }
        else if (text[txt_pos] == '"' || text[txt_pos] == '\'')
        {
            int left_coat = text[txt_pos];
            start = ++txt_pos;
            while (text[txt_pos] && text[txt_pos] != left_coat)
                txt_pos++;
            if (text[txt_pos] != left_coat)
                ft_printf(err, "expcted '%c'\n", left_coat);
            new_token(start, characters_);
            txt_pos++;
            continue;
        }
        else
            ft_printf(err, "unknown value s:'%s', c:'%c', d:'%d'\n", text + txt_pos, text[txt_pos], text[txt_pos]);
    }
    new_token(txt_pos, eof_);
}

// build nodes
Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();

void expect(Type c)
{
    if (tokens[tk_pos]->type != c)
        ft_printf(err, "Syntax error, expecting '%s'\n", type_to_string(c));
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
        Node *node = calloc(1, sizeof(Node));
        node->type = assign_;
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
        // Node node = new_node();
        Node *node = calloc(1, sizeof(Node));
        node->type = tokens[tk_pos]->type;
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
        Node *node = calloc(1, sizeof(Node));
        node->type = tokens[tk_pos]->type;
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
    if (tokens[tk_pos]->type == lbracket_)
    {
        // skip first bracket
        Node *array = new_node(array_, NULL);
        Node *curr = array;
        tk_pos++;
        while (tokens[tk_pos]->type != rbracket_)
        {
            curr->next = add_sub();
            if (tokens[tk_pos]->type == comma_)
                tk_pos++;
            else if (tokens[tk_pos]->type != rbracket_)
            {
                ft_printf(err, "Expected ']' in tk_pos: '%d'\n", tk_pos);
                break;
            }
        }
        tk_pos++;
        // skip second bracket
        left = array;
    }
    else if (tokens[tk_pos]->type == variable_ || tokens[tk_pos]->type == integer_ || tokens[tk_pos]->type == float_ || tokens[tk_pos]->type == characters_ || tokens[tk_pos]->type == comma_)
    {
        left = new_node(tokens[tk_pos]->type, tokens[tk_pos]->content);
        tk_pos++;
    }
    return left;
}

Var *eval(Node *node)
{
    Var *res = NULL;
    ft_printf(out, "node type in eval '%s'\n", type_to_string(node->type));
    if (node->type == integer_ || node->type == float_ || node->type == characters_)
        return new_var("", node->type, node->content);
    else if (node->type == add_ || node->type == sub_)
    {
        Var *left = eval(node->left);
        Var *right = eval(node->right);
        if (left->type != right->type && (left->type != integer_ && right->type != integer_))
            ft_printf(err, "can't '%s' different data type '%s' and '%s' \n", type_to_string(node->type), type_to_string(left->type), type_to_string(right->type));
        if (left->type == integer_ || left->type == float_)
        {
            Type type = (left->type == float_ || right->type == float_) ? float_ : integer_;
            res = new_var("", type, NULL);
            if (node->type == add_)
                res->value.number = left->value.number + right->value.number;
            if (node->type == sub_)
                res->value.number = left->value.number - right->value.number;
        }
        else if (left->type == characters_ && node->type == add_)
        {
            res = new_var("", left->type, NULL);
            res->value.string = calloc(ft_strlen(left->value.string) + ft_strlen(right->value.string) + 1, sizeof(char));
            ft_strcpy(res->value.string, left->value.string);
            ft_strcpy(res->value.string + ft_strlen(res->value.string), right->value.string);
        }
        else
            ft_printf(err, "Error in eval in '%s'\n", type_to_string(node->type));
    }
    else if (node->type == mul_ || node->type == div_)
    {
        Var *left = eval(node->left);
        Var *right = eval(node->right);
        if (left->type != right->type && (left->type != integer_ && right->type != integer_))
            ft_printf(err, "can't '%s' different data type '%s' and '%s' \n", type_to_string(node->type), type_to_string(left->type), type_to_string(right->type));
        if (left->type == integer_ || left->type == float_)
        {
            Type type = (left->type == float_ || right->type == float_) ? float_ : integer_;
            res = new_var("", type, NULL);
            if (node->type == mul_)
                res->value.number = left->value.number * right->value.number;
            if (node->type == div_)
                res->value.number = left->value.number / right->value.number;
        }
        else
            ft_printf(err, "Error in eval in '%s'\n", type_to_string(node->type));
    }
    return res;
}

void execute()
{
    Node *curr = expr();
    /*
    important, in assign:
        left should be always variable
        right should be a data type
    */
    if (curr->type == assign_)
    {
        if (curr->right->type != array_)
        {
            ft_printf(out, "do '%s', between '%s' and '%s'\n", type_to_string(curr->type), curr->left->content, type_to_string(curr->right->type));
            // start assigning

            if (curr->type == assign_)
            {
                if (curr->right->type >= variable_ && curr->right->type <= float_)
                    new_var(curr->left->content, curr->right->type, curr->right->content);
                if (curr->right->type >= add_ && curr->right->type <= div_)
                {
                    // create new variabale type float_
                    // calculate result
                    // set the new variable the new result
                    Var *res = eval(curr->right);
                }
            }
        }
        else
        {
            ft_printf(out, "do '%s', between '%s' and \n", type_to_string(curr->type), curr->left->content);
            new_array(curr->left->content, curr->right);
        }
    }
    else
        ft_printf(err, "Unknown Node type in execute\n");
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
    // print text
    ft_printf(out, "%s\n", text);
    indexes();
    build_tokens();
    tk_pos = 0;
    execute();
    visualize_variables();
}