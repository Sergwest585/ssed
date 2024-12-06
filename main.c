
#include "ssed.h"
#include "files.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

char *program_name;

[[noreturn]] void error(int code, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char *err_msg;
    vsscanf(err_msg, fmt, args);
    perror(err_msg);
    
    va_end(args);
    exit(code);
}

void help_message() {
    printf("%s <filename> <cmd>\n%s <filename> <flag> <pattern>\n", program_name, program_name);
}

int main(int argc, char *args[]) {
    program_name = args[0];
    if (argc > 2) {
        if (!is_file_exist(args[1]))
            error(1, "File not exist: %s\n", args[1]);
        if (is_directory(args[1]))
            error(1, "It is directory: %s\n", args[1]);
        
        if (argc == 3) {  /*       ./ssed file cmd         */
            sed_parse_cmd(args[1], args[2]);
        } else {          /*  ./ssed file -flag pattern... */
            if (args[2][0] != '-') {
                help_message();
                error(1, "Flag not found\n");
            }
            if (strcmp(args[2], "-r")) {
                if (argc != 5)
                    error(1, "Replace must be: %s -r old new\n", program_name);
                sed_replace(args[1], args[3], args[4]);
            } else if (strcmp(args[2], "-d")) {
                if (argc != 4)
                    error(1, "Delete must be: %s -d pattern\n", program_name);
                sed_delete(args[1], args[3]);
            } else if (strcmp(args[2], "-i")) {
                if (argc != 5)
                    error(1, "Insert must be: %s -i -f/b text\n", program_name);
                
                if (strcmp(args[3], "-f")) {
                    sed_add_suffix(args[1], args[4]);
                } else if (strcmp(args[3], "-b")) {
                    sed_add_prefix(args[1], args[4]);
                } else {
                    error(1, "Unknown flag: %s\n", args[3]);    
                }
            } else {
                error(1, "Unknown flag: %s\n", args[2]);
            }
        }
    } else {
        help_message();
    }
}