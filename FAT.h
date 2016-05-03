#ifndef __FAT__
#define __FAT__

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "DirEntry.h"

class FAT {
    public:
        FAT();
        void dir(std::string dir_name, char* fs);
        void read_sector(int num, bool root_dir);
        int get_next_sector(int num, bool root_dir);
        void init_entries(char* fs);
        char* open_file(std::string filename);
        std::string get_filename(int entry, char* start_byte);
        struct tm* get_modified_time(int entry, char* start_byte);
        void info(std::string name);
        int get_first_cluster(int entry, char* start_byte);
        bool is_directory(int entry, char* entry_ptr);
        int get_filesize(int entry, char* start_byte);

        static const int FAT_ENTRY_OFFSET;
        static const int DIR_OFFSET;
    private:
        std::vector<DirEntry> entries;

};

/**
 * bytes 0-7   = filename
 * bytes 8-10  = extension
 * byte 11     = attributes
 * byte 12     = reserved
 * byte 13     = fine creation time
 * bytes 14-15 = creation time
 * bytes 16-17 = creation date
 * bytes 18-19 = last accessed date
 * bytes 20-21 = unused
 * bytes 22-23 = last accessed time
 * bytes 24-25 = last modified date
 * bytes 26-27 = cluster number
 * bytes 28-31 = filesize
 */

#endif
