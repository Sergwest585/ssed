
#include "ssed.h"

#include <stdint.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

[[noreturn]] void error(int code, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);
    
    va_end(args);
    exit(code);
}

void sed_parse_cmd(char *filename, char *cmd) {
    if (cmd[0] == 's') {
        char *c = cmd;
        c++;
        if (!*c || *c != '/')
            error(1, "Not correct command %s. Maybe you want s/pattern/text/?\n", cmd);

        c++;
        char *regex_s = c;
        while (*c && *c != '/') c++;
        if (!*c)
            error(1, "Not correct command %s. Maybe you want s/pattern/text/?\n", cmd);
        char *regex_e = c;
        if (regex_e == regex_s)
            error(1, "Pattern not found: %s", cmd);

        char *regex_str = strndup(regex_s, sizeof(char) * (regex_e - regex_s));

        c++;
        char *text_s = c;
        while (*c && *c != '/') c++;
        if (!*c)
            error(1, "Not correct command %s. Maybe you want s/pattern/text/?\n", cmd);
        char *text_e = c;
        char *text_str = strndup(text_s, sizeof(char) * (text_e - text_s));
        
        if (regex_e - regex_s == 1) {
            if (regex_str[0] == '^') sed_add_prefix(filename, text_str);
            else if (regex_str[0] == '$') sed_add_suffix(filename, text_str);
            else sed_replace(filename, regex_str, text_str);
        } else {
            sed_replace(filename, regex_str, text_str);
        }
        free(regex_str);
        free(text_str);
        return;
    } else if (cmd[0] == '/') {
        char *c = cmd;
        c++;
        char *regex_s = c;
        while (*c && *c != '/') c++;
        if (!*c)
            error(1, "Not correct command %s. Maybe you want /pattern/d?\n", cmd);
        char *regex_e = c;
        char *regex_str = strndup(regex_s, sizeof(char) * (regex_e - regex_s));

        c++;
        if (*c != 'd')
            error(1, "Not correct command %s. Maybe you want /pattern/d?\n", cmd);

        sed_delete(filename, regex_str);
        free(regex_str);
        return;
    } else {
        error(1, "Unknown command: %s\n", cmd);
    }
}
void sed_replace(char *filename, char *before, char *after) {
    
    FILE *tmp = tmpfile();
    FILE *f = fopen(filename, "r");
    
    int c;
    while ((c = fgetc(f)) != EOF) {
        fputc(c, tmp);
    }

    fseek(tmp, 0, SEEK_SET);
    fclose(f);
    f = fopen(filename, "w");

    regex_t reg;
    regmatch_t match[1];
    int result;

    size_t trash;
    size_t len;
    char *line;

    if ((result = regcomp(&reg, before, REG_EXTENDED))) {
        if (result == REG_ESPACE)
            error(1, "regex error: %s\n", strerror(ENOMEM));
        else
            error(1, "regex syntax error\n");
    }

    while ((len = getline(&line, &trash, tmp)) != -1) {
        char *line_s = line;
        while (!regexec(&reg, line_s, 1, match, 0)) {
            for (char *i = line_s; i < line_s + match[0].rm_so; ++i) {
                fputc(*i, f);
            }
            for (size_t i = 0; i < strlen(after); ++i) {
                fputc(after[i], f);
            }

            line_s += match[0].rm_eo;
        }
        for (char *i = line_s; i < line + len; ++i) {
            fputc(*i, f);
        }
    }

    free(line);
    fclose(f);
    fclose(tmp);
}
void sed_delete(char *filename, char *pattern) {
    char after[] = "";
    sed_replace(filename, pattern, after);
}
void sed_add_suffix(char *filename, char *text) {
    FILE *f = fopen(filename, "a");
    fwrite(text, sizeof(char), strlen(text), f);
    fclose(f);
    return;
}
void sed_add_prefix(char *filename, char *text) {
    FILE *tmp = tmpfile();
    FILE *f = fopen(filename, "rw+");
    fwrite(text, sizeof(char), strlen(text), tmp);
    
    int c;
    while ((c = fgetc(f)) != EOF) {
        fputc(c, tmp);
    }

    fseek(f, 0, SEEK_SET);
    fseek(tmp, 0, SEEK_SET);
    while ((c = fgetc(tmp)) != EOF) {
        fputc(c, f);
    }

    fclose(f);
    fclose(tmp);
}

