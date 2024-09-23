#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOWERCASE_LOWER 97
#define LOWERCASE_UPPER 122

#define UPPERCASE_LOWER 65
#define UPPERCASE_UPPER 90

#define CHAR_CONVERSION 32

char null_filter(char c);
char upper_filter(char c);
char lower_filter(char c);

int main(const int argc, char *argv[])
{
    char        ch;
    int         opt;
    int         ifd;
    int         ofd;
    int         filter_choice = 0;
    const char *input_file    = NULL;
    const char *output_file   = NULL;
    const char *filter        = NULL;

    char (*filters[3])(char) = {null_filter, upper_filter, lower_filter};

    // PARSE ARGS
    while((opt = getopt(argc, argv, "i:o:f:")) != -1)
    {
        switch(opt)
        {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'f':
                filter = optarg;
                break;
                // USAGE
            default:
                fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-f filter]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // HANDLE ARGS
    if(input_file == NULL)
    {
        fprintf(stderr, "Error: Input file is required\n");
        fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-f filter]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(output_file == NULL)
    {
        fprintf(stderr, "Error: No output file specified\n");
        fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-f filter]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(optind < argc)
    {
        fprintf(stderr, "Error: Unexpected arguments\n");
        fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-f filter]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(filter == NULL)
    {
        fprintf(stderr, "Error: no filter specified\n");
        fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-f filter]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // OPEN FILES
    ofd = open(output_file, O_CREAT | O_RDWR | O_CLOEXEC | O_TRUNC, S_IRUSR | S_IWUSR);
    ifd = open(input_file, O_RDONLY | O_CLOEXEC);
    if(ifd == -1)
    {
        fprintf(stderr, "Error: Could not open file\n");
        exit(EXIT_FAILURE);
    }

    // SELECT FILTER
    if(strcmp(filter, "null") == 0)
    {
        filter_choice = 0;
    }

    else if(strcmp(filter, "upper") == 0)
    {
        filter_choice = 1;
    }

    else if(strcmp(filter, "lower") == 0)
    {
        filter_choice = 2;
    }

    else
    {
        fprintf(stderr, "Non existent filter specified\n");
        fprintf(stderr, "Options for -f:\n-null\n-upper\n-lower\n");
        exit(EXIT_FAILURE);
    }

    // APPLY FILTER
    while(read(ifd, &ch, 1) > 0)
    {
        if(filter_choice == 0)
        {
            ch = filters[0](ch);
        }

        else if(filter_choice == 1)
        {
            ch = filters[1](ch);
        }

        else if(filter_choice == 2)
        {
            ch = filters[2](ch);
        }

        write(ofd, &ch, 1);
    }

    // CLEANUP
    close(ifd);
    close(ofd);

    // EXIT
    return EXIT_SUCCESS;
}

char null_filter(char c)
{
    return c;
}

char upper_filter(char c)
{
    if((int)c <= LOWERCASE_UPPER && (int)c >= LOWERCASE_LOWER)
    {
        c = (char)((int)c - CHAR_CONVERSION);
    }
    return c;
}

char lower_filter(char c)
{
    if((int)c <= UPPERCASE_UPPER && (int)c >= UPPERCASE_LOWER)
    {
        c = (char)((int)c + CHAR_CONVERSION);
    }
    return c;
}
