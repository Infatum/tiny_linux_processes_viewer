#include <iostream>
#include <dirent.h>
#include <vector>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>

std::vector<std::string> get_dirs(const char *dir_name);
bool has_any_digits(const std::string &s);

int main() {
    auto v = get_dirs("/proc");
    for (int i = 0; i < v.size(); ++i) {
        if (i == v.size()-1) {
            std::cout <<  "\t" << v[i] << "." << std::endl;
            break;
        }
        std::cout << "\t" << v[i] << ",\t\t ";
    }
    return 0;
}

std::vector<std::string> get_dirs(const char *dir_name)
{
    DIR *dir;
    std::string dir_path;
    std::vector<std::string> directories_names;
    dirent *ent;
    if ((dir = opendir(dir_name)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            dir_path = "/proc/" + std::string(ent->d_name) + "/cmdline";
            if (access(dir_path.c_str(), R_OK) != -1 && has_any_digits(dir_path))
                directories_names.push_back(std::string(dir_path));

        }
        closedir(dir);
    } else {
        perror("");
        std::cerr << "Error reading from directory/file: " << EXIT_FAILURE << std::endl;
    }
    return  directories_names;
}

bool has_any_digits(const std::string &s)
{
    return std::any_of(s.begin(), s.end(), ::isdigit);
}