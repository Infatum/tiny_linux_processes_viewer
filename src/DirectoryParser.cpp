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
                dir_path = "/proc/" + std::string(ent->d_name) + '/';
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

    PID_Table DirectoryParser::process_files(std::vector<std::string> &dir_paths)
    {
        std::ifstream file_to_process;
        std::string line;
        std::string filename;
        Process_manager proc_man;
        for (int i = 0; i < dir_paths.size(); ++i) {
            for (int j = 0; j < _files_to_process.size(); ++j) {
                filename = dir_paths[i] + _files_to_process[j];
                file_to_process.open(filename);
                if (file_to_process.is_open()) {
                    while (getline(file_to_process, line)) {
                        std::cout << line << '\n';
                        file_to_process.close();
                    }

                } else
                    std::cerr << "\nUnable to open file";
            }
        }

        std::cout << "\nTottal CPU usage: " << proc_man.calculateTotalCpu_usage() << std::endl;
        std::cout << "\nProcess 9428 CPU usage: " << proc_man.getProcess_CPU_usage(9428) << std::endl;
    }



    bool DirectoryParser::files_names_match(const std::string &filename)
    {
        for (int j = 0; j < _files_to_process.size(); ++j) {
            if (filename == _files_to_process[j])
                return true;
            }
        return false;
    }


    DirectoryParser::~DirectoryParser()
    {
        _root_dir_name.clear();
    }
}

