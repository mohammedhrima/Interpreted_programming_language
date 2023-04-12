#include <ncurses.h>

#define MAX_BUF_SIZE 1024

int main()
{
    int ch, buf_pos = 0;
    char buffer[MAX_BUF_SIZE];
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);
    curs_set(1);

    while((ch = getch()) != KEY_F(1))
    {
        switch(ch)
        {
            case KEY_LEFT:
                // Move cursor left
                break;
            case KEY_RIGHT:
                // Move cursor right
                break;
            case KEY_UP:
                // Scroll up
                break;
            case KEY_DOWN:
                // Scroll down
                break;
            case KEY_BACKSPACE:
            case 127:
                // Delete character before the cursor
                break;
            case KEY_DC:
                if(buf_pos > 0) {
                    // Delete character at cursor position
                    for(int i = buf_pos; i < MAX_BUF_SIZE; i++)
                        buffer[i - 1] = buffer[i];
                    buffer[MAX_BUF_SIZE - 1] = '\0';
                    buf_pos--;
                    mvdelch(getcury(stdscr), getcurx(stdscr) - 1); // Delete character on screen
                }
                break;       
            default:
                if(buf_pos < MAX_BUF_SIZE - 1) {
                    // Insert typed character at cursor position
                    for(int i = MAX_BUF_SIZE - 1; i > buf_pos; i--)
                        buffer[i] = buffer[i - 1];
                    buffer[buf_pos] = ch;
                    buf_pos++;
                    mvaddch(getcury(stdscr), getcurx(stdscr), ch); // Add character on screen
                }
                break;
        }
    }

    endwin();
    return 0;
}
