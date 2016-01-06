#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "army.h"

// Forward declarations
static void parse_args(int argc, char *argv[]);
static void print_usage();

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    if ((input_file = fopen(argv[argc - 1], "r")) == NULL)
    {
        printf("Error: file not found: %s\n", argv[argc - 1]);
        return EXIT_FAILURE;
    }

    

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
    static struct option long_options[] = {
        { "listing", no_argument,       0, 'l' },
        { "output",  required_argument, 0, 'o' },
        { "version", no_argument,       0, 'v' },
        { 0, 0, 0, 0 }
    };

    int opt;
    int long_index;
    while (( opt = getopt_long(argc, argv, "lo:v", long_options, &long_index) ) != -1)
    {
        switch (opt)
        {
            case 'l':
                flag_listing = 1;
                break;

            case 'o':
                flag_output = optarg;
                break;

            case 'v':
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
                break;

            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc)
    {
        printf("Error: no input files\n");
        exit(EXIT_FAILURE);
    }
    
    // TODO: something with input files

}

/**
 * Print program usage
 */
static void print_usage()
{
    printf(
        "Usage: army [option...] <file>\n"
        "Options:\n"
        "  -l,--listing             Create a listing file\n"
        "  -o,--output [file]       Output the object file to the given file instead.\n"
        "                           Default file to output to is " DEFAULT_OUTPUT_FILE "\n"
        "  -v,--version             Show program version info (must be only flag)\n"
    );
}