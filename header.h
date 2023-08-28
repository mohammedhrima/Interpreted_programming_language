#ifndef MINI_HEADER_H
#define MINI_HEADER_H

#include "Libft/libft.h"
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
#define DEBUG 0

// typedefs
typedef struct Node Node;
typedef struct Token Token;
typedef struct Token Value;
typedef struct List List;
typedef enum Type Type;

// for the stupid implicit declaration error
char *type_to_string(Type type);
Token *evaluate(Node *node);
void visualize_variables(void);
void undeclared_error(char *location, Token *token, char *type);
void see_token(Token *token);
void build_tokens();
Token *new_token(Type type);
long pow_ten(long exponent);
void output(Token *token);
bool check(Type to_find, ...);
void visualize_variables(void);

Node **bloc();      // bloc of code
Node *expr();       // expression
Node *assign();     // = += -= *= /=
Node *conditions(); // while, if, elif, else
Node *logic_or();   // || or
Node *logic_and();  // && and
Node *equality();   // ==  !=
Node *comparison(); // < > <= >=
Node *add_sub();    // + -
Node *mul_div();    // * /
Node *sign();       // sign  -
Node *iteration();  // .len ... [0]
Node *prime();      // primary
Node *parents();    // ()

enum Type
{
    zero_,
    // like none data type
    void_,
    // data types
    identifier_,
    characters_,
    boolean_,
    number_,
    array_,
    obj_,
    tuple_,
    // asssigns
    assign_,
    add_assign_,
    sub_assign_,
    mul_assign_,
    div_assign_,
    mod_assign_,
    // statements
    while_,
    if_,
    elif_,
    else_,
    for_,
    in_,
    // end statement
    dots_,
    comma_,
    // parents, brackets
    lparent_,
    rparent_,
    lbracket_,
    rbracket_,
    lcbracket_,
    rcbracket_,
    // built in functions
    output_,
    input_,
    range_,
    // math operator
    add_,
    sub_,
    mul_,
    div_,
    mod_,
    less_than_,
    more_than_,
    less_than_or_equal_,
    more_than_or_equal_,
    // logic operator
    and_,
    or_,
    not_,
    equal_,
    not_equal_,
    // functions
    func_dec_,
    func_call_,
    // key words
    break_,
    continue_,
    return_,
    // attribute
    dot_,
    attribute_,
    att_type_,
    key_iter,
    index_iter,
    // built in attributes
    indexof_,
    count_,
    split_,
    trim_,
    base_,
    starts_with_,
    ends_with_,
    // EOF
    eof_,
};

struct Token
{
    char *name;
    Type type;
    // positions
    int line;   // line where the var exists
    int column; // column by index
    int tab;    // by tabs
    int txt_pos;
    union
    {
        // number
        struct
        {
            long long number;
            int exponent;
            // bool is_float;
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

typedef struct
{
    char *name;
    Type type;
} Typed_token;

typedef struct
{
    char *special;
    char replace;
} Special_characters;

struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

struct List
{
    int val_index;
    Token *variables[5000]; // to be modify after
    List *next;
    List *prev;
};

// globals
extern Special_characters *special_characters;
extern Typed_token *operators_tokens;
extern Typed_token *alpha_tokens;
extern char *text;

// text
extern char *text;
extern int txt_pos;

// tokens
extern Token **tokens;
extern int tk_len;
extern int tk_pos;

// position in file
extern int line;
extern int column;
extern int tab;
extern int start;
extern int exe_pos;
extern char *tab_space;
extern List *current;
extern Node *FUNCTIONS[5000]; // to be modified after
extern int func_index;
extern int scoop;

#endif