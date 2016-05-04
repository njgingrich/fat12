#ifndef __DIR_ENTRY__
#define __DIR_ENTRY__

#include <ctime>
#include <iostream>
#include <string>

class DirEntry {
    public:
        DirEntry(
            std::string filename,
            std::string modified_date,
            int attributes,
            bool is_dir,
            int cluster,
            int filesize
        );
        DirEntry();
        std::string filename;
        std::string modified_date;
        int attributes;
        bool is_dir;
        int cluster;
        int filesize;
    private:

};



#endif
