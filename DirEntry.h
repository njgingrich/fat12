#ifndef __DIR_ENTRY__
#define __DIR_ENTRY__

#include <ctime>
#include <iostream>
#include <string>

class DirEntry {
    public:
        DirEntry(
            std::string filename,
            std::time_t creation_date,
            std::time_t creation_time,
            std::time_t accessed_date,
            std::time_t accessed_time,
            std::time_t modified_date,
            int cluster,
            int filesize
        );
    private:
        std::string filename;
        std::time_t creation_date;
        std::time_t creation_time;
        std::time_t accessed_date;
        std::time_t accessed_time;
        std::time_t modified_date;
        int cluster;
        int filesize;

};



#endif
