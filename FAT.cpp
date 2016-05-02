#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
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
using std::right;
using std::setw;
using std::string;
using std::time_t;
using std::vector;

const int FAT::FAT_ENTRY_OFFSET = 512;
const int FAT::DIR_OFFSET = 9728;

FAT::FAT() {

}

void FAT::dir(string dir_name, char* fs) {
    char* file_ptr = fs + (9728);
    if (file_ptr) {}
    if (dir_name == "") {
        dir_name = "/";
    }

    for (DirEntry e : entries) {
        cout << e.modified_date << "  " << (e.is_dir ? "<DIR>" : "\t") << "\t";
        if (e.is_dir) {
            cout << "\t";
        } else {
            cout << setw(8) << right << e.filesize;
        }
        cout << "  " << e.filename << endl;
    }
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
    if (num) {}
    if (root_dir) {}
}

/**
 * Initialize the program by putting the root directory entries into
 * a vector to make it simpler to traverse the directory entries later.
 */
void FAT::init_entries(char* fs) {
    struct tm* t;
    char* file_ptr = fs + (9728);
    string filename;
    string modified_date;

    for (int entry = 0; entry < 224; entry++) {
        filename = get_filename(entry, file_ptr);
        if (filename == "") {
            break;
        }
        char buffer[80];
        t = get_modified_time(entry, file_ptr);
        strftime(buffer, 80, "%m/%d/%Y  %H:%M:%S", t);
        int filesize = get_filesize(entry, file_ptr);
        bool is_dir = is_directory(entry, file_ptr);/*
        cout << "name: " << filename << endl;
        cout << "\t" << buffer << endl;
        cout << "\t" << filesize << endl;
        cout << "\t" << (is_dir ? "yes" : "no") << endl;*/
        entries.push_back(DirEntry(filename, string(buffer), 0, is_dir, 0, filesize));
    }
}

/**
 * The filename is the first eight bytes of the directory, with the next
 * three bytes as the extension.
 */
string FAT::get_filename(int entry, char* start_byte) {
    start_byte += (entry * 32);
    string filename = "";
    for (int i = 0; i < 8; i++) {
        if (start_byte[i] != ' ' && start_byte[i] != 0) {
            filename.push_back(start_byte[i]);
        }
    }
    if (is_directory(0, start_byte) || filename.empty()) {
        return filename;
    }
    filename.push_back('.');
    for (int i = 8; i < 11; i++) {
        if (start_byte[i] != ' ') {
            filename.push_back(start_byte[i]);
        }
    }
    return filename;
}

bool FAT::is_directory(int entry, char* entry_ptr) {
    entry_ptr += (entry * 32) + 11;
    return (entry_ptr[0] == 16);
}

struct tm* FAT::get_modified_time(int entry, char* start_byte) {
    start_byte += (entry * 32) + 22; // offset for first byte of creation time

    struct tm* mod_time;
    time_t rawtime;
    time(&rawtime);
    mod_time = localtime(&rawtime);

    // 15-11 = hour, 10-5 = min, 4-0 = sec
    mod_time->tm_sec  = (start_byte[0] & 0b00011111) * 2; // to the nearest 2s
    mod_time->tm_min  = ((start_byte[0] >> 5) & 0b00000111) + ((start_byte[1] & 0b00000111) << 3);
    mod_time->tm_hour = (start_byte[1] >> 3);
    //cout << "\t" << hour << ":" << min << ":" << sec;

    start_byte += 2;
    // 15-9 = year (0=1980), 8-5 = month, 4-0 = day
    mod_time->tm_mday = (start_byte[0] & 0b00011111);
    mod_time->tm_mon  = ((start_byte[0] >> 5) & 0b00000111) + ((start_byte[1] & 0b00000001) << 1);
    mod_time->tm_mon -= 1; // because strftime() prints month 1-12
    mod_time->tm_year = (start_byte[1] >> 1) + 80;
    //cout << "\t" << month << "/" << day << "/" << year;

    //return date;
    return mod_time;
}

string FAT::info(string name, char* start_byte) {
    int entry = 4; // todo remove
    if (name == "") {}
    int offset = FAT::DIR_OFFSET + (entry * 32);
    int cluster = get_cluster_number(entry, start_byte);
    cout << "Directory entry offset: " << offset << endl;
    cout << "Starting sector: " << cluster << endl;
    return "";
}

int FAT::get_cluster_number(int entry, char* start_byte) {
    start_byte += (entry * 32) + 26; //(FAT::DIR_OFFSET + (entry * 32)) + 26;
    int cluster = start_byte[0] + (start_byte[1] << 8);
    return cluster;
}

int FAT::get_filesize(int entry, char* start_byte) {
    start_byte += (entry * 32) + 28; // offset for 4 bytes of filesize
    uint8_t fourth = start_byte[0];
    uint8_t third  = start_byte[1];
    uint8_t second = start_byte[2];
    uint8_t first  = start_byte[3];
    int filesize = fourth + (third << 8) + (second << 16) + (first << 24);

    return filesize;
}

int FAT::get_next_sector(int num, bool root_dir) {
    if (root_dir) {
        return num + 1;
    } else {
        return -1; // TODO
    }
}






