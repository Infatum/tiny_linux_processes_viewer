//
// Created by infatum on 4/21/17.
//

#ifndef LINUX_PROCESS_VIEWER_PID_TABLE_H
#define LINUX_PROCESS_VIEWER_PID_TABLE_H

#include <chrono>
#include <string>

namespace linux_process_viewer {

    struct PID_Table {
        std::string Process_name;
        unsigned int PID;
        unsigned int CPU_usage_old;
        unsigned int CPU_usage_new;
        unsigned int Memmory_usage;
        char Process_status;
        PID_Table() {}
        PID_Table(unsigned int pid, unsigned int cpu_u, unsigned int mem_u, char pr_s, std::string n = "undefined_process")
                : PID(pid), CPU_usage_old(cpu_u), CPU_usage_new(cpu_u), Memmory_usage(mem_u), Process_status(pr_s), Process_name(n) {}
        PID_Table(const PID_Table &pd_t);
        PID_Table & operator=(const PID_Table &pd_t);
        virtual ~PID_Table() {}
    };
}
#endif //LINUX_PROCESS_VIEWER_PID_TABLE_H
