#ifndef __FAT_DRIVER__
#define __FAT_DRIVER__

#include <iostream>
#include <string>

class FAT {
    public:
        FAT();
        void dir(std::string dir_name);
        void read_sector(int num, bool root_dir);
        int get_next_sector(int num, bool root_dir);
        std::string get_filename(int entry);

    private:
        char* open_file(std::string filename);
        static const int FAT_ENTRY_OFFSET;
        static const int DIR_OFFSET;
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
