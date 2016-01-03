#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "army.h"

// Forward declarations
static void parse_args(int argc, char *argv[]);
static void print_usage();

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    return 0;
}

/**
 * Parse a set of command line arguments for the program
 *
 * @param argc Number of args in argv
 * @param argv Program arguments ([0] is the program name)
 */
static void parse_args(int argc, char *argv[])
{
    // Ensure we have at least two args
    if (argc < 2)
    {
        print_usage();
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp(argv[1], "-v"))
    {
        printf(
            "army v" ARMY_VERSION "\n"
            "Copyright (c) 2016 daviga404\n"
            "License GPLv3: GNU GPL version 3 <http://gnu.org/licenses/gpl.html>\n"
            "\n"
            "This software is free software and was made for fun. You may\n"
            "redistribute it under the terms of the GPLv3 license. This program\n"
            "has absolutely no warranty.\n"
        );
        exit(EXIT_SUCCESS);
    }

    int i;
    for (i = 1; i < argc - 1; i++)
    {
        char *arg = argv[i];

        switch (arg[1])
        {
        // -l: generate listing
        case 'l':
            flag_listing = 1;
            break;

        // -o: output file
        case 'o':
            // Ensure we have at least the file to output to and
            // the input file
            if (argc > i + 2)
            {
                flag_output = argv[++i];
            }
            else
            {
                print_usage();
                exit(EXIT_SUCCESS);
            }
            break;

        default:
            printf("Unrecognized flag: %s\n", arg);
            print_usage();
            exit(EXIT_SUCCESS);
        }
    }

    // Set input file (constant defined in army.h)
    input_file = argv[argc - 1];
}

/**
 * Print program usage
 */
static void print_usage()
{
    printf(
        "Usage: army [option...] <file>\n"
        "Options:\n"
        "  -l              Create a listing file\n"
        "  -o [file]       Output the object file to the given file instead.\n"
        "                  Default file to output to is " DEFAULT_OUTPUT_FILE "\n"
        "  -v              Show program version info (must be only flag)\n"
    );
}