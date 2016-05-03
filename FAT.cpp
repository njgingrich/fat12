#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
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

const int FAT::ENTRY_OFFSET = 512;
const int FAT::DATA_OFFSET = 15872;
const int FAT::DIR_OFFSET = 9728;

FAT::FAT() {
    cur_dir = "/";
}

void FAT::cat(string filename, char* entry_ptr) {
    int next_cluster = 0;
    for (uint16_t i = 0; i < cur_entries.size(); i++) {
        DirEntry e = cur_entries.at(i);
        if (e.filename == filename) {
            next_cluster = e.cluster;
            break;
        }
    }
    // TODO: check if directory
    if (next_cluster == 0) {
        cout << filename << ": no such file or directory" << endl;
        return;
    }
    while (next_cluster != 4095) {
        read_cluster(next_cluster, entry_ptr);
        next_cluster = get_next_cluster(next_cluster, false, entry_ptr);
    }
    cout << endl;
    return;
}

string FAT::cd(string dir_name, char* entry_ptr) {
    if (dir_name.empty()) {
        cout << "Usage: cd directory" << endl;
        return cur_dir;
    }
    string path = dir_name; // assume dir_name is an absolute path
    if (dir_name[0] != '/') {
        path = cur_dir + dir_name; // make dir an absolute path
    }
    std::stringstream ss(path);
    vector<string> directories;
    string d;
    while (std::getline(ss, d, '/')) {
        directories.push_back(d);
    }

    int cluster = 0;
    string dir = "/";
    for (uint16_t i = 0; i < directories.size(); i++) {
        for (uint16_t entry = 0; entry < cur_entries.size(); entry++) {
            DirEntry e = cur_entries.at(entry);
            if (e.filename == directories.at(i)) {
                if (!e.is_dir) {
                    cout << dir_name << " is not a directory" << endl;
                    return cur_dir;
                }
                dir.append(e.filename);
                cluster = e.cluster;
            }
        }
        if (dir.empty()) {
            cout << "The directory " << dir_name << " cannot be found" << endl;
            return cur_dir;
        }

        init_entries((entry_ptr + FAT::DATA_OFFSET + (cluster * 512)), 16, false);
    }
    cur_dir = path;
    return path;
}

void FAT::copy(string source, string dest) {
    if (source == "" || dest == "") {}
    return;
}

void FAT::del(string filename) {
    if (filename == "") {}
    return;
}

void FAT::dir(string dir_name, char* entry_ptr) {
    string path = dir_name; // assume dir_name is an absolute path
    if (dir_name[0] != '/') {
        path = cur_dir + dir_name; // make dir an absolute path
    }
    std::stringstream ss(path);
    vector<string> directories;
    string d;
    while (std::getline(ss, d, '/')) {
        directories.push_back(d);
    }

    int cluster = 0;
    for (uint16_t i = 0; i < directories.size(); i++) {
        if (i == 0) {
            for (uint16_t entry = 0; entry < root_entries.size(); entry++) {
                DirEntry e = root_entries.at(entry);
                if (e.filename == directories.at(i)) {
                    cout << "found right entry" << endl;
                    if (!e.is_dir) {
                        cout << dir_name << " is not a directory" << endl;
                        return;
                    }
                    break;
                }
            }
            init_entries(entry_ptr + FAT::DIR_OFFSET, 224, true);
        } else {
            for (uint16_t entry = 0; entry < cur_entries.size(); entry++) {
                DirEntry e = cur_entries.at(entry);
                if (e.filename == directories.at(i)) {
                    if (!e.is_dir) {
                        cout << dir_name << " is not a directory" << endl;
                        return;
                    }
                    cluster = e.cluster;
                    init_entries(entry_ptr + FAT::DATA_OFFSET + (cluster * 512), 16, false);
                    break;
                }
            }
            //init_entries(entry_ptr + FAT::DATA_OFFSET + (cluster * 512), 16, false);
        }
    }
    print_dir(cur_entries);
}

void FAT::print_dir(vector<DirEntry> entries) {
    for (DirEntry e : entries) {
        cout << e.modified_date << "  " << (e.is_dir ? "<DIR>" : "\t") << "\t";
        if (e.is_dir) {
            cout << "\t";
        } else {
            cout << setw(8) << right << e.filesize;
        }
        cout << "  " << e.filename << endl;
    }
    // TODO: print bytes free
}

void FAT::help() {
    cout << "Available commands:" << endl;
    cout << "dir [directory]" << endl;
    cout << "displays the contents of the named directory, or the current directory ";
    cout << "if directory is omitted" << endl;
    cout << endl;
    cout << "cd directory" << endl;
    cout << "changes the current directory in the floppy image to the named directory" << endl;
    cout << endl;
    cout << "info file" << endl;
    cout << "displays the starting sector and the directory entry offset for the named ";
    cout << "file/directory" << endl;
    cout << endl;
    cout << "cat file" << endl;
    cout << "displays the contents of the named file" << endl;
    cout << endl;
    cout << "del file" << endl;
    cout << "deletes the named file from the floppy disk image" << endl;
    cout << endl;
    cout << "copy file destination" << endl;
    cout << "copies the named file into the destination, which can specify either a filename ";
    cout << "or a directory" << endl;
    cout << endl;
    cout << "quit" << endl;
    cout << "terminates the program" << endl;
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

/**
 * Initialize the program by putting the root directory entries into
 * a vector to make it simpler to traverse the directory entries later.
 */
void FAT::init_entries(char* fs, int max_entries, bool root_dir) {
    struct tm* t;
    char* file_ptr = fs;
    string filename;
    string modified_date;
    cur_entries.clear();

    for (int entry = 0; entry < max_entries; entry++) {
        filename = get_filename(entry, file_ptr);
        if (filename == "") {
            break;
        }
        char buffer[80];
        t = get_modified_time(entry, file_ptr);
        strftime(buffer, 80, "%m/%d/%Y  %H:%M:%S", t);
        int filesize = get_filesize(entry, file_ptr);
        bool is_dir = is_directory(entry, file_ptr);
        int first_cluster = get_first_cluster(entry, file_ptr);
/*
        cout << "name: " << filename << endl;
        cout << "\t" << buffer << endl;
        cout << "\t" << filesize << endl;
        cout << "\t" << (is_dir ? "yes" : "no") << endl;
*/
        cur_entries.push_back(
            DirEntry(filename, string(buffer), 0, is_dir, first_cluster, filesize)
        );
        if (root_dir) {
            root_entries.push_back(
                DirEntry(filename, string(buffer), 0, is_dir, first_cluster, filesize)
            );
        }
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

void FAT::info(string name) {
    int cluster     = -1;
    int offset      = -1;
    string filename = "";

    for (uint8_t i = 0; i < cur_entries.size(); i++) {
        DirEntry e = cur_entries.at(i);

        if (e.filename == name) {
            filename = e.filename;
            cluster = e.cluster;
            offset = FAT::DIR_OFFSET + (i * 32);
        }
    }
    if (filename.empty()) {
        cout << "The file or directory " << name << " does not exist" << endl;
    } else {
        cout << "Directory entry offset: " << offset << endl;
        cout << "Starting sector: " << cluster << endl;
    }
}

int FAT::get_first_cluster(int entry, char* start_byte) {
    start_byte += (entry * 32) + 26;
    uint8_t second = start_byte[0];
    uint8_t first  = start_byte[1];
    int cluster = second + (first << 8);
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

int FAT::get_next_cluster(int entry, bool root_dir, char* entry_ptr) {
    if (root_dir) {
        return entry + 1;
    } else {
        entry_ptr += FAT::ENTRY_OFFSET;
        // first byte needed for entry is (3/2) * entry
        entry_ptr += (entry / 2) * 3;
        if (entry % 2 != 0) {
            entry_ptr += 1;
        }
        uint8_t byte1 = entry_ptr[0];
        uint8_t byte2 = entry_ptr[1];
        // cout << "byte1: " << +byte1 << " - byte2: " << +byte2 << endl;
        uint16_t cluster = 1;
        cluster = get_cluster(byte1, byte2, (entry % 2 == 0));
        return cluster;
    }
}

uint16_t FAT::get_cluster(uint8_t byte1, uint8_t byte2, bool even_entry) {
    /* even entry:
     * abcdefgh ijklmnop
     * mnopabcdefgh
     *
     * odd entry:
     * abcdefgh ijklmnop
     * ijklmnopabcd
     */

    uint16_t cluster = 0;
    if (even_entry) {
        // we want the right most 4 bits of the 2nd byte first
        cluster = byte1 + ((byte2 & 0x0F) << 8);
    } else {
        // we want the 2nd byte first then the first 4 bytes of byte 1
        cluster = byte2;
        cluster <<= 4;
        cluster += ((byte1 >> 4) & 0x0F);
    }
    return cluster;
}

void FAT::read_cluster(int num, char* entry_ptr) {
    entry_ptr += FAT::DATA_OFFSET + (num * 512);
    for (int i = 0; i < 512; i++) {
        if (entry_ptr[i] == 0) {
            break;
        }
        cout << entry_ptr[i];
    }
}

void FAT::set_cur_dir(string new_dir) {
    cur_dir = new_dir;
}






