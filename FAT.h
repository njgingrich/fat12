#ifndef __FAT_DRIVER__
#define __FAT_DRIVER__

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "DirEntry.h"

class FAT {
    public:
        FAT();
        char* dir(std::string dir_name);
        void read_sector(int num, bool root_dir);
        int get_next_sector(int num, bool root_dir);
        void get_entries(char* fs);
        std::string get_filename(int entry, char* start_byte);
        struct tm* get_modified_time(char* start_byte);
        std::string info(int entry, char* start_byte);
        int get_cluster_number(int entry, char* start_byte);

        static const int FAT_ENTRY_OFFSET;
        static const int DIR_OFFSET;
    private:
        char* open_file(std::string filename);

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
