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

const int FAT::FAT_ENTRY_OFFSET = 512;
const int FAT::DIR_OFFSET = 9728;

FAT::FAT() {

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

void FAT::read_sector(int num, bool root_dir) {

}

/**
 * Initialize the program by putting the root directory entries into
 * a vector to make it simpler to traverse the directory entries later.
 */
void FAT::get_entries(char* fs) {
    vector<
    for (int i = 0; i < 224; i++) {
        char* entry_start = (i * 32) + FAT::DIR_OFFSET;

    }
}

/**
 * The filename is the first eight bytes of the directory, with the next
 * three bytes as the extension.
 */
string FAT::get_filename(int entry, char* fs) {

}

int FAT::get_next_sector(int num, bool root_dir) {
    if (root_dir) {
        return num + 1;
    } else {
        return -1; // TODO
    }
}






