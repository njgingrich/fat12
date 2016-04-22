#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "FAT.h"

using std::cout;
using std::endl;
using std::string;

FATDriver::FAT() {

}

void FAT::dir(string dir_name) {
    char* fs = open_file(dir_name);
    char* first_file_name = fs + (9728);
    for (int i = 0; i < 11; i++) {
        cout << first_file_name[i];
    }
    cout << endl;
}

char* FAT::open_file(std::string filename) {
    int input_file = open(filename.c_str(), O_RDONLY);
    struct stat file_info;
    if (stat(filename.c_str(), &file_info) != 0) {
        perror("Opening input file");
        exit(-1);
    }
    int file_size = file_info.st_size;
    char* fs = (char*)
    mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, input_file, 0);
    if (fs == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }
    return fs;
}
