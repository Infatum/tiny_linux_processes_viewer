//
// Created by infatum on 4/21/17.
//

#ifndef LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H
#define LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <regex>

namespace linux_process_viewer {

    class Process_manager {
    private:
        static unsigned long long _lastTotalUser;
        static unsigned long long _lastTotalUserLow;
        static unsigned long long _lastTotalSys;
        static unsigned long long _lastTotalIdle;

    public:
        Process_manager();
        double getTotalCPUUsage();
        double getProcess_CPU_usage(int process_ID);
    };
}

#endif //LINUX_PROCESS_VIEWER_PROCESS_MANAGER_H
