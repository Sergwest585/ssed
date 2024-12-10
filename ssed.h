
#ifndef SSED_H
#define SSED_H

[[noreturn]] void error(int code, char *fmt, ...);

void sed_parse_cmd(char *filename, char *cmd);
void sed_replace(char *filename, char *before, char *after);
void sed_delete(char *filename, char *pattern);
void sed_add_suffix(char *filename, char *text);
void sed_add_prefix(char *filename, char *text);


#endif