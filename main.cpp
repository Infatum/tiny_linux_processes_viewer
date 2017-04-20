#include <iostream>
#include <dirent.h>


int get_dirs(const char *dir_name);

int main() {
    get_dirs("/proc");
    return 0;
}

int get_dirs(const char *dir_name)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(dir_name)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::cout << ent->d_name << '\n';
        }
        closedir(dir);
    } else {
        perror("");
        return EXIT_FAILURE;
    }
}