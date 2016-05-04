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
        void                  cat(std::string filename, char* entry_ptr);
        std::string           cd(std::string dir_name, char* entry_ptr);
        void                  copy(std::string source, std::string dest, char* entry_ptr);
        void                  del(std::string filename);
        void                  dir(std::string dir_name, char* entry_ptr);
        void                  help();
        void                  info(std::string name);
        std::vector<DirEntry> init_entries(char* fs, int max_entries, bool root_dir);
        char*                 open_file(std::string filename);
        void                  set_cur_dir(std::string new_dir);

        static const int ENTRY_OFFSET;
        static const int DATA_OFFSET;
        static const int DIR_OFFSET;
    private:
        void        copy_cluster(int src_cluster, int dest_cluster, char* entry_ptr);
        std::vector<int> find_free_clusters(int clusters_needed, char* entry_ptr);
        uint16_t    get_cluster(uint8_t byte1, uint8_t byte2, bool even_entry);
        std::string get_filename(int entry, char* start_byte);
        int         get_filesize(int entry, char* start_byte);
        int         get_first_cluster(int entry, char* start_byte);
        struct tm*  get_modified_time(int entry, char* start_byte);
        int         get_next_cluster(int entry, bool root_dir, char* entry_ptr);
        bool        is_directory(int entry, char* entry_ptr);
        void        print_dir(std::vector<DirEntry> entries);
        void        read_cluster(int num, char* entry_ptr);
        void        set_cluster(int val, char* entry_ptr, bool even_entry);
        void        update_fat(int entry, int next_entry, char* entry_ptr);
        void        insert_entry(std::string filename, std::string modified_date,
                                 int attributes, bool is_dir, int cluster, int filesize,
                                 char* entry_ptr, char* src_ptr);

        std::vector<DirEntry> root_entries;
        std::vector<DirEntry> cur_entries;
        std::string cur_dir;
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
