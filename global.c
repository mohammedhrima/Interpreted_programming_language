#include "header.h"

// alpha tokens, will be gotten as identifiers
// then I compare them to the values here to check if it's one of them to change its type
// else it will be stored as identifier
Typed_token *alpha_tokens = (Typed_token[]){
    // data types
    {"true", boolean_},
    {"false", boolean_},
    {"void", void_},
    // statements
    {"if", if_},
    {"elif", elif_},
    {"else", else_},
    {"while", while_},
    {"for", for_},
    {"in", in_},
    // built in functions
    {"output", output_},
    {"input", input_},
    {"range", range_},
    // logic operators
    {"and", and_},
    {"or", or_},
    {"not", not_},
    {"is", equal_},
    {"isnot", not_equal_},
    // key words
    {"break", break_},
    {"continue", continue_},
    {"return", return_},
    // functions
    {"func", func_dec_},
    // func attributes
    {"indexof", indexof_},
    {"count", count_},
    {"split", split_},
    {"trim", trim_},
    {"base", base_},
    {"startswith", starts_with_},
    {"endswith", ends_with_},
    {0, 0},
};

Typed_token *operators_tokens = (Typed_token[]){
    // logic operators
    {"&&", and_},
    {"||", or_},
    {"==", equal_},
    {"!=", not_equal_},
    {"!", not_},
    // assignements
    {"+=", add_assign_},
    {"-=", sub_assign_},
    {"*=", mul_assign_},
    {"/=", div_assign_},
    // math operators
    {"<=", less_than_or_equal_},
    {">=", more_than_or_equal_},
    {"<", less_than_},
    {">", more_than_},
    {"=", assign_},
    // parents, brackets
    {"(", lparent_},
    {")", rparent_},
    {"[", lbracket_},
    {"]", rbracket_},
    {"{", lcbracket_},
    {"}", rcbracket_},
    {",", comma_},
    // math operators
    {"+", add_},
    {"-", sub_},
    {"*", mul_},
    {"/", div_},
    {"%", mod_},
    {":", dots_},
    {".", dot_},
    {0, 0},
};

Special_characters *special_characters = (Special_characters[]){
    {"\\n", '\n'},
    {"\\\"", '\"'},
    {"\\\'", '\''},
    {"\\\\", '\\'},
    {0, 0},
};
