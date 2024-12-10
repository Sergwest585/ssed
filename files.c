

#include "files.h"
#include <unistd.h>
#include <sys/stat.h>

bool is_file_exist(const char *path) {
    return access(path, F_OK) == 0;
}
bool is_directory(const char *path) {
    struct stat stats;
    stat(path, &stats);
    return S_ISDIR(stats.st_mode);
}
