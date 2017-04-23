//
// Created by infatum on 4/23/17.
//

#include "PID_Table.h"

namespace linux_process_viewer {

    PID_Table::PID_Table(const PID_Table &pd_t)
    {
        if (&pd_t == this)
            return;
        PID = pd_t.PID;
        Process_name = pd_t.Process_name;
        CPU_usage_old = pd_t.CPU_usage_old;
        CPU_usage_new = pd_t.CPU_usage_new;
        Memmory_usage = pd_t.Memmory_usage;
        Process_status = pd_t.Process_status;
    }

    PID_Table& PID_Table::operator=(const PID_Table &pd_t)
    {
        if (&pd_t == this)
            return *this;
        PID = pd_t.PID;
        Process_name = pd_t.Process_name;
        CPU_usage_old = pd_t.CPU_usage_old;
        CPU_usage_new = pd_t.CPU_usage_new;
        Memmory_usage = pd_t.Memmory_usage;
        Process_status = pd_t.Process_status;
        return *this;
    }
}
