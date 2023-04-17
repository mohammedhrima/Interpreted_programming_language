#include "header.h"
#include "tools.c"
#include "debug.c"

int tk_pos;

void new_token(int start, Type type)
{
    Token *new = NULL;
    new = calloc(1, sizeof(Token));
    new->value = calloc(txt_pos - start + 1, sizeof(char));
    ft_strncpy(new->value, text + start, txt_pos - start);
    new->type = type;
    ft_printf(out, "new token with value '%s' and type '%s', put in position '%d' \n", new->value, type_to_string(new->type), tk_pos);
    tokens[tk_pos++] = new;
    ;
}

void skip_spaces()
{
    while (ft_isspace(text[txt_pos]))
        txt_pos++;
}

void expect(char c)
{
    if (text[txt_pos] != c)
        ft_printf(err, "Syntax error, expecting '%c'\n", c);
    txt_pos++;
}

void build_tokens()
{
    int start = 0;
    while (text[txt_pos])
    {
        skip_spaces();
        if (text[txt_pos] == '\0')
            break;
        start = txt_pos;
        int type = 0;
        for (int i = 0; multi_tokens1[i].type; i++)
        {
            if (ft_strncmp(multi_tokens1[i].value, text + txt_pos, ft_strlen(multi_tokens1[i].value)) == 0 && ft_isspace(text[txt_pos + ft_strlen(multi_tokens1[i].value)]))
            {
                type = multi_tokens1[i].type;
                txt_pos += ft_strlen(multi_tokens1[i].value);
                break;
            }
        }
        if (type)
            new_token(start, type);
        type = 0;
        for (int i = 0; multi_tokens2[i].type; i++)
        {
            if (ft_strncmp(multi_tokens2[i].value, text + txt_pos, ft_strlen(multi_tokens2[i].value)) == 0)
            {
                type = multi_tokens2[i].type;
                txt_pos += ft_strlen(multi_tokens2[i].value);
                break;
            }
        }
        if (type)
            new_token(start, type);
        else if (ft_isalpha(text[txt_pos]))
        {
            // get varibale name
            while (ft_isalnum(text[txt_pos]))
                txt_pos++;
            new_token(start, variable_);
        }
        else if (ft_isdigit(text[txt_pos]))
        {
            // get number value
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
                txt_pos++;
            while (ft_isdigit(text[txt_pos]))
                txt_pos++;
            new_token(start, number_);
        }
        else if (text[txt_pos] == '"')
        {
            txt_pos++;
            while (text[txt_pos] && text[txt_pos] != '"')
                txt_pos++;
            expect('\"');
            new_token(start, characters_);
        }
        else
        {
            ft_printf(err, "unknown value s:'%s', c:'%c', d:'%d'\n", text + txt_pos, text[txt_pos], text[txt_pos]);
        }
    }
    new_token(txt_pos, eof_);
}

// parsing
Node *expr();
Node *assign();
Node *prime();

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = prime();
    if (tokens[tk_pos]->type == assign_)
    {
        // printf("found assign\n");
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
        // skip left braket

        // to be verified
        tk_pos++;
        left = expr();
        left->type = array_;
        // tk_pos++;
    }
    else if (tokens[tk_pos]->type == variable_)
    {
        printf("found variable\n");
        left = calloc(1, sizeof(Node));
        left->type = tokens[tk_pos]->type;
        left->content = tokens[tk_pos]->value;
        tk_pos++;
    }
    else if (tokens[tk_pos]->type == characters_)
    {
        printf("found characters\n");
        left = calloc(1, sizeof(Node));
        left->type = tokens[tk_pos]->type;
        left->content = tokens[tk_pos]->value;
        tk_pos++;
    }
    else if (tokens[tk_pos]->type == number_)
    {
        printf("found numbers : '%s'\n", tokens[tk_pos]->value);
        left = calloc(1, sizeof(Node));
        left->type = tokens[tk_pos]->type;
        left->content = tokens[tk_pos]->value;
        printf("left->content: %s\n", left->content);
        tk_pos++;
    }
    return left;
}

// variables
void new_var(char *name, Type type, char *value)
{
    var *new = NULL;
    new = calloc(1, sizeof(var));
    new->name = name;
    new->curr_index = var_index;
    new->type = type;
    if (type == characters_)
        new->value.string = value;
    if (type == number_)
    {
        new->type = integer_;
        double res = 0;
        int i = 0;
        while (ft_isdigit(value[i]))
        {
            res = 10 * res + (value[i] - '0');
            i++;
        }
        if (value[i] == '.')
        {
            new->type = float_;
            i++;
        }
        double pres = 0.1;
        while (ft_isdigit(value[i]))
        {
            res = res + pres * (value[i] - '0');
            pres /= 10;
            i++;
        }
        ft_printf(out, "assign : '%d'\n", res);
        new->value.number = res;
    }
    variables[var_index++] = new;
}

void execute()
{
    Node *curr = expr();
    char *str1, *str2, *str3;
    str1 = type_to_string(curr->type);
    if (str1 == NULL)
        ft_printf(err, "error in parse.c line %d\n", __LINE__);
    ft_printf(out, "do '%s', between '%s' and '%s' \n", str1, curr->left->content, curr->right->content);
    while (curr)
    {
        if (curr->type == assign_)
            new_var(curr->left->content, curr->right->type, curr->right->content);
        curr = expr();
    }
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
    ft_printf(out, "%s\n", text);
    indexes();
    ft_printf(out, "\n");

    build_tokens();

    int i = 0;
    while (i < tk_pos)
    {
        // ft_printf(out, "index: %d\n", i);
        if (tokens[i] == NULL)
            ft_printf(err, "found null token in index : %d\n", i);
        char *type_str = type_to_string(tokens[i]->type);
        if (type_str)
            ft_printf(out, "%s : %s\n", type_str, tokens[i]->value);
        else
            ft_printf(err, "Unknown token '%d'\n", tokens[i]->type);
        i++;
    }
    tk_pos = 0;
    execute();
    visualize_variables();
}