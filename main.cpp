#include "DirectoryParser.h"
static const std::string root = "/proc";


int main() {
    linux_process_viewer::DirectoryParser p(root);
    auto v = p.get_directories_names();
    p.process_files(v);
//    for (int i = 0; i < v.size(); ++i) {
//        if (i == v.size()-1) {
//            std::cout <<  "\t" << v[i] << "." << std::endl;
//            break;
//        }
//        std::cout << "\t" << v[i] << ",\t\t ";
//    }
    return 0;
}

