#include <cstdlib>
#include <ctime>
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
using std::time_t;
using std::vector;

const int FAT::FAT_ENTRY_OFFSET = 512;
const int FAT::DIR_OFFSET = 9728;

FAT::FAT() {

}

char* FAT::dir(string dir_name) {
    char* fs = open_file(dir_name);
    char* first_file_name = fs + (9728);
    cout << get_filename(0, first_file_name);
    return first_file_name;
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
    cout << endl;
    for (int i = 0; i < 30; i++) {
        char* entry_start = fs + (i * 32);
        cout << "entry " << i << ": " << get_filename(i, entry_start) << endl;
        cout << "\t" << get_creation_time(entry_start) << endl;
    }
}

/**
 * The filename is the first eight bytes of the directory, with the next
 * three bytes as the extension.
 */
string FAT::get_filename(int entry, char* start_byte) {
    string filename;
    for (int i = 0; i < 8; i++) {
        if (start_byte[i] != ' ') {
            filename.push_back(start_byte[i]);
        }
    }
    filename.push_back('.');
    for (int i = 8; i < 11; i++) {
        if (start_byte[i] != ' ') {
            filename.push_back(start_byte[i]);
        }
    }
    return filename;
}

tm FAT::get_creation_time(char* start_byte) {
    start_byte += 14; // offset for first byte of creation time
    date.push_back(start_byte[0]);
    date.push_back(start_byte[1]);

    struct tm* create_time;
    create_time->tm_sec  = (start_byte[1] & 0b00011111) * 2; // to the nearest 2s
    create_time->tm_min  = (start_byte[1] >> 3) + (start_byte[0] & 0b00000111);
    create_time->tm_hour = (start_byte[0] >> 3);

    return date;
}

int FAT::get_next_sector(int num, bool root_dir) {
    if (root_dir) {
        return num + 1;
    } else {
        return -1; // TODO
    }
}






