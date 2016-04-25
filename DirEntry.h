#ifndef __DIR_ENTRY__
#define __DIR_ENTRY__

#include <iostream>
#include <string>

class DirEntry {
    public:
        DirEntry();
    private:
        std::string filename;
        std::time_t creation_date;
        std::time_t creation_time;
        std::time_t accessed_date;
        std::time_t accessed_time;
        std::time_t modified_date;
        std::int cluster;
        std::int filesize;

}



#endif
