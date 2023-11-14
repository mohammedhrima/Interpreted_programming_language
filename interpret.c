#include "header.h"

List *current;
Node *FUNCTIONS[5000]; // to be modified after
int func_index;
int scoop;

int Value_len(Value **ptr)
{
    int i = 0;
    while (ptr && ptr[i])
        i++;
    return (i);
}

void access_next_scoop()
{
    current->next = ft_calloc(1, sizeof(List));
    List *tmp = current;
    current = current->next;
    current->prev = tmp;
    scoop++;
}

void exit_current_scoop()
{
    if (current->prev == NULL)
        ft_fprintf(err, "Error in exiting level\n");
    current = current->prev;
    scoop--;
}

// Variable
Token *new_variable(Token *var)
{
#if 0
    Token *new = var;
#else
    Token *new = ft_calloc(1, sizeof(Token));
    memcpy(new, var, sizeof(Token));
#endif
    current->variables[current->val_index] = new;
    // new->level = scoop;
    current->val_index++;
    return (new);
}

List *tmp;
Token *get_var(char *name)
{
    visualize_variables();
    tmp = current;
    while (tmp)
    {
        for (int i = tmp->val_index - 1; i >= 0; i--)
        {
            if (ft_strcmp(tmp->variables[i]->name, name) == 0)
                return tmp->variables[i];
        }
        tmp = tmp->prev;
    }
    return NULL;
}

// Function
Node *new_func(Node *func)
{
    // ft_printf(out, "New function with name %s\n", func->token->name);
    FUNCTIONS[func_index] = func;
    func_index++;
    return (func);
}
Node *get_func(char *name)
{
    for (int i = 0; i < func_index; i++)
    {
        if (ft_strcmp(FUNCTIONS[i]->token->name, name) == 0)
            return (FUNCTIONS[i]);
    }
    // ft_fprintf(err, "Undeclared function %s\n", name);
    return NULL;
}

long pow_ten(long exponent)
{
    if (exponent < 0)
        return 0; // negative exponents are not handled in this simple example

    long result = 1;
    for (long i = 0; i < exponent; i++)
        result *= 10;
    return result;
}

Value *evaluate(Node *node)
{
    // ft_printf(out, "Evaluate %k\n", node->token);
    switch (node->token->type)
    {
    // assignement
    case assign_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
        // ft_printf(out, "Do assignemnt between %k and %k\n", left, right);
        // skip this error for function because i can set global variable to idenitifer from params
        if (right->type == identifier_)
            undeclared_error("assign", right, "variable");
        if (left->type != identifier_ && left->type != right->type)
            ft_fprintf(err, "can't assign '%s' type %s to '%s' type %s in line %d\n", left->name, type_to_string(left->type), right->name, type_to_string(right->type), left->line);

        char *name = left->name;
        if (right->type == characters_ && right->is_char == true)
        {
            if (left->type == identifier_)
            {
                left->type = characters_;
                left->is_char = true;
                left->characters = ft_calloc(2, sizeof(char));
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
                left->characters = ft_calloc(ft_strlen(right->characters) + 1, sizeof(char));
                memcpy(left->characters, right->characters, ft_strlen(right->characters));
            }
            else if (left->type == characters_ && left->is_char)
            {
                if (ft_strlen(right->characters) == 1)
                    left->characters[0] = right->characters[0];
                else
                    ft_fprintf(err, "Error: can't assign string to character\n");
            }
            else if (left->type == characters_ && left->is_char == false)
            {
                // protect it from leaks after
                left->characters = ft_calloc(ft_strlen(right->characters) + 1, sizeof(char));
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
    case count_:
    case split_:
    case trim_:
    case base_:
    case starts_with_:
    case ends_with_:
    case att_type_:
    {
        return (node->token);
    }
    // array
    case array_:
    {
        Node **head = node->token->array_head;
        int i = 0;

        Token **array = ft_calloc(i + 1, sizeof(Token *));
        while (head[i])
        {
            Value *to_assign = evaluate(head[i]);
            if (to_assign->type == identifier_)
                undeclared_error("array:", to_assign, "variable");
            array[i] = to_assign;
            i++;
            array = ft_realloc(array, (i) * sizeof(Token *), (i + 1) * sizeof(Token *));
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

        Token **object = ft_calloc(i + 1, sizeof(Token *));
        while (head[i])
        {
            Value *to_assign = evaluate(head[i]);
            if (to_assign->type == identifier_)
                ft_fprintf(err, "key '%s', has no valid value\n", to_assign->name);
            object[i] = to_assign;
            i++;
            object = ft_realloc(object, (i) * sizeof(Token *), (i + 1) * sizeof(Token *));
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
            undeclared_error("math operation", left, "variable");
        if (right->type == identifier_)
            undeclared_error("math operation", right, "variable");
        Value *ret = ft_calloc(1, sizeof(Value));
        if (left->type == number_ && right->type == number_)
        {
            long long number = 0;
            int tmp_right;
            int tmp_left;
            if (left->exponent > right->exponent)
            {
                right->number *= pow_ten(left->exponent - right->exponent);
                right->exponent = left->exponent;
            }
            else if (left->exponent < right->exponent)
            {
                left->number *= pow_ten(right->exponent - left->exponent);
                left->exponent = right->exponent;
            }

            switch (node->token->type)
            {
            case add_:
                number = left->number + right->number;
                break;
            case sub_:
                number = left->number - right->number;
                break;
            case mul_:
                ret->exponent = left->exponent + right->exponent;
                number = left->number * right->number;
                break;
            case div_:
                ret->exponent = left->exponent - right->exponent;
                number = left->number / right->number;
                break;
            case mod_:
                number = left->number % right->number;
                break;
            }

            ret->type = number_;
            ret->number = number;

            return (ret);
        }
        else if (node->token->type == add_ && left->type == characters_ && right->type == characters_)
        {
            ret->type = characters_;
            ret->is_char = left->is_char && right->is_char;
            ret->characters = ft_calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            if (left->is_char)
                ft_strncpy(ret->characters, left->characters, sizeof(char));
            else
                ft_strcpy(ret->characters, left->characters);
            if (right->is_char)
                ft_strncpy(ret->characters + ft_strlen(ret->characters), right->characters, sizeof(char));
            else
                ft_strcpy(ret->characters + ft_strlen(ret->characters), right->characters);
            return (ret);
        }
        else if (node->token->type == add_ && (left->type == array_ && right->type == array_))
        {
            int left_len = Value_len(left->array);
            int right_len = Value_len(right->array);
            ret->type = array_;
            ret->array = ft_calloc(left_len + right_len + 1, sizeof(Token *));
            memcpy(ret->array, left->array, left_len * sizeof(Token *));
            memcpy(&ret->array[left_len], right->array, right_len * sizeof(Token *));
            return (ret);
        }
        else
            ft_fprintf(err, "Error 1: can't do '%s' between '%s' and '%s'\n", type_to_string(node->token->type), type_to_string(left->type), type_to_string(right->type));
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
        if (left->type == identifier_)
            undeclared_error("operaned assign", left, "variable");
        if (right->type == identifier_)
            undeclared_error("operaned assign", right, "variable");
        Value *ret = ft_calloc(1, sizeof(Value));
        if (left->type == number_ && right->type == number_)
        {
            long long number = 0;
            int exponent_difference = 0;

            // Making exponents same before operations
            if (left->exponent > right->exponent)
            {
                exponent_difference = left->exponent - right->exponent;
                right->number *= pow_ten(exponent_difference);
                right->exponent = left->exponent;
            }
            else if (left->exponent < right->exponent)
            {
                exponent_difference = right->exponent - left->exponent;
                left->number *= pow_ten(exponent_difference);
                left->exponent = right->exponent;
            }

            switch (node->token->type)
            {
            case add_:
            case add_assign_:
                number = left->number + right->number;
                break;
            case sub_:
            case sub_assign_:
                number = left->number - right->number;
                break;
            case mul_:
            case mul_assign_:
                ret->exponent = left->exponent + right->exponent;
                number = left->number * right->number;
                break;
            case div_:
            case div_assign_:
                ret->exponent = left->exponent - right->exponent;
                number = left->number / right->number;
                break;
            case mod_:
            case mod_assign_:
                number = left->number % right->number;
                break;
            }

            ret->type = number_;
            ret->number = number;

            // Storing result back to left operand for compound assignments
            if (node->token->type == add_assign_ || node->token->type == sub_assign_ ||
                node->token->type == mul_assign_ || node->token->type == div_assign_ ||
                node->token->type == mod_assign_)
            {
                left->number = ret->number;
                left->exponent = ret->exponent;
            }
        }
        else if (node->token->type == add_assign_ && left->type == characters_ && right->type == characters_)
        {
            ret->type = left->is_char || right->is_char;
            ret->characters = ft_calloc(ft_strlen(left->characters) + ft_strlen(right->characters) + 1, sizeof(char));
            ft_strcpy(ret->characters, left->characters);
            ft_strcpy(ret->characters + ft_strlen(ret->characters), right->characters);
            left->characters = ret->characters;
        }
        else if (node->token->type == add_assign_ && (left->type == array_ && right->type == array_))
        {
            int left_len = Value_len(left->array);
            int right_len = Value_len(right->array);
            ret->type = array_;
            ret->array = ft_calloc(left_len + right_len + 1, sizeof(Token *));
            memcpy(ret->array, left->array, left_len * sizeof(Token *));
            memcpy(&ret->array[left_len], right->array, right_len * sizeof(Token *));
            left->array = ret->array;
        }
        else
        {
            ft_putchar(out, '\n');
            txt_pos = node->token->txt_pos;
            while (txt_pos > 0 && text[txt_pos - 1] != '\n')
                txt_pos--;
            while (text[txt_pos] && text[txt_pos] != '\n')
            {
                ft_putchar(out, text[txt_pos]);
                txt_pos++;
            }
            ft_putchar(out, '\n');
            print_space(node->token->column - ft_strlen(node->token->name));
            ft_putstr(out, "^\n");
            ft_fprintf(err, "Error 2: can't do '%t' between '%t' and '%t'\n", node->token->type, type_to_string(left->type), type_to_string(right->type));
        }
        break;
    }
    // statements
    case if_:
    case elif_:
    {
        // ft_printf(out, "enter if\n");
        Value *condition = evaluate(node->left);
        Value *ret = NULL;
        if (condition->type != boolean_)
            ft_fprintf(err, "Error in if_ elif_, exepected boolean value\n");
        if (condition->boolean)
        {
            Node **bloc_head = node->token->bloc_head;
            int i = 0;
            while (bloc_head[i])
            {
#if 1
                if (bloc_head[i]->token->type == return_)
                {
                    // ft_printf(out, "found return in if_ elif_\n");
                    // exit(0);
                    return bloc_head[i]->token;
                }
                ret = evaluate(bloc_head[i]);
#else
                ret = evaluate(bloc_head[i]);
                if (ret && ret->type == return_)
                {
                    ft_printf(out, "if, elif_ return %v\n", ret);
                    return ret;
                }
#endif
                i++;
            }
        }
        else if (node->right)
        {
            return (evaluate(node->right)); // t obe veirfied after
        }
        ret = new_token(void_);
        // ft_printf(out, "if return %v\n", ret);
        return (ret); // to be verified
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
            if (bloc_head[i]->token->type == return_)
            {
                return bloc_head[i]->token;
            }
            ret = evaluate(bloc_head[i]);
#else
            ret = evaluate(bloc_head[i]);
            if (ret && ret->type == return_)
            {
                ft_printf(out, "else return %v\n", ret);
                return ret;
            }
#endif
            i++;
        }
#endif
        ret = new_token(void_);
        return (ret);
    }
    case while_:
    {
        Value *condition = evaluate(node->left);
        Value *ret;
        if (condition->type != boolean_)
            ft_fprintf(err, "Error in while_, exepected boolean value\n");
        while (condition->boolean)
        {
            Node **bloc_head = node->token->bloc_head;
            int i = 0;
            while (bloc_head[i])
            {
                if (bloc_head[i]->token->type == return_)
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
    case for_:
    {

        Value *right = evaluate(node->right);
        right->array_len = Value_len(right->array);
        Value *ret;
        if (right->type != array_ && right->type != range_)
            ft_fprintf(err, "Expected an array to iterate over it in for loop\n");

        int j = 0;
        access_next_scoop();
        while (j < right->array_len)
        {
            right->array[j]->name = node->left->token->name;
            Token *value = get_var(node->left->token->name);
            if (value == NULL)
                new_variable(right->array[j]);
            else
                *value = *right->array[j];

            ret = NULL;
            int i = 0;
            Node **bloc_head = node->token->bloc_head;
            while (bloc_head[i])
            {
                if (bloc_head[i]->token->type == return_)
                {
                    return bloc_head[i]->token;
                }
                ret = evaluate(bloc_head[i]);
                if (ret && ret->type == break_)
                    break;
                if (ret && ret->type == continue_) // to be verified
                    break;
                i++;
            }
            j++;
        }
        exit_current_scoop();
        return ret;
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
            undeclared_error("logic assign", left, "variable");
        if (right->type == identifier_)
            undeclared_error("logic assign", right, "variable");
        Value *ret = ft_calloc(1, sizeof(Value));
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
            ft_fprintf(err, "Error 3: can't do '%t' between '%t' and '%t'\n", node->token->type, left->type, right->type);
        break;
    }
    case and_:
    case or_:
    {
        Value *left = evaluate(node->left);
        Value *right = evaluate(node->right);
        if (left->type == identifier_)
            undeclared_error("logic assign", left, "variable");
        if (right->type == identifier_)
            undeclared_error("logic assign", right, "variable");
        Value *ret = ft_calloc(1, sizeof(Value));
        if (left->type != boolean_ || right->type != boolean_)
            ft_fprintf(err, "in %t operation, expected boolean values\n", node->token->type);

        switch (node->token->type)
        {
        case and_:
            ret->boolean = left->boolean && right->boolean;
            break;
        case or_:
            ret->boolean = left->boolean || right->boolean;
            break;
        default:
            ft_fprintf(err, "Error ...\n");
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
        return (node->token); // to be verified
    }
    case input_:
    {
        // ft_printf(out, "call input\n");
        Node **head = node->token->array_head;
        int i = 0;
        while (head[i])
        {
            // verify return_ it may cause problems or something
            output(evaluate(head[i]));
            i++;
        }
        // exit(0);
        char *string = ft_readline(out);
        Value *new = ft_calloc(1, sizeof(Value));
        new->type = characters_;
        new->characters = string;
        return (new);
    }
    case range_:
    {
        Value *start = evaluate(node->left);
        Value *end = evaluate(node->right);
        // ft_printf(out, "range from %k to %k\n", start, end);
        Value *ret = ft_calloc(1, sizeof(Value));
        ret->type = array_;
        ret->array_len = end->number > start->number ? end->number - start->number : start->number - end->number;
        ret->array = ft_calloc(ret->array_len + 1, sizeof(Value *));

        long double n = start->number;
        long double step = end->number > start->number ? 1.0 : -1.0;

        int i = 0;
        while (i < ret->array_len)
        {
            ret->array[i] = ft_calloc(1, sizeof(Value));
            ret->array[i]->type = number_;
            ret->array[i]->number = n;
            n += step;
            i++;
        }
        return ret;
    }

    case func_dec_:
    {
        Node *func_dec = new_func(node);
        break;
    }
    case func_call_:
    {
        // find function
        Node *existed_func = get_func(node->token->name);
        if (existed_func == NULL)
            undeclared_error("function call", node->token, "function");
        Node **existed_params = existed_func->left->token->array_head;
        Node **new_params = node->left->token->array_head;
        // access next scoop
        // evaluate params of original function, and set hem to those in call
        int i = 0;
        access_next_scoop();
        while (existed_params[i])
        {
            Token *value = evaluate(new_params[i]);
            value = new_variable(value);
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
            if (ret && ret->type == return_)
            {
                Node **ret_head = ret->bloc_head;
                // ft_printf(out, "Evaluate bloc in function\n");
                int j = 0;
                while (ret_head[j])
                {
                    ret = evaluate(ret_head[j]);
                    j++;
                }
                exit_current_scoop();
                return ret;
            }
#endif
            i++;
        }
        exit_current_scoop();
        ret = new_token(void_);
        return ret;
        break;
    }
    case return_:
        return (node->token);
    case attribute_:
    {
        // len, isnumber, isalpha, indexof ...
        // it's better to check left type then check right attribute
        // iteration over characters and arrays
        // iteration with number
        // attribute with key (identifier)
        // or with characters !!!
        Value *left = evaluate(node->left); // variable
        Value *right = evaluate(node->right);
        // ft_printf(out, "do itteration between %k and %k\n",left, right);
        // exit(0);
        char *key = NULL;
        int val_index = 0;
        Type type = void_;

        if (right->type == identifier_)
        {
            key = right->name;
            type = key_iter;
        }
        else if (right->type == characters_)
        {
            key = right->characters;
            type = key_iter;
        }
        else if (right->type == att_type_)
        {
            key = right->name;
            type = key_iter;
        }
        else if (right->type == number_)
        {
            val_index = (long)right->number;
            type = index_iter;
        }
        else if (check(right->type, indexof_, split_, count_, base_, trim_, starts_with_, ends_with_))
        {
            type = right->type;
        }
        else
        {
            ft_fprintf(err, "Error in iteration\n");
        }
        // ft_printf(out, "left: %v\nright: %v\n", left, right);
        if (left->type == identifier_)
            undeclared_error("attribute", left, "variable");

        if (type == key_iter)
        {
            if (ft_strcmp(key, "type") == 0)
            {
                Value *ret = ft_calloc(1, sizeof(Value));
                ret->type = characters_;
                ret->characters = type_to_string(left->type);
                return ret;
            }
            if (left->type == obj_)
            {
                int i = 0;
                while (left->keys && left->keys[i])
                {
                    if (strcmp(key, left->keys[i]) == 0)
                    {
                        Value *ret = ft_calloc(1, sizeof(Value));
                        // ft_printf(out, "iterate return %v\n", left->object[i]);
                        // exit(0);
                        memcpy(ret, left->object[i], sizeof(Value));
                        return ret;
                    }
                    i++;
                }
                if (ft_strcmp(key, "type") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = characters_;
                    ret->characters = type_to_string(left->type);
                    return ret;
                }
                ft_fprintf(err, "Error: %s has no attribute '%s'\n", left->name, right->name);
            }
            if (left->type == characters_)
            {
                int i = 0;
                if (ft_strcmp(key, "len") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = number_;
                    ret->number = (long double)ft_strlen(left->characters);
                    return ret;
                }
                if (ft_strcmp(key, "toup") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->characters = ft_calloc(ft_strlen(left->characters) + 1, sizeof(char));
                    ret->type = characters_;
                    while (left->characters && left->characters[i])
                    {
                        if (ft_isalpha(left->characters[i]))
                            ret->characters[i] = to_upper(left->characters[i]);
                        else
                            ret->characters[i] = left->characters[i];
                        i++;
                    }
                    return ret;
                }
                if (ft_strcmp(key, "tolow") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->characters = ft_calloc(ft_strlen(left->characters) + 1, sizeof(char));
                    ret->type = characters_;
                    while (left->characters && left->characters[i])
                    {
                        if (ft_isalpha(left->characters[i]))
                            ret->characters[i] = to_lower(left->characters[i]);
                        else
                            ret->characters[i] = left->characters[i];
                        i++;
                    }
                    return ret;
                }
                if (ft_strcmp(key, "tonum") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = number_;
                    // to verify after
                    ret->number = atof(left->characters);
                    return ret;
                }
                if (ft_strcmp(key, "isup") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
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
                    Value *ret = ft_calloc(1, sizeof(Value));
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
                    Value *ret = ft_calloc(1, sizeof(Value));
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
                    Value *ret = ft_calloc(1, sizeof(Value));
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
                if (ft_strcmp(key, "type") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = characters_;
                    ret->characters = type_to_string(left->type);
                    return ret;
                }
                ft_fprintf(err, "%s has no attribute %s\n", left->name, right->name);
            }
            if (left->type == number_)
            {
                if (ft_strcmp(key, "type") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = characters_;
                    ret->characters = type_to_string(left->type);
                    return ret;
                }
                if (ft_strcmp(key, "tochar") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = characters_;
                    ret->characters = convert_base(left->number, "0123456789");
                    return ret;
                }
                ft_fprintf(err, "%s has no attribute %s\n", left->name, right->name);
            }
            if (left->type == array_)
            {
                if (ft_strcmp(key, "len") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = number_;
                    long i = 0;
                    while (left->array && left->array[i])
                        i++;
                    ret->number = i;
                    return ret;
                }
                if (ft_strcmp(key, "type") == 0)
                {
                    Value *ret = ft_calloc(1, sizeof(Value));
                    ret->type = characters_;
                    ret->characters = type_to_string(left->type);
                    return ret;
                }
                output(left);
                ft_fprintf(err, " has no attribute %s\n", left, key);
            }
        }
        if (type == index_iter)
        {
            if (left->type == characters_)
            {
                long i = 0;
                Value *ret = ft_calloc(1, sizeof(Value));
                ret->type = characters_;
                ret->is_char = true;
                if (i < ft_strlen(left->characters))
                {
                    ret->characters = left->characters + val_index;
                    return ret;
                }
                ft_fprintf(err, "Index out of range\n");
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
                ft_fprintf(err, "Index out of range\n");
            }
            ft_fprintf(err, "Can't iterate over %t\n", left->type);
        }

        if (type == indexof_)
        {
            // check if is array or string only
            Value *to_find = evaluate(node->right->right);
            Value *ret = ft_calloc(1, sizeof(Value));
            ret->type = number_;
            if (to_find->type == array_)
                ft_fprintf(err, "can't search of val_index of array (it's not suported for now)\n");
            if (left->type == characters_)
            {
                if (to_find->type != characters_)
                    ft_fprintf(err, "can't do indexof between %t and %t\n", left->type, to_find->type);
                int i = 0;
                while (left->characters && left->characters[i])
                {
                    if (ft_strncmp(left->characters + i, to_find->characters, ft_strlen(to_find->characters)) == 0)
                    {
                        ret->number = (long double)i;
                        return ret;
                    }
                    i++;
                }
            }
            else if (left->type == array_)
            {
                if (left->array[0]->type != to_find->type)
                    ft_fprintf(err, "can't do indexof between %t and %t\n", left->array[0]->type, to_find->type);
                int i = 0;
                while (left->array && left->array[i])
                {
                    if (left->array[i]->type == characters_)
                    {
                        if (ft_strncmp(left->array[i]->characters + i, to_find->characters, ft_strlen(to_find->characters)) == 0)
                        {
                            ret->number = (long double)i;
                            return ret;
                        }
                    }
                    if (left->array[i]->type == number_)
                    {
                        if (left->array[i]->number == to_find->number)
                        {
                            ret->number = (long double)i;
                            return ret;
                        }
                    }
                    i++;
                }
            }
            ret->number = -1;
            return ret;
        }
        if (type == count_)
        {
            Value *to_find = evaluate(node->right->right);
            Value *ret = ft_calloc(1, sizeof(Value));
            ret->type = number_;
            ret->number = 0;
            int i = 0;
            // check if is array or string only
            if (left->type == characters_)
            {
                if (to_find->type != characters_)
                    ft_fprintf(err, "can't do indexof between %t and %t\n", left->type, to_find->type);
                int i = 0;
                while (left->characters && left->characters[i])
                {
                    if (ft_strncmp(left->characters + i, to_find->characters, ft_strlen(to_find->characters)) == 0)
                        ret->number++;
                    i++;
                }
            }
            else if (left->type == array_)
            {
                if (left->array[0]->type != to_find->type)
                    ft_fprintf(err, "can't do indexof between %t and %t\n", left->array[0]->type, to_find->type);
                while (left->array && left->array[i])
                {
                    if (left->array[i]->type == characters_)
                    {
                        if (ft_strncmp(left->array[i]->characters + i, to_find->characters, ft_strlen(to_find->characters)) == 0)
                            ret->number++;
                    }
                    if (left->array[i]->type == number_)
                    {
                        if (left->array[i]->number == to_find->number)
                            ret->number++;
                    }
                    i++;
                }
            }
            else
                ft_fprintf(err, "%t has no attribute count\n", left->type);
            return ret;
        }
        if (type == split_)
        {
            if (left->type == characters_)
            {
                // chekc if is array or string only
                Value *spliter_value = evaluate(node->right->right);
                Value *ret = ft_calloc(1, sizeof(Value));
                ret->type = array_;
                ret->array = NULL;
                // printf("split: %s, by: '%s'\n", left->characters, spliter_value->characters);
                // exit(0);
                char *spliter = spliter_value->characters;
                char **string_array = ft_split(left->characters, spliter);
                int i = 0;
                while (string_array && string_array[i])
                    i++;
                ret->array = ft_calloc(i + 1, sizeof(Token *));
                i = 0;
                while (string_array && string_array[i])
                {
                    ret->array[i] = ft_calloc(1, sizeof(Token));
                    ret->array[i]->type = characters_;
                    ret->array[i]->characters = string_array[i];
                    i++;
                }
                ret->array[i] = NULL;
                return ret;
            }
            else
                ft_fprintf(err, "%t has no attribute split\n", left->type);
        }
        if (type == trim_)
        {
            if (left->type == characters_)
            {
                // chekc if is array or string only
                Value *set_value = evaluate(node->right->right);
                Value *ret = ft_calloc(1, sizeof(Value));
                ret->type = characters_;
                char *set = set_value->characters;
                ret->characters = ft_strtrim(left->characters, set);
                return ret;
            }
            else
                ft_fprintf(err, "%t has no attribute trim\n", left->type);
        }
        if (type == starts_with_)
        {
            if (left->type == characters_)
            {
                Value *to_find_value = evaluate(node->right->right);
                Value *ret = ft_calloc(1, sizeof(Value));
                ret->type = boolean_;
                char *to_find = to_find_value->characters;
                ret->boolean = false;
                if (ft_strlen(to_find) < ft_strlen(left->characters))
                    ret->boolean = (ft_strncmp(left->characters, to_find, ft_strlen(to_find)) == 0);
                return ret;
            }
            else
                ft_fprintf(err, "%t has no attribute startswith\n", left->type);
        }
        if (type == ends_with_)
        {
            if (left->type == characters_)
            {
                Value *to_find_value = evaluate(node->right->right);
                Value *ret = ft_calloc(1, sizeof(Value));
                ret->type = boolean_;
                char *to_find = to_find_value->characters;
                ret->boolean = false;
                if (ft_strlen(to_find) < ft_strlen(left->characters))
                    ret->boolean = (ft_strncmp(left->characters + ft_strlen(left->characters) - ft_strlen(to_find), to_find, ft_strlen(to_find)) == 0);
                return ret;
            }
            else
                ft_fprintf(err, "%t has no attribute endswith\n", left->type);
        }
        if (type == base_)
        {
            if (left->type == number_)
            {
                // chekc if is array or string only
                Value *base = evaluate(node->right->right);
                Value *ret = ft_calloc(1, sizeof(Value));
                ret->type = characters_;
                ret->characters = convert_base(left->number, base->characters);
                // ft_printf(out, "return %k\n", ret);
                return ret;
            }
            else
                ft_fprintf(err, "%t has no attribute base\n", left->type);
        }
        output(left);
        ft_fprintf(err, " has no attribute %s\n", left, type_to_string(type));
        break;
    }
    default:
        ft_fprintf(err, "Error: eval received unknown type '%t' \n", node->token->type);
    }
    return NULL;
}
