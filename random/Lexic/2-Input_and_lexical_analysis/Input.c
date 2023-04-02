#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
// #include <__debug>
#include <string.h>
#include <unistd.h>

#ifndef MSDOS
#define COPY(d, s, a) memmove(d, s, a)
#else
#define COPY(d, s, a) memcpy(d, s, a)
#endif

#define STDIN 0

#define MAXLOOK 16
#define MAXLEN 1024

#define BUFSIZE ((MAXLEN * 3) + (MAXLOOK * 2)) // hcnage the 3 only

#define DANGER (End_buf - MAXLOOK) // Flush buffer when Next passes thsi address

#define END (&Start_buf[BUFSIZE]) // past last char in buf

#define NO_MORE_CHARS() (Eod_read && Next >= End_buf)

typedef unsigned char uchar;

static uchar Start_buf[BUFSIZE]; // Input buffer
static uchar *End_buf = END;     // past last character
static uchar *Next = END;        // next input character
static uchar *sMark = END;       // start of current lexeme
static uchar *eMark = END;       // end of current lexeme
static uchar *pMark = NULL;      // start of previous lexeme
static int pLineno = 0;          // line # of previous lexeme
static int pLenght = 0;          // Lenght of previous lexeme

static int Inp_file = STDIN; // Input file handle
static int Lineno = 1;       // current line number
static int Mline = 1;        // Line # when mark_end() called
static int Termchar = 0;     // holds the chacter that was overwritting by 0 when we null terminated
                             // the last lexeme
static int Eof_read = 0;     /* End of file has been read it's possible for this to be true
                                and for chracters to still be in the input buffer.*/

// extern int open(), close(), read(); // if an error found remove comment slashes
static int (*Openp)() = open;   // Pointer to open function
static int (*Closep)() = close; // Pointer to close function
static int (*Readp)() = read;   // Pointer to read function

void ii_io(int (*open_funct)(), int (*close_funct)(), int (*read_funct)())
{
    Openp = open_funct;
    Closep = close_funct;
    Readp = read_funct;
}

int ii_newfile(char *name)
{
    /*
   page 41
    */
    int fd;
    //    MS(if (strcmp(name, "/dev/tty") == 0))
    //    MS(name = "CON";)
    if ((fd = !name ? STDIN : (*Openp)(name, O_RDONLY)) != -1) // O_BINARY
    {
        if (Inp_file != STDIN)
            (*Closep)(Inp_file);
        Inp_file = fd;
        Eof_read = 0;
        Next = NULL;
        sMark = NULL;
        eMark = END;
        End_buf = END;
        
        Lineno = 1;
        Mline = 1;
    }
    return fd;
}