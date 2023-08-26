#include "header.h"

char *type_to_string(Type type)
{
    struct
    {
        char *string;
        Type type;
    } Types[] = {
        // data types
        {"IDENTIFIER", identifier_},
        {"CHARACTERS", characters_},
        {"BOOLEAN", boolean_},
        {"NUMBER", number_},
        {"ARRAY", array_},
        {"OBJ", obj_},
        {"VOID", void_},
        {"TUPLE", tuple_},
        // assigns
        {"ASSIGNEMENT", assign_},
        {"ADD ASSIGN", add_assign_},
        {"SUB ASSIGN", sub_assign_},
        {"MUL ASSIGN", mul_assign_},
        {"DIV ASSIGN", div_assign_},
        {"MOD ASSIGN", mod_assign_},
        // statements
        {"WHILE", while_},
        {"IF", if_},
        {"ELIF", elif_},
        {"ELSE", else_},
        {"FOR", for_},
        {"IN", in_},
        // end statements
        {"DOTS", dots_},
        {"COMMA", comma_},
        // parents,  brackets
        {"LEFT PARENT", lparent_},
        {"RIGHT PARENT", rparent_},
        {"LEFT BRACKET", lbracket_},
        {"RIGHT BRACKET", rbracket_},
        {"LEFT CURLY BRACKET", lcbracket_},
        {"RIGHT CURLY BRACKET", rcbracket_},
        // built in functions
        {"INPUT", input_},
        {"OUTPUT", output_},
        {"RANGE", range_},
        // math operators
        {"ADDITION", add_},
        {"SUBSTRACTION", sub_},
        {"MULTIPLACTION", mul_},
        {"DIVISION", div_},
        {"MODULO", mod_},
        {"MORE THAN", more_than_},
        {"LESS THAN", less_than_},
        {"MORE THAN OR EQUAL", more_than_or_equal_},
        {"LESS THAN OR EQUAL", less_than_or_equal_},
        // logic operators
        {"AND", and_},
        {"OR", or_},
        {"NOT", not_},
        {"COMPARE", equal_},
        {"NOT EQUAL", not_equal_},
        // functions
        {"FUNCTION DECLARATION", func_dec_},
        {"FUNCTION CALL", func_call_},
        // key words
        {"BREAK", break_},
        {"CONTINUE", continue_},
        {"RETURN", return_},
        // attributes
        {"DOT", dot_},
        {"ATTRIBUTE NODE", attribute_},
        {"ATTRIBUTE TYPE", att_type_},
        // built in attributes
        {"indexof", indexof_},
        {"split", split_},
        {"trim", trim_},
        {"count", count_},
        {"base", base_},
        {"starts with", starts_with_},
        {"ends with", ends_with_},
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

void undeclared_error(Token *token, char *type)
{
    ft_putchar(out, '\n');
    txt_pos = token->txt_pos;
    while (txt_pos > 0 && text[txt_pos - 1] != '\n')
        txt_pos--;
    while (text[txt_pos] && text[txt_pos] != '\n')
    {
        ft_putchar(out, text[txt_pos]);
        txt_pos++;
    }
    ft_putchar(out, '\n');
    print_space(token->column - ft_strlen(token->name));
    ft_putstr(out, "^\n");
    ft_fprintf(err, "Error: Undeclared %s in line '%d'\n", type, token->line);
}

int numberOfDigits(long long n)
{
    int count = 0;
    while (n != 0)
    {
        count++;
        n /= 10;
    }
    return count;
}

void printFixedPoint(long number, int exponent)
{
    if (exponent < 0)
    {
        // Case for very large numbers
        ft_printf("Error: Exponent is negative, cannot handle this scenario.");
        return;
    }

    // Determine integer part and fractional part

    long int_part = exponent ? number / pow_ten(exponent) : number;
    long frac_part = exponent ? number % pow_ten(exponent) : 0;

    // Print integer part
    ft_putnbr(out, int_part);
    // printf("%lld.", int_part);

    // Print fractional part with leading zeros

    if (exponent)
    {
        for (int i = 0; i < exponent - numberOfDigits(frac_part); i++)
        {
            ft_putchar(out, '0');
        }
        ft_putnbr(out, frac_part);
    }
    // printf("%lld\n", frac_part);
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
            undeclared_error(token, "variable");
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
            // token->is_float = ft_putfloat(fd, token->number, 6);
            // ft_printf("hey fix printing float\n");
            printFixedPoint(token->number, token->exponent);
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
                    ft_putstr(fd, "\033[31m, ");
            }
            ft_putstr(fd, " \033[31m]");
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
                    ft_putstr(fd, "\033[31m,");
            }
            ft_putstr(fd, " \033[31m}");
            break;
        }
        case void_:
        {
            ft_putstr(fd, "(void)\n");
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

void see_token(Token *token)
{
#if DEBUG
    if (token)
    {
        ft_printf("%s in line [", type_to_string(token->type));
        if (token->line < 10)
            ft_putstr(out, "00");
        else if (token->line < 100)
            ft_putstr(out, "0");
        ft_printf("%d] in column [", token->line);
        if (token->column < 10)
            ft_putstr(out, "0");
        ft_printf("%d]", token->column);

        ft_printf(" in tab [");
        if (token->tab < 10)
            ft_putstr(out, "0");
        ft_printf("%d]", token->tab);

        if (token->name)
            ft_printf(" name: %s, ", token->name);
        switch (token->type)
        {
        case characters_:
        {
            ft_putstr(out, " value: ");
            output(token);
            break;
        }
        case number_:
        {
            ft_putstr(out, " value: ");
            output(token);
            break;
        }
        case boolean_:
        {
            ft_putstr(out, " value: ");
            output(token);
            break;
        }
        case array_:
        {
            ft_putstr(out, " value: \n");
            ft_putstr(out, "                  ");
            output(token);
            break;
        }
        case obj_:
        {
            ft_putstr(out, " value: \n");
            for (int i = 0; token->object && token->object[i]; i++)
            {
                ft_printf("         %s : ", token->keys[i]);
                output(token->object[i]);
                ft_putchar(out, '\n');
            }
            ft_putstr(out, "        ");
            break;
        }
        case void_:
        {
            ft_putstr(out, " (void)\n");
            break;
        }
        default:
        {
            if (type_to_string(token->type) == NULL)
            {
                ft_putstr(err, "Unkown token type: ");
                ft_putstr(err, type_to_string(token->type));
                exit(1);
            }
        }
        }
    }
    else
        ft_putstr(out, "(null token)");
    ft_putchar(out, '\n');
#endif
}

void visualize_variables(void)
{
#if DEBUG
    int i = 0;
    ft_fprintf(out, "\n\nvariables: \n");
    List *tmp = current;
    while (tmp->prev)
        tmp = tmp->prev;
    // var_level is changeable don't think about putting it here
    while (tmp)
    {
        ft_fprintf(out, "    level %d, has %d variables:\n", i, tmp->val_index);
        int j = 0;
        while (j < tmp->val_index)
        {
            ft_putstr(out, "        ");
            output(tmp->variables[j]);
            ft_putchar(out, '\n');
            j++;
        }
        tmp = tmp->next;
        i++;
    }
    if (FUNCTIONS[0])
    {
        ft_fprintf(out, "\nfunctions: \n");
        i = 0;
        while (FUNCTIONS[i])
        {
            ft_putstr(out, "        ");
            output(FUNCTIONS[i]->token);
            ft_putchar(out, '\n');
            i++;
        }
    }
#endif
    ft_fprintf(out, "\n");
}
