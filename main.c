#include "header.h"

int exe_pos = 0;
void execute()
{
    current = ft_calloc(1, sizeof(List));
    while (tokens[exe_pos]->type != eof_)
    {
        Node *curr = expr();
        evaluate(curr);
    }
}


int main(int argc, char **argv)
{
    FILE *fp = NULL;
    long file_size = 0;

    // open file and read it and feed the text
    fp = fopen(argv[1], "r");
    if (fp == NULL)
        ft_fprintf(err, "Error openning file\n");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    text = ft_calloc(file_size + 1, sizeof(char));
    fseek(fp, 0, SEEK_SET);
    fread(text, file_size, sizeof(char), fp);
    fclose(fp);

    // start interpreting
    // ft_fprintf(out, "%s\n", text);

    // tokenize
    build_tokens();
    tk_pos++; // verify it after
    execute();
    my_free_all();
}

