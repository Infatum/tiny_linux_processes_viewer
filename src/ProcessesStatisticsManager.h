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
#include <math.h>
#include <regex>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Process_manager.h"
#include "PID_Table.h"
#define SORT_PID_FLAG 'p'
#define SORT_CPU_FLAG 'c'
#define SORT_MEM_FLAG 'm'
#define SORT_STATE_FLAG 's'

extern char flag;

namespace linux_process_viewer {

    typedef posix_time::ptime _time;
    class ProcessesStatisticsManager {
    private:
        std::string _root_dir_name;
        std::vector<unsigned int> _pids;
        PID_Table *_processes_table = nullptr;
        std::vector<std::string> _files_to_process;
        std::vector<linux_process_viewer::PID_Table *> _proc_statistics;

        bool has_any_digits(const std::string &s);
        void refresh(std::chrono::duration<int> &seconds);
        bool files_names_match(const std::string &filename);

    public:
        ProcessesStatisticsManager(const std::string &root_dir_name,
                        std::vector<std::string> fn = {"cmdline", "stat", "statm", "cpuset"})
                : _root_dir_name(root_dir_name), _files_to_process(fn) {}
        std::vector<std::string> update_pids();
        void show_proc_info(std::vector<std::string> &dir_paths, unsigned delay_time);
        virtual ~ProcessesStatisticsManager();
        float calculate_CPU_usage_by_process(const PID_Table *pid);
        void sort_processes(char flag);

        void show(const std::string &s);

        void draw_table(const std::vector<PID_Table *> statistics);
    };
}

#endif //LINUX_PROCESS_VIEWER_DIRECTORYPARSER_H
