//
// Created by infatum on 4/21/17.
//

#ifndef LINUX_PROCESS_VIEWER_PID_TABLE_H
#define LINUX_PROCESS_VIEWER_PID_TABLE_H

#include <chrono>
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace linux_process_viewer {

    struct PID_Table {

        unsigned int PID;
        unsigned long CPU_usage_old;
        unsigned long CPU_usage_new;
        unsigned int Memmory_usage;
        std::vector<long long> CPU_total_idle_new;
        std::vector<long long> CPU_total_idle_old;
        boost::posix_time::ptime current_time_new;
        boost::posix_time::ptime current_time_old;
        unsigned int CPU_percentage;
        std::string name;
        char state;
        PID_Table();
        PID_Table(unsigned int pid, unsigned long cpu_u, unsigned int mem_u, char pr_s, std::vector<long long> cpu_idle,
                  std::string n = "undefined_process")
                : PID(pid), CPU_usage_old(cpu_u), CPU_usage_new(cpu_u), Memmory_usage(mem_u),
                  state(pr_s), name(n), CPU_total_idle_new(cpu_idle), CPU_total_idle_old(cpu_idle) {}
        PID_Table(const PID_Table &pd_t);
        PID_Table & operator=(const PID_Table &pd_t);
        virtual ~PID_Table() {}
    };
}
#endif //LINUX_PROCESS_VIEWER_PID_TABLE_H
