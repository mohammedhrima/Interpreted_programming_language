#include "header.h"

// building Nodes

Node *new_node(Token *token)
{
    Node *new = ft_calloc(1, sizeof(Token));
    new->token = token;
    see_token(new->token);
    return (new);
}

void skip(Type type)
{
    if (tokens[exe_pos]->type != type)
    {
        // try getting back then print all those who have the same line attribute;
        ft_putchar(out, '\n');
        txt_pos = tokens[exe_pos - 1]->txt_pos;
        while (txt_pos > 0 && text[txt_pos - 1] != '\n')
            txt_pos--;
        while (text[txt_pos] && text[txt_pos] != '\n')
        {
            ft_putchar(out, text[txt_pos]);
            txt_pos++;
        }
        ft_putchar(out, '\n');
        print_space(tokens[exe_pos - 1]->column);
        ft_putstr(out, "^\n");
        ft_fprintf(err, "syntax error: Expected '%t' in line '%d'\n", type, tokens[exe_pos - 1]->line);
    }
    exe_pos++;
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

// bloc
Node **bloc(int tab)
{
    if (tokens[exe_pos]->tab < tab)
        ft_fprintf(err, "syntax error: Expected %d tab in line '%d'\n", tokens[exe_pos - 1]->tab + 1, tokens[exe_pos]->line);
    int len = 0;
    Node **list = ft_calloc(len + 1, sizeof(Node *));
    list[len] = expr();
    len++;
    list = ft_realloc(list, (len) * sizeof(Node *), (len + 1) * sizeof(Node *));
    while (tokens[exe_pos]->type != eof_ && tokens[exe_pos]->tab == tab)
    {
        list[len] = expr();
        len++;
        list = ft_realloc(list, (len) * sizeof(Node *), (len + 1) * sizeof(Node *));
    }
    list[len] = NULL;
    return list;
}
// expression
Node *expr()
{
    if (check(tokens[exe_pos]->type, return_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        node->token->bloc_head = bloc(node->token->tab + 1);
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
        node->right = conditions();
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
        skip(tokens[exe_pos]->type);

        // left is condition
        node->left = logic_or();
        if (node->left == NULL)
            ft_fprintf(err, "Expected condition\n");
        skip(dots_);

        // if bloc to execute
        node->token->bloc_head = bloc(node->token->tab + 1);
        if (check(tokens[exe_pos]->type, elif_, 0))
        {
            node->right = conditions();
            // edit level here of node->right
        }
        if (check(tokens[exe_pos]->type, else_, 0))
        {
            node->right = new_node(tokens[exe_pos]);
            // edit level here of node->right
            skip(tokens[exe_pos]->type);
            skip(dots_);
            node->right->token->bloc_head = bloc(node->token->tab + 1);
        }
        return node;
    }
    else if (check(tokens[exe_pos]->type, while_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);

        // left is condition
        node->left = logic_or();
        if (node->left == NULL)
            ft_fprintf(err, "Expected condition\n");
        skip(dots_);

        // while bloc to execute
        node->token->bloc_head = bloc(node->token->tab + 1);
        return node;
    }
    else if (check(tokens[exe_pos]->type, for_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);

        // left is condition
        node->left = prime();
        if (node->left->token->type != identifier_)
            ft_fprintf(err, "can't loop over %s\n", type_to_string(node->left->token->type));
        skip(in_);
        // array to iterate over it
        node->right = prime();
        skip(dots_);
        // bloc to execute
        node->token->bloc_head = bloc(node->token->tab + 1);
        return node;
    }
    return logic_or();
}

// || or
Node *logic_or()
{
    Node *left = logic_and();
    while (check(tokens[exe_pos]->type, or_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
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
    while (check(tokens[exe_pos]->type, and_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
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
    while (check(tokens[exe_pos]->type, equal_, not_equal_, 0))
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
    while (check(tokens[exe_pos]->type, less_than_, more_than_, less_than_or_equal_, more_than_or_equal_, 0))
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
    while (check(tokens[exe_pos]->type, mul_, div_, mod_, 0))
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
        skip(tokens[exe_pos]->type);
        node->token->type = mul_;
        node->left = new_node(new_token(number_));
        node->left->token->number = -1;
        node->right = iteration();
        return node;
    }
    return iteration();
}

// iteration
Node *iteration()
{
    Node *left = prime();
    if (check(left->token->type, identifier_, characters_, func_call_, input_, 0) && (tokens[exe_pos]->type == lbracket_ || tokens[exe_pos]->type == dot_))
    {
        while (tokens[exe_pos]->type == dot_ || tokens[exe_pos]->type == lbracket_)
        {
            // ft_printf(err, "%t\n", tokens[exe_pos]->type);
            while (tokens[exe_pos]->type == lbracket_)
            {
                Node *node = new_node(tokens[exe_pos]);
                skip(lbracket_);
                node->left = left; // element to iterate over it
                node->right = expr();
                // if(tokens[exe_pos]->type == dots_)
                // node->right->token->type = att_type_; // it get declared as identifier
                skip(rbracket_);
                node->token->type = attribute_;
                left = node;
            }
            while (tokens[exe_pos]->type == dot_)
            {
                Node *node = new_node(tokens[exe_pos]);
                skip(dot_);
                node->left = left; // element to iterate over it
                node->right = prime();
                if (node->right->token->type == identifier_)
                    node->right->token->type = att_type_; // it get decared sometimes as identifier
                node->token->type = attribute_;
                left = node;
            }
        }
    }
    return left;
}

// primary
Node *prime()
{
    if (check(tokens[exe_pos]->type, lparent_, 0))
    {
        skip(lparent_);
        Node *node = expr();
        skip(rparent_);
        return node;
    }
    // function declaration
    if (check(tokens[exe_pos]->type, func_dec_, 0))
    {
        // ft_fprintf(out, "found function declaration \n");
        skip(func_dec_);
        Node *node = new_node(tokens[exe_pos]);
        skip(identifier_);
        node->token->type = func_dec_;
        // function parameters on left
        skip(lparent_);
        exe_pos--;
        // parameters
        node->left = parents(prime, identifier_, lparent_, rparent_);
        // expect :
        skip(dots_);
        // current is bloc of code to execute
        node->token->bloc_head = bloc(node->token->tab + 1);
        return node;
    }
    // we can't iterate over type_ number_ break_ continue_
    if (check(tokens[exe_pos]->type, boolean_, number_, break_, continue_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        return node;
    }
    // we can iterate over string
    if (check(tokens[exe_pos]->type, characters_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(characters_);
        return node;
#if 0
        Node *left = node;
        // it's possible to iterate over characters
        while(tokens[exe_pos]->type == dot_ || tokens[exe_pos]->type == lbracket_)
        {
            // ft_printf(err, "%t\n", tokens[exe_pos]->type);
            while (tokens[exe_pos]->type == lbracket_)
            {
                node = new_node(tokens[exe_pos]);
                skip(lbracket_);
                node->left = left;      // element to iterate over it
                node->right = expr();
                node->right->token->type = att_type_; // it get declared as identifier
                skip(rbracket_);
                node->token->type = attribute_;
                left = node;
            }
            while (tokens[exe_pos]->type == dot_)
            {
                node = new_node(tokens[exe_pos]);
                skip(dot_);
                node->left = left;    // element to iterate over it
                node->right = expr();
                if(node->right->token->type == identifier_)
                    node->right->token->type = att_type_; // it get decared sometimes as identifier
                node->token->type = attribute_;
                left = node;
            }
        }
        return left;
#endif
    }
    // array
    if (check(tokens[exe_pos]->type, lbracket_, 0))
        return (parents(assign, 0, lbracket_, rbracket_));
    // object, check if you can optimize it
    if (check(tokens[exe_pos]->type, lcbracket_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(lcbracket_);
        node->token->type = obj_;
        int i = 0;
        Node **list = NULL;
        char **keys = NULL;
        if (tokens[exe_pos]->type != rcbracket_) // enter if object is not empty
        {
            list = ft_calloc(i + 1, sizeof(Node *));
            keys = ft_calloc(i + 1, sizeof(char *));
            Token *identifier = tokens[exe_pos]; // expect identifier
            skip(identifier_);
            skip(dots_); // expect dots
            keys[i] = identifier->name;
            list[i] = add_sub();
            i++;
            list = ft_realloc(list,(i ) * sizeof(Node *) ,(i + 1) * sizeof(Node *));
            keys = ft_realloc(keys, (i ) * sizeof(char *), (i + 1) * sizeof(char *));
            while (tokens[exe_pos]->type != rcbracket_ && tokens[exe_pos]->type != eof_)
            {
                skip(comma_);
                identifier = tokens[exe_pos]; // expect identifier
                skip(identifier_);
                skip(dots_); // expect dots :
                keys[i] = identifier->name;
                list[i] = add_sub();
                i++;
                list = ft_realloc(list, (i) * sizeof(Node *) ,(i + 1) * sizeof(Node *));
                keys = ft_realloc(keys, (i) * sizeof(char *), (i + 1) * sizeof(char *));
            }
        }
        keys[i] = NULL;
        list[i] = NULL;
        skip(rcbracket_);
        node->token->keys = keys;
        node->token->object_head = list;
        return node;
    }
    // range
    if (check(tokens[exe_pos]->type, range_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(range_);
        skip(lparent_);
        node->left = expr(); // maybe it shouldn't be expr
        skip(comma_);
        node->right = expr();
        skip(rparent_);
        return node;
    }
    // built in attributes
    if (check(tokens[exe_pos]->type, indexof_, count_, split_, base_, trim_, starts_with_, ends_with_, 0))
    {
        // call it and let iretun rigth value
        // and handle it in attribute
        Node *node = new_node(tokens[exe_pos]);
        skip(tokens[exe_pos]->type);
        skip(lparent_);
        node->right = expr(); // maybe it shouldn't be expr
        skip(rparent_);
        return node;
    }
    // output function
    if (check(tokens[exe_pos]->type, output_, input_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        Node **list = NULL;
        Type type = tokens[exe_pos]->type;

        skip(tokens[exe_pos]->type);
        skip(lparent_);
        if (tokens[exe_pos]->type == rparent_)
            ft_fprintf(err, "expected something to output\n");
        exe_pos--;
        node->token = parents(expr, 0, lparent_, rparent_)->token;
        node->token->type = type;
        return node;
#if 0
        Node *left = node;
        // to be verified
        while(tokens[exe_pos]->type == dot_ || tokens[exe_pos]->type == lbracket_)
        {
            //ft_printf(err, "%t\n", tokens[exe_pos]->type);
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
                if(node->right->token->type == identifier_)
                    node->right->token->type = att_type_; // it get decared sometimes as identifier
                node->token->type = attribute_;
                left = node;
            }
        }
        return left;
#endif
    }
    // identifier
    if (check(tokens[exe_pos]->type, identifier_, 0))
    {
        Node *node = new_node(tokens[exe_pos]);
        // Node *left = node;
        skip(identifier_);
        if (tokens[exe_pos]->type == lparent_)
        {
            node->token->type = func_call_;
            node->left = parents(expr, 0, lparent_, rparent_);
            // left = node;
        }
        return node;
#if 0
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
                if(node->right->token->type == identifier_)
                    node->right->token->type = att_type_; // it get decared sometimes as identifier
                node->token->type = attribute_;
                left = node;
            }
        }
        return left;
#endif
    }
    ft_putchar(out, '\n');
    txt_pos = tokens[exe_pos]->txt_pos;
    while (txt_pos > 0 && text[txt_pos - 1] != '\n')
        txt_pos--;
    while (text[txt_pos] && text[txt_pos] != '\n')
    {
        ft_putchar(out, text[txt_pos]);
        txt_pos++;
    }
    ft_putchar(out, '\n');
    print_space(tokens[exe_pos - 1]->column);
    ft_putstr(out, "^\n");
    ft_fprintf(err, "Error: Unexpected %s in line '%d'\n", type_to_string(tokens[exe_pos]->type), tokens[exe_pos]->line);

    return NULL;
}

// parentises
Node *parents(Node *call_node(void), Type to_find, Type left_, Type right_)
{
    if (check(tokens[exe_pos]->type, left_))
    {
        Node *node = new_node(tokens[exe_pos]);
        skip(left_);
        node->token->type = array_;
        int i = 0;
        Node **array = ft_calloc(i + 1, sizeof(Node *));
        if (tokens[exe_pos]->type != right_) // enter if there is params to hold
        {
            array[i] = call_node(); // to be checked after
            i++;
            array = ft_realloc(array, (i) * sizeof(Node *), (i + 1) * sizeof(Node *));
            while (tokens[exe_pos]->type != right_ && tokens[exe_pos]->type != eof_)
            {
                skip(comma_);
                array[i] = call_node();
                if (to_find && array[i]->token->type != to_find)
                    ft_fprintf(err, "Error in parents\n");
                i++;
                array = ft_realloc(array, (i) * sizeof(Node *),  (i + 1) * sizeof(Node *));
            }
        }
        array[i] = NULL;
        skip(right_);
        node->token->array_head = array;
        node->token->array_len = i;
        return node;
    }
    return NULL;
}
