//
// Created by infatum on 4/21/17.
//
#include "ProcessesStatisticsManager.h"

namespace linux_process_viewer {

    std::vector<std::string> ProcessesStatisticsManager::get_directories_names()
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


    bool ProcessesStatisticsManager::has_any_digits(const std::string &s)
    {
        return std::any_of(s.begin(), s.end(), ::isdigit);
    }

    /**
     * Calculates the average processes info statistics and returns a structure, collects data each update_time seconds
     * @param dir_paths
     * @param update_time
     * @return average structure with process id, name, status, cpu and memmory usage in %
     */
    PID_Table ProcessesStatisticsManager::show_proc_info(std::vector<std::string> &dir_paths, unsigned update_time)
    {
        Process_manager processManager;
        char state[2];


    }

    /**
    * TODO: refresh procedure table instance each n seconds
    * @param seconds
    * @param proc_table
    */
    void ProcessesStatisticsManager::refresh(std::chrono::duration<int> &seconds)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto start_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
        auto end = now + seconds;
        auto n = std::chrono::system_clock::now();
        do {
            std::this_thread::yield();
            n = std::chrono::system_clock::now();
        } while (n < end);
    }

    bool ProcessesStatisticsManager::files_names_match(const std::string &filename)
    {
        for (int j = 0; j < _files_to_process.size(); ++j) {
            if (filename == _files_to_process[j])
                return true;
            }
        return false;
    }


    ProcessesStatisticsManager::~ProcessesStatisticsManager()
    {
        delete _processes_table;
        _root_dir_name.clear();
    }
}

