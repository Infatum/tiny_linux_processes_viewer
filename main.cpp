#include "src/ProcessesStatisticsManager.h"
static const std::string root = "/proc";


int main() {
    linux_process_viewer::ProcessesStatisticsManager p(root);
    auto v = p.get_directories_names();
    p.show_proc_info(v);
    return 0;
}

