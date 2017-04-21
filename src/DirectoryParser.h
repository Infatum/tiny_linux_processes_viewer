//
// Created by infatum on 4/21/17.
//

#ifndef LINUX_PROCESS_VIEWER_DIRECTORYPARSER_H
#define LINUX_PROCESS_VIEWER_DIRECTORYPARSER_H
#include <iostream>
#include <dirent.h>
#include <vector>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <regex>
#include "Process_manager.h"
#include "PID_Table.h"

namespace linux_process_viewer {
    class DirectoryParser {
    private:
        std::string _root_dir_name;
        std::vector<std::string> _files_to_process;
        std::vector<unsigned int> _pids;
        bool has_any_digits(const std::string &s);
        bool files_names_match(const std::string &filename);

    public:
        DirectoryParser(const std::string &root_dir_name,
                        std::vector<std::string> fn = {"cmdline", "stat", "statm", "cpuset"})
                : _root_dir_name(root_dir_name), _files_to_process(fn) {}
        std::vector<std::string> get_directories_names();
        PID_Table process_files(std::vector<std::string> &dir_paths);
        virtual ~DirectoryParser();
    };
}

#endif //LINUX_PROCESS_VIEWER_DIRECTORYPARSER_H
