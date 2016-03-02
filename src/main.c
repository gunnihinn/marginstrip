#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

const char* help = "Strip \\marginpar commands from LaTeX files.\n"
                   "\n"
                   "Use:\n"
                   "\n"
                   "    marginstrip [OPTION]... FILE...\n"
                   "\n"
                   "Read from standard input if no FILE is given.\n"
                   "\n"
                   "Options:\n"
                   "    -h, --help     Print this help and exit\n"
                   "    -v, --version  Print version info and exit\n";

const char* version = "Version 2016-02-25\nBuilt from Git commit 8a392d5\n";

void get_control_word(FILE* fh, char* buf, int bufsize)
{
    /* 
     * Get LaTeX control word or symbol from file handle.
     * A control word is 
     *      \\[A-Za-z]+
     * and a control symbol is
     *      \\[^A-Za-z]
     * The function assumes we've already seen the \\.
     *
     * Since we only need to know if the control word we have is \marginpar
     * or \marginparhere, we only read enough chars from the file to check
     * that.
     */

    int c = getc(fh);
    *buf = c;

    int i = 0;
    if (isalpha(c)) {
        while (isalpha((c = getc(fh)))) {
            if (i == bufsize - 2) {
                break;
            }
            *(buf + ++i) = c;
        }
        ungetc(c, fh);
    }
    *(buf + ++i) = '\0';
}

bool is_skipped(char* word, int limit)
{
    if (strncmp("marginpar", word, limit) == 0) {
        return true;
    } else if (strncmp("marginparhere", word, limit) == 0) {
        return true;
    } else {
        return false;
    }
}

void skip_until_newline(FILE *fh)
{
    int c;
    while ((c = getc(fh)) != EOF) {
        if (c == '\n') {
            break;
        }
    }
}

/* Skip until opening { */
void skip_until_open_brace(FILE* fh)
{
    int c;
    int braces = 0;
    while (!braces) {
        c = getc(fh);
        switch (c) {
        case '\\':
            getc(fh);
            break;
        case '{':
            braces++;
            break;
        case EOF:
            braces--;
            break;
        default:
            /* Drop c by default */
            break;
        }
    }
}

/* Skip until closing }, assuming we're at an opening { */
void skip_until_close_brace(FILE* fh)
{
    int c;
    int braces = 1;
    while (braces) {
        c = getc(fh);
        switch (c) {
        case '\\':
            getc(fh);
            break;
        case '%':
            skip_until_newline(fh);
            break;
        case '{':
            braces++;
            break;
        case '}':
            braces--;
            break;
        case EOF:
            printf("\n");
            braces = 0;
            break;
        default:
            /* Drop c by default */
            break;
        }
    }
}

/*
 * Skip over the arguments to \marginpar or \marginparhere.
 */
void skip_argument(FILE* fh)
{
    skip_until_open_brace(fh);
    skip_until_close_brace(fh);

    /* Check for comments or \n */
    int c = getc(fh);
    switch (c) {
    case '%':
        while (true) {
            c = getc(fh);
            if (c == '\n' || c == EOF) {
                break;
            }
        }
        break;
    case '\n':
        break;
    default:
        ungetc(c, fh);
        break;
    }
}

void print_until_newline(FILE *fh)
{
    int c;
    while ((c = getc(fh)) != EOF) {
        printf("%c", c);
        if (c == '\n') {
            break;
        }
    }
}

/* 
 * TeX control word buffer. The buffer only needs to be able to hold
 * 'marginpar' or 'marginparhere', so it is rather small.
 */
#define BUFSIZE 24

void marginstrip(FILE* fh)
{
    int c;
    char* buf = malloc(BUFSIZE * sizeof(char));

    while ((c = getc(fh)) != EOF) {
        switch (c) {
        case '\\':
            get_control_word(fh, buf, BUFSIZE);
            if (is_skipped(buf, BUFSIZE)) {
                skip_argument(fh);
            } else {
                printf("\\%s", buf);
            }
            break;
        case '%':
            printf("%c", c);
            print_until_newline(fh);
            break;
        default:
            printf("%c", c);
            break;
        }
    }
}

int main(int argc, char** argv)
{
    int c;

    while (true) {
        static struct option long_options[] = {
            { "help", no_argument, 0, 'h' },
            { "version", no_argument, 0, 'v' },
            { 0, 0, 0, 0 }
        };

        /* getopt_long option index */
        int option_index = 0;

        c = getopt_long(argc, argv, "hv", long_options, &option_index);

        /* Detect the end of the options */
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'h':
            printf("%s", help);
            exit(EXIT_SUCCESS);
            break;
        case 'v':
            printf("%s", version);
            exit(EXIT_SUCCESS);
            break;
        default:
            abort();
        }
    }

    if (optind < argc) {
        while (optind < argc) {
            FILE* fh = fopen(argv[optind], "r");
            if (!fh) {
                /* TODO: Check errno for better error messages */
                fprintf(stderr, "Error: Couldn't open '%s' for reading\n", argv[optind]);
                exit(EXIT_FAILURE);
            }
            marginstrip(fh);
            fclose(fh);
            optind++;
        }
    } else {
        marginstrip(stdin);
    }

    exit(EXIT_SUCCESS);
}
