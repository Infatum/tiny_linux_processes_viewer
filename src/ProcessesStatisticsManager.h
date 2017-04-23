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
    class ProcessesStatisticsManager {
    private:
        std::string _root_dir_name;
        std::vector<unsigned int> _pids;
        PID_Table *_processes_table = nullptr;
        std::vector<std::string> _files_to_process;
        std::vector<linux_process_viewer::PID_Table> _proc_statistics;

        bool has_any_digits(const std::string &s);
        void refresh(std::chrono::duration<int> &seconds);
        bool files_names_match(const std::string &filename);

    public:
        ProcessesStatisticsManager(const std::string &root_dir_name,
                        std::vector<std::string> fn = {"cmdline", "stat", "statm", "cpuset"})
                : _root_dir_name(root_dir_name), _files_to_process(fn) {}
        std::vector<std::string> get_directories_names();
        PID_Table show_proc_info(std::vector<std::string> &dir_paths);
        virtual ~ProcessesStatisticsManager();
    };
}

#endif //LINUX_PROCESS_VIEWER_DIRECTORYPARSER_H
