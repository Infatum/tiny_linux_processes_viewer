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

    Process_manager::Process_manager()
    {
        init_system_res_info();
    }

    void Process_manager::init_system_res_info()
    {
        FILE *file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &_lastTotalUser, &_lastTotalUserLow, &_lastTotalSys, &_lastTotalIdle);
        fclose(file);
        file = fopen("/proc/meminfo", "r");
        if (file != nullptr)
            fscanf(file, "%*s %lu %*s", &_physicallMemmoryCapacity);
    }

    /**
     * Reads /proc/stat and returns the idle time for each CPU core in a vector
     * @return idle time for each CPU core
     */
    std::vector<long long> Process_manager::get_total_CPU_idle()
    {
        std::ifstream in( "/proc/stat" );
        std::vector<long long> result;

        //TODO: Rewrite to generic version, 'cause this might broke if there are not 11 columns in /proc/stat.
        regex reg("cpu(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+)");

        std::string line;
        while ( std::getline(in, line) ) {

            smatch match;
            if ( boost::regex_match( line, match, reg ) ) {

                long long idle_time = lexical_cast<long long>(match[5]);

                result.push_back(idle_time);
            }
        }
        return result;
    }

    /**
     * Calculate the avarage CPU load % in the next interval_seconds for each CPU core
     * @param time interval in seconds
     * @return avarage CPU load % during certain time interval
     */
    std::vector<float> Process_manager::get_total_CPU_usage(unsigned interval_seconds)
    {
        posix_time::ptime current_time_1 = date_time::microsec_clock<posix_time::ptime>::universal_time();
        std::vector<long long> idle_time_1 = get_total_CPU_idle();

        sleep(interval_seconds);

        auto current_time_2 = date_time::microsec_clock<posix_time::ptime>::universal_time();
        std::vector<long long> idle_time_2 = get_total_CPU_idle();

        //Time measuring
        const float total_seconds_elpased = float((current_time_2 - current_time_1).total_milliseconds()) / 1000.f;

        std::vector<float> cpu_loads;

        for ( unsigned i = 0; i < idle_time_1.size(); ++i ) {

            float idle_diff = float(idle_time_2[i] - idle_time_1[i]);
            idle_diff /= total_seconds_elpased; // Get diff per one second
            const float load = 100.f - idle_diff; // "Convert" idle to load
            cpu_loads.push_back( load );

        }
        return cpu_loads;
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

    /**
     * Calculates % of memmory used by a certain process
     * @param process_id
     * @return percent of memmory used by process with the certain proccess id
     */
    unsigned int Process_manager::calculate_memm_used_by_process(unsigned int process_id)
    {
        double one_percent_kb = 0;
        one_percent_kb = _physicallMemmoryCapacity / 100;
        auto tmp_p = get_mem_used(process_id) / one_percent_kb;
        return round(tmp_p);
    }

    /**
     * Get full memmory used by a certain process in Kb
     * @param process_id
     * @return memmory used by process in kb
     */
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
        proc_name.erase(0,1); //erase "("
        proc_name.erase(proc_name.length() - 1, proc_name.length()); //erase ")"
        delete name;
        return proc_name;
    }

    /**
     * TODO: refresh procedure table instance each n seconds
     * @param seconds
     * @param proc_table
     */
    void Process_manager::refresh(std::chrono::seconds &seconds)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto end = start + seconds;
        do {
            std::this_thread::yield();
        } while (std::chrono::system_clock::now() < end);

    }
}
