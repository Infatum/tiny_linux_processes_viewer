//
// Created by infatum on 4/21/17.
//

#ifndef LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H
#define LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <regex>
#include <fstream>
#include <string.h>
#include <sys/times.h>
#include <sys/vtimes.h>
#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Directories.h"
#include "PID_Table.h"

namespace linux_process_viewer {
    using namespace boost;
    class Process_manager {
    private:
        static unsigned long long _lastTotalUser;
        static unsigned long long _lastTotalUserLow;
        static unsigned long long _lastTotalSys;
        static unsigned long long _lastTotalIdle;
        static unsigned int _processorsCount;
        static unsigned long long _physicallMemmoryCapacity;
        void refresh(std::chrono::seconds &seconds);
        void init_system_res_info();
        std::vector<long long int> get_total_CPU_idle();

    public:
        Process_manager();
        char* get_process_Status(unsigned int process_id);
        unsigned long get_mem_used(unsigned int process_id);
        unsigned int convert_to_Kb(unsigned long bytes);
        unsigned int convert_to_Mb(unsigned long Kb);
        unsigned int calculate_memm_used_by_process(unsigned int process_id);
        std::string get_process_name(unsigned int process_id);
        std::vector<float> get_total_CPU_usage(unsigned int interval_seconds);

    };
}

#endif //LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H
