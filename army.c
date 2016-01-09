#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "army.h"

// Forward declarations
static int parse_args(int argc, char *argv[]);
static void print_usage();

int main(int argc, char *argv[])
{
    int fileind = parse_args(argc, argv);

    for (int i = fileind; i < argc; i++)
    {
        printf("File: %s\n", argv[i]);
    }

    return 0;
}

/**
 * Parse a set of command line arguments for the program. Arguments
 * that do not correspond to options/flags are placed at the
 * end of argv by getopt_long. The index of the first non-option
 * (i.e. a filename) argument is returned by this function so that
 * the filenames can be obtained by the calling function.
 *
 * @param argc Number of args in argv
 * @param argv Program arguments ([0] is the program name)
 * @return Index of first file argument in argv
 */
static int parse_args(int argc, char *argv[])
{
    static struct option long_options[] = {
        { "listing", no_argument,       0, 'l' },
        { "output",  required_argument, 0, 'o' },
        { "version", no_argument,       0, 'v' },
        { "help",    no_argument,       0, 'h' },
        { 0, 0, 0, 0 }
    };

    int opt;
    int long_index;
    while (( opt = getopt_long(argc, argv, "lo:vh", long_options, &long_index) ) != -1)
    {
        switch (opt)
        {
            case 'l':
                flag_listing = true;
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

            case 'h':
                print_usage();
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
    
    return optind;

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
        "  -h,--help                Show this help message"
    );
}