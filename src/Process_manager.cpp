//
// Created by infatum on 4/21/17.
//

#include "Process_manager.h"

namespace linux_process_viewer {

    unsigned long long Process_manager::_lastTotalUser;
    unsigned long long Process_manager::_lastTotalUserLow;
    unsigned long long Process_manager::_lastTotalSys;
    unsigned long long Process_manager::_lastTotalIdle;
    unsigned long long Process_manager::_physicallMemmoryCapacity;
    unsigned int Process_manager::_processorsCount;

    Process_manager::Process_manager() {
        init_system_res_info();
    }

    void Process_manager::init_system_res_info() {
        FILE *file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &_lastTotalUser, &_lastTotalUserLow, &_lastTotalSys, &_lastTotalIdle);
        fclose(file);
        file = fopen("/proc/meminfo", "r");

        if (file == nullptr)
            return;

        fscanf(file, "%*s %lu %*s", &_physicallMemmoryCapacity);
        fclose(file);
    }

    /**
     * Reads /proc/stat and returns the idle time for each CPU core in a vector
     * @return idle time for each CPU core
     */
    void Process_manager::set_CPU_idle(PID_Table *pid) {
        std::ifstream in("/proc/stat");
        std::vector<long long> result;
        //TODO: Rewrite to generic version, 'cause this might broke if there are not 10 columns in /proc/stat.
        regex reg("cpu(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+)");
        std::string line;

        if (pid->CPU_total_idle_old.size() != 0 || pid->CPU_total_idle_new.size() != 0) {
            pid->CPU_total_idle_old.swap(pid->CPU_total_idle_new);
            pid->CPU_total_idle_new.clear();
        }
        while (std::getline(in, line)) {
            smatch match;
            if (boost::regex_match(line, match, reg)) {
                long long idle_time = lexical_cast<long long>(match[5]);
                if (pid->CPU_total_idle_new.size() == 0 && pid->CPU_total_idle_old.size() == 0)
                    pid->CPU_total_idle_new.push_back(idle_time);
                else {
                    pid->CPU_total_idle_new.push_back(idle_time);
                }
            }
        }
    }

    void Process_manager::set_CPU_time_usage(unsigned int process_id, PID_Table *pid) {
        std::string process_filename(BASE_PROC_PATH);
        process_filename += std::to_string(process_id);
        process_filename += BASE_CPU_FILE;

        FILE *proc_f = fopen(process_filename.c_str(), "r");
        if (proc_f == nullptr)
            return;

        unsigned long utime = 0, stime = 0;

        fscanf(proc_f, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu", &utime, &stime);
        fclose(proc_f);

        pid->CPU_usage_old = pid->CPU_usage_new;
        pid->CPU_usage_new = utime + stime;
        pid->current_time_old = pid->current_time_new;
        pid->current_time_new = date_time::microsec_clock<posix_time::ptime>::universal_time();
    }

    void Process_manager::set_process_status(unsigned int process_id, PID_Table *pid) {
        char *state = new char[2];
        std::string path_id_in_str = std::to_string(process_id);

        char *file_path = new char[strlen(BASE_PROC_PATH) + strlen(path_id_in_str.c_str()) + strlen(BASE_CPU_FILE)];
        strcpy(file_path, BASE_PROC_PATH); // copy /proc/
        strcpy(&file_path[strlen(BASE_PROC_PATH)], path_id_in_str.c_str()); // add process id
        strcpy(&file_path[strlen(BASE_PROC_PATH) + strlen(path_id_in_str.c_str())], BASE_CPU_FILE); // add filename

        FILE *cmd_file_stream;
        cmd_file_stream = fopen(file_path, "r");
        if (cmd_file_stream == nullptr)
            return;

        fscanf(cmd_file_stream, "%*ld %*s %s %*ld", state);
        fclose(cmd_file_stream);

        pid->state = state[0];
    }

    /**
     * Calculates % of memmory used by a certain process
     * @param process_id
     * @return percent of memmory used by process with the certain proccess id
     */
    void Process_manager::set_memmory_used_percentage(unsigned int process_id, PID_Table *pid) {
        double one_percent_kb = 0;
        one_percent_kb = _physicallMemmoryCapacity / 100;
        auto tmp_p = get_mem_used(process_id) / one_percent_kb;
        pid->Memmory_usage = round(tmp_p);
    }

    /**
     * Get full memmory used by a certain process in Kb
     * @param process_id
     * @return memmory used by process in kb
     */
    long unsigned int Process_manager::get_mem_used(unsigned int process_id) {
        long unsigned int mem;
        std::string file_name(BASE_PROC_PATH);
        file_name += std::to_string(process_id) + BASE_MEM_FILE;
        FILE *mem_info = fopen(file_name.c_str(), "r");

        if (mem_info == nullptr)
            return -1;

        fscanf(mem_info, "%lu %*lu", &mem);
        fclose(mem_info);
        return mem;
    }

    void Process_manager::set_pid(unsigned process_id, PID_Table *pid) {
        pid->PID = process_id;
    }

    void Process_manager::set_process_name(unsigned int process_id, PID_Table *pid) {
        std::string filename(BASE_PROC_PATH);
        filename += std::to_string(process_id) + BASE_CPU_FILE;
        FILE *process_stat_f = fopen(filename.c_str(), "r");
        char *name = new char[256];

        if (process_stat_f == nullptr)
            return;

        fscanf(process_stat_f, "%*d %s", name);
        fclose(process_stat_f);

        std::string proc_name(name);

        // oops, already empty
        if (strlen(name) == 0) {
            pid->name = proc_name;
            return;
        }

        proc_name.erase(0, 1); //erase "("
        proc_name.erase(proc_name.length() - 1, proc_name.length()); //erase ")"
        delete name;
        pid->name = proc_name;
    }
}
