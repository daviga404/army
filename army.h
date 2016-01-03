#ifndef ARMY_H
#define ARMY_H

/*
    Definitions
*/
// Program version
#define ARMY_VERSION "0.0.1"

// Default filename for program output (i.e. the object filename)
#define DEFAULT_OUTPUT_FILE "out.o"

/*
    Flags
*/
char *flag_output = DEFAULT_OUTPUT_FILE; // -o [file]
int flag_listing; // -l

/*
    Constants
*/
const char *input_file;

#endif /* ARMY_H */