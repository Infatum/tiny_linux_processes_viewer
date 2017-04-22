//
// Created by infatum on 4/21/17.
//

#include "Process_manager.h"
#include "PID_Table.h"

namespace linux_process_viewer {

    unsigned long long Process_manager::_lastTotalUser;
    unsigned long long Process_manager::_lastTotalUserLow;
    unsigned long long Process_manager::_lastTotalSys;
    unsigned long long Process_manager::_lastTotalIdle;
    unsigned long long Process_manager::_physicallMemmoryCapacity;
    unsigned int Process_manager::_processorsCount;
    clock_t Process_manager::_lastCPU, Process_manager::_lastSysCPU, Process_manager::_lastUserCPU;

    Process_manager::Process_manager()
    {
        FILE *file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &_lastTotalUser, &_lastTotalUserLow, &_lastTotalSys, &_lastTotalIdle);
        fclose(file);
        file = fopen("/proc/meminfo", "r");
        if (file != nullptr)
            fscanf(file, "%*s %lu %*s", &_physicallMemmoryCapacity);
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
        _processorsCount = 0;
        while(fgets(line, 128, file) != NULL){
            if (strncmp(line, "processor", 9) == 0) _processorsCount++;
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
            fscanf(cmd_file_stream, "%*ld %*s %s %*ld", state);
        }

        fclose(cmd_file_stream);
        return state;
    }

    unsigned int Process_manager::calculate_mem_used_percent(unsigned int process_id)
    {
        double one_percent_kb = 0;
        one_percent_kb = _physicallMemmoryCapacity / 100;
        auto tmp_p = get_mem_used(process_id) / one_percent_kb;
        return round(tmp_p);
    }

    long unsigned int Process_manager::get_mem_used(unsigned int process_id)
    {
        long unsigned int mem;
        std::string file_name(BASE_PROC_PATH);
        file_name += std::to_string(process_id) + BASE_MEM_FILE;
        FILE *mem_info = fopen(file_name.c_str(), "r");

        if (mem_info != nullptr) {
            fscanf(mem_info, "%lu %*lu", &mem);
        }
        fclose(mem_info);
        return mem;
    }

    unsigned int Process_manager::convert_to_Kb(long unsigned bytes)
    {
        return bytes / 1024;
    }

    unsigned int Process_manager::convert_to_Mb(long unsigned Kb)
    {
        return  Kb / 1024;
    }

    std::string Process_manager::get_process_name(unsigned int process_id)
    {
        std::string filename(BASE_PROC_PATH);
        filename += std::to_string(process_id) + BASE_CPU_FILE;
        FILE *process_stat_f = fopen(filename.c_str(), "r");
        char *name = new char[256];

        if (process_stat_f != nullptr) {
            fscanf(process_stat_f, "%*d %s", name);
        }
        std::string proc_name(name);
        delete name;
        return proc_name;
    }

    /**
     * TODO: refresh procedure table instance each n seconds
     * @param seconds
     * @param proc_table
     */
    void Process_manager::refresh(std::chrono::seconds &seconds, PID_Table &proc_table)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto end = start + seconds;
        do {
            std::this_thread::yield();
        } while (std::chrono::system_clock::now() < end);

    }
}
