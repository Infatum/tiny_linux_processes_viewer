//
// Created by infatum on 4/21/17.
//

#ifndef LINUX_PROCESS_VIEWER_PID_TABLE_H
#define LINUX_PROCESS_VIEWER_PID_TABLE_H

#include <chrono>

namespace linux_process_viewer {

    struct PID_Table {
        unsigned int PID;
        unsigned int CPU_usage;
        unsigned int Memmory_usage;
        char Process_status;
        PID_Table() {}
        PID_Table(unsigned int pid, unsigned int cpu_u, unsigned int mem_u, char pr_s)
                : PID(pid), CPU_usage(cpu_u), Memmory_usage(mem_u), Process_status(pr_s) {}
    };
}
#endif //LINUX_PROCESS_VIEWER_PID_TABLE_H
