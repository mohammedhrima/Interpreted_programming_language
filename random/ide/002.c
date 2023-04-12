#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <form.h>

#define MAX_BUF_SIZE 1024

int main()
{
    int ch;
    char buffer[MAX_BUF_SIZE];
    int pos = 0;

    initscr();            /* Start curses mode 		*/
    raw();                /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */

    int x = 0;
    int y = 0;
    while (1)
    {
        move(y, x);
        ch = getch(); /* If raw() hadn't been called*/
        if (ch == 'x')
            break;
        if (ch == '\n')
        {
            x = 0;
            y++;
        }
        else if (isalpha(ch) || isspace(ch))
        {
            x++;
            attron(A_BOLD);
            printw("%c", ch);
            attroff(A_BOLD);
        }
        else if (ch == KEY_DC)
        {
            // form_driver(1, REQ_DEL_PREV);
            if (pos > 0)
            {
                // Delete character at cursor position
                for (int i = pos; i < MAX_BUF_SIZE; i++)
                    buffer[i - 1] = buffer[i];
                buffer[MAX_BUF_SIZE - 1] = '\0';
                pos--;
                mvdelch(getcury(stdscr), getcurx(stdscr) - 1); // Delete character on screen
            }
        }else if (pos < MAX_BUF_SIZE - 1)
        {
            // Insert typed character at cursor position
            for (int i = MAX_BUF_SIZE - 1; i > pos; i--)
                buffer[i] = buffer[i - 1];
            buffer[pos] = ch;
            pos++;
            mvaddch(getcury(stdscr), getcurx(stdscr), ch); // Add character on screen
        }

        // refresh(); /* Print it on to the real screen */
    }
    getch();  /* Wait for user input */
    endwin(); /* End curses mode		  */

    return 0;
}