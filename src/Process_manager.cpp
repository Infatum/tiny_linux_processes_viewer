//
// Created by infatum on 4/21/17.
//

#include "Process_manager.h"
#include <iostream>

namespace linux_process_viewer {

    unsigned long long Process_manager::_lastTotalUser;
    unsigned long long Process_manager::_lastTotalUserLow;
    unsigned long long Process_manager::_lastTotalSys;
    unsigned long long Process_manager::_lastTotalIdle;
    unsigned int Process_manager::_PID;
    clock_t Process_manager::_lastCPU, Process_manager::_lastSysCPU, Process_manager::_lastUserCPU;

    Process_manager::Process_manager()
    {
        FILE *file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &_lastTotalUser, &_lastTotalUserLow, &_lastTotalSys, &_lastTotalIdle);
        fclose(file);
    }

    double Process_manager::calculateTotalCpu_usage() {
        double percent;
        FILE *file;
        unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

        file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
               &totalSys, &totalIdle);
        fclose(file);

        if (totalUser < _lastTotalUser || totalUserLow < _lastTotalUserLow ||
            totalSys < _lastTotalSys || totalIdle < _lastTotalIdle) {
            //Overflow detection. Just skip this value.
            percent = -1.0;
        } else {
            total = (totalUser - _lastTotalUser) + (totalUserLow - _lastTotalUserLow) +
                    (totalSys - _lastTotalSys);
            percent = total;
            total += (totalIdle - _lastTotalIdle);
            percent /= total;
            percent *= 100;
        }
        _lastTotalUser = totalUser;
        _lastTotalUserLow = totalUserLow;
        _lastTotalSys = totalSys;
        _lastTotalIdle = totalIdle;

        return percent;
    }

    double Process_manager::getProcess_CPU_usage(unsigned int process_id)
    {
        initialize_process_info(process_id);
    }

    void Process_manager::initialize_process_info(unsigned int pid)
    {
        FILE* file;
        struct tms timeSample;
        char line[128];

        _lastCPU = times(&timeSample);
        _lastSysCPU = timeSample.tms_stime;
        _lastUserCPU = timeSample.tms_utime;

        file = fopen("/proc/cpuinfo", "r");
        _PID = 0;
        while(fgets(line, 128, file) != NULL){
            if (strncmp(line, "processor", 9) == 0) _PID++;
        }
        fclose(file);
    }

    char* Process_manager::get_process_Status(unsigned int process_id)
    {
        char *state = new char[2];
        std::string path_id_in_str = std::to_string(process_id);

        char *file_path = new char[strlen(BASE_PROC_PATH) + strlen(path_id_in_str.c_str()) + strlen(BASE_CPU_FILE)];
        strcpy(file_path, BASE_PROC_PATH); // copy /proc/
        strcpy(&file_path[strlen(BASE_PROC_PATH)], path_id_in_str.c_str()); // add process id
        strcpy(&file_path[strlen(BASE_PROC_PATH)+strlen(path_id_in_str.c_str())], BASE_CPU_FILE); // add filename

        FILE *cmd_file_stream;
        cmd_file_stream = fopen(file_path, "r");

        if (cmd_file_stream != nullptr) {
            fscanf(cmd_file_stream, "%*d %*s %s %*d", state);
        }
        fclose(cmd_file_stream);
        return state;
    }
}



