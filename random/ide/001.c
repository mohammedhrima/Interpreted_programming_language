#include <ncurses.h>
#include <unistd.h>

void ft_putstr(char *str)
{
    int i = 0;
    while(str && str[i])
        write(1, str + i , 1);
}

int main()
{
    int ch;
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);
    curs_set(1); // Show cursor

    while((ch = getch()) != KEY_F(1))
    {
        usleep(900000);
        switch(ch)
        {
            case KEY_LEFT:
                ft_putstr("left\n");
                break;
            case KEY_RIGHT:
                ft_putstr("right\n");
                break;
            case KEY_UP:
                ft_putstr("up\n");
                break;
            case KEY_DOWN:
                ft_putstr("down\n");
                break;
            case '\n':
                ft_putstr("newline\n");
                break;
            case KEY_BACKSPACE:
            case 127:
                // Delete character before the cursor
                break;
            case KEY_F(2):
                printw("F2 button clicked\n"); // Display message
                break;
            case KEY_F(3):
                printw("F3 button clicked\n"); // Display message
                break;
            default:
                // Insert the typed character at the cursor position
                break;
        }
    }

    endwin();
    return 0;
}
