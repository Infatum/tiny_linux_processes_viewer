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
#include "Directories.h"

namespace linux_process_viewer {

    class Process_manager {
    private:
        static unsigned long long _lastTotalUser;
        static unsigned long long _lastTotalUserLow;
        static unsigned long long _lastTotalSys;
        static unsigned long long _lastTotalIdle;
        static unsigned int _PID;
        static clock_t _lastCPU, _lastSysCPU, _lastUserCPU;
        void initialize_process_info(unsigned int pid);

    public:
        Process_manager();
        double calculateTotalCpu_usage();
        double getProcess_CPU_usage(unsigned int process_id);
        char* get_process_Status(unsigned int process_id);
    };
}

#endif //LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H
