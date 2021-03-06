#include "src/ProcessesStatisticsManager.h"
static const std::string root = "/proc";

char flag = SORT_PID_FLAG;


int main(int argc, const char *argv[]) {
    if (argc > 2 && (strcmp(argv[1], "--SORT") == 0 ||  strcmp(argv[1], "--sort") == 0)) {
        switch (argv[2][0]) {
            case SORT_CPU_FLAG:
                flag = SORT_CPU_FLAG;
                break;
            case SORT_MEM_FLAG:
                flag = SORT_MEM_FLAG;
                break;
            case SORT_STATE_FLAG:
                flag = SORT_STATE_FLAG;
                break;
            case SORT_PID_FLAG:
                flag = SORT_PID_FLAG;
                break;
        }
    } else {
        printf("Usage: %s --sort (p|c|m|s)", argv[0]);
        printf("\n p - PID\n c - CPU\n m - MEM\n s - STATE\n");
        return 1;
    }
    linux_process_viewer::ProcessesStatisticsManager p(root);
    auto v = p.update_pids();
    p.show_proc_info(v, 10);
    return 0;
}

