//
// Created by infatum on 4/21/17.
//
#include "DirectoryParser.h"

namespace linux_process_viewer {

    std::vector<std::string> DirectoryParser::get_directories_names()
    {
        DIR *dir;
        std::string dir_path;
        std::vector<std::string> directories_names;
        dirent *ent;
        if ((dir = opendir(_root_dir_name.c_str())) != NULL) {
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

    bool DirectoryParser::has_any_digits(const std::string &s)
    {
        return std::any_of(s.begin(), s.end(), ::isdigit);
    }

    void DirectoryParser::process_files(std::vector<std::string> &filepaths)
    {
        std::ifstream file_to_process;
        std::string line;
        for (int i = 0; i < filepaths.size(); ++i) {
            file_to_process.open(filepaths[i]);
            if (file_to_process.is_open()) {
                while (getline(file_to_process, line))
                    std::cout << line << '\n';
                file_to_process.close();
            } else
                std::cerr << "Unable to open file";
        }
    }

    DirectoryParser::~DirectoryParser()
    {
        _root_dir_name.clear();
    }
}

