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
        void init_system_res_info();
        void set_CPU_idle(PID_Table *pid);

    public:

        Process_manager();
        void set_process_status(unsigned int process_id, PID_Table *pid);
        unsigned long get_mem_used(unsigned int process_id);
        void set_memmory_used_percentage(unsigned process_id, PID_Table *);
        void set_process_name(unsigned process_id, PID_Table *);
        void set_CPU_time_usage(unsigned process_id, PID_Table *);
        void set_pid(unsigned int process_id, PID_Table *pid);
    };
}

#endif //LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H
