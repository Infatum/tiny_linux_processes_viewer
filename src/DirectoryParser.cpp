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
                if (access(dir_path.c_str(), R_OK) != -1 && has_any_digits(dir_path)) {
                    directories_names.push_back(std::string(dir_path));
                    _pids.push_back(atoi(ent->d_name));
                }

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
        Process_manager processManager;
        char state[2];

        for (int i = 0; i < _pids.size(); ++i) {
            strcpy(state, processManager.get_process_Status(_pids[i]));
            std::cout << "\nProcess: " << processManager.get_process_name(_pids[i]) << "\tPID: " << _pids[i] << "\t\tState: "
                      << state << "\t\t\tMemmory used: " << processManager.calculate_memm_used_by_process(_pids[i]) << " %";
            auto core_usage = processManager.get_total_CPU_usage(2);
            for (int j = 0; j < core_usage.size(); ++j) {
                std::cout << "  CPU: " << core_usage[i] << "%";
            }
            //TODO: Realize PID table instance
            //_processes_table = new PID_Table( state[i])

        }
        std::cout << std::endl;
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
        delete _processes_table;
        _root_dir_name.clear();
    }
}

