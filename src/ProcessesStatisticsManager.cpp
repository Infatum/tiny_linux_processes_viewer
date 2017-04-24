//
// Created by infatum on 4/21/17.
//
#include <sys/param.h>
#include "ProcessesStatisticsManager.h"

namespace linux_process_viewer {

    std::vector<std::string> ProcessesStatisticsManager::update_pids() {
        DIR *dir;
        std::string dir_path;
        std::vector<std::string> directories_names;
        dirent *ent;

        _pids.clear();

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
        return directories_names;
    }


    bool ProcessesStatisticsManager::has_any_digits(const std::string &s) {
        return std::any_of(s.begin(), s.end(), ::isdigit);
    }


    void ProcessesStatisticsManager::draw_table(const std::vector<PID_Table *> statistics)
    {
        winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        for (int i = 0; i < statistics.size(); ++i) {
            printf("%6d | ", statistics[i]->PID);
            printf("  %c   | ", statistics[i]->state);
            printf("%3d%% | ", statistics[i]->CPU_percentage);
            printf("%3d%% | ", statistics[i]->Memmory_usage);
            printf("%s ", statistics[i]->name.c_str());
            printf("\n");
        }
        printf("\033[1m   PID | STATE |  CPU |  MEM | PROCESS NAME\033[0m\n");
    }

    /**
     * Calculates the average processes info statistics and returns a structure, collects data each update_time seconds
     * @param dir_paths
     * @param delay_time
     * @return average structure with process id, name, status, cpu and memmory usage in %
     */
    void ProcessesStatisticsManager::show_proc_info(std::vector<std::string> &dir_paths, unsigned delay_time) {
        Process_manager processManager;
        PID_Table *tmp_process = nullptr;

        // it's endless lup, don't show highlight! It's sooo spart! You so smart IDE...
        while (true) {
            update_pids();
            for (int i = 0; i < _pids.size(); ++i) {
                tmp_process = nullptr;
                // Find existed
                for (int ya_hz = 0; ya_hz < _proc_statistics.size(); ++ya_hz) {
                    if (_proc_statistics[ya_hz]->PID == _pids[i]) {
                        tmp_process = _proc_statistics[ya_hz];
                        break;
                    }
                }
                // or create new
                if (tmp_process == nullptr) {
                    tmp_process = new PID_Table();
                    _proc_statistics.push_back(tmp_process);
                }
                processManager.set_pid(_pids[i], tmp_process);
                processManager.set_process_name(_pids[i], tmp_process);
                processManager.set_memmory_used_percentage(_pids[i], tmp_process);
                processManager.set_CPU_time_usage(_pids[i], tmp_process);
                processManager.set_process_status(_pids[i], tmp_process);
                tmp_process->CPU_percentage = calculate_CPU_usage_by_process(tmp_process);
            }
            
            // delete old
            bool exist = false;
            for (int j = 0; j < _proc_statistics.size(); ++j) {
                exist = false;
                for (int x = 0; x < _pids.size(); ++x) {
                    if (_proc_statistics[j]->PID == _pids[x]) {
                        exist = true;
                    }
                }
                if (!exist) {
                    _proc_statistics.erase(_proc_statistics.begin() + j);
                }
            }

            sort_processes(flag);

            draw_table(_proc_statistics);
            sleep(delay_time);

        }
    }

    void ProcessesStatisticsManager::sort_processes(char flag)
    {
        switch (flag) {
            case SORT_CPU_FLAG:
                std::sort(_proc_statistics.begin(), _proc_statistics.end(),
                          [](const PID_Table *a, const PID_Table *b) {return a->CPU_percentage < b->CPU_percentage;});
                break;
            case SORT_MEM_FLAG:
                std::sort(_proc_statistics.begin(), _proc_statistics.end(),
                          [](const PID_Table *a, const PID_Table *b) {return a->Memmory_usage < b->Memmory_usage;});
                break;
            case SORT_PID_FLAG:
                std::sort(_proc_statistics.begin(), _proc_statistics.end(),
                          [](const PID_Table *a, const PID_Table *b) {return a->PID < b->PID;});
                break;
            case SORT_STATE_FLAG:
                std::sort(_proc_statistics.begin(), _proc_statistics.end(),
                          [](const PID_Table *a, const PID_Table *b) {return a->state < b->state;});
                break;
        }
    }

    float ProcessesStatisticsManager::calculate_CPU_usage_by_process(const PID_Table *pid)
    {
        float total_seconds_elapsed = float((pid->current_time_new - pid->current_time_old).total_milliseconds()) / 1000;
        float percent = (pid->CPU_usage_new - pid->CPU_usage_old) / total_seconds_elapsed;
        percent = roundf(percent);
        percent = MIN(MAX(percent, 0.0), 100.0);
        return percent;
    }

    /**
    * TODO: refresh procedure table instance each n seconds
    * @param seconds
    * @param proc_table
    */
    void ProcessesStatisticsManager::refresh(std::chrono::duration<int> &seconds) {

    }

    bool ProcessesStatisticsManager::files_names_match(const std::string &filename) {
        for (int j = 0; j < _files_to_process.size(); ++j) {
            if (filename == _files_to_process[j])
                return true;
        }
        return false;
    }

    ProcessesStatisticsManager::~ProcessesStatisticsManager() {
        delete _processes_table;
        _root_dir_name.clear();
    }
}

