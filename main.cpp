#include "src/DirectoryParser.h"
static const std::string root = "/proc";


int main() {
    linux_process_viewer::DirectoryParser p(root);
    auto v = p.get_directories_names();
    p.process_files(v);
    return 0;
}

