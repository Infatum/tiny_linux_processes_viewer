//
// Created by infatum on 4/23/17.
//

#include "PID_Table.h"

namespace linux_process_viewer {

    PID_Table::PID_Table()
    {
        PID = 0;
        CPU_usage_old = 0;
        CPU_usage_new = 0;
        Memmory_usage = 0;
        state = ' ';
        name = "";
    }

    PID_Table::PID_Table(const PID_Table &pd_t)
    {
        if (&pd_t == this)
            return;
        PID = pd_t.PID;
        name = pd_t.name;
        CPU_usage_old = pd_t.CPU_usage_old;
        CPU_usage_new = pd_t.CPU_usage_new;
        Memmory_usage = pd_t.Memmory_usage;
        state = pd_t.state;
        CPU_total_idle_old.clear();
        CPU_total_idle_new.clear();

        for (int i = 0; i < pd_t.CPU_total_idle_old.size() ; ++i) {
            CPU_total_idle_old.push_back(pd_t.CPU_total_idle_old[i]);
        }
        for (int j = 0; j < pd_t.CPU_total_idle_new.size() ; ++j) {
            CPU_total_idle_new.push_back(pd_t.CPU_total_idle_new[j]);
        }
    }

    PID_Table& PID_Table::operator=(const PID_Table &pd_t)
    {
        if (&pd_t == this)
            return *this;
        PID = pd_t.PID;
        name = pd_t.name;
        CPU_usage_old = pd_t.CPU_usage_old;
        CPU_usage_new = pd_t.CPU_usage_new;
        Memmory_usage = pd_t.Memmory_usage;
        state = pd_t.state;

        CPU_total_idle_new.clear();
        CPU_total_idle_old.clear();

        for (int i = 0; i < pd_t.CPU_total_idle_old.size() ; ++i) {
            CPU_total_idle_old.push_back(pd_t.CPU_total_idle_old[i]);
        }
        for (int j = 0; j < pd_t.CPU_total_idle_new.size() ; ++j) {
            CPU_total_idle_new.push_back(pd_t.CPU_total_idle_new[j]);
        }
        return *this;
    }
}
