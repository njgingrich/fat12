#include <ctime>
#include <iostream>
#include <string>

#include "DirEntry.h"

using std::string;

DirEntry::DirEntry(
            std::string filename,
            std::time_t creation_date,
            std::time_t creation_time,
            std::time_t accessed_date,
            std::time_t accessed_time,
            std::time_t modified_date,
            int cluster,
            int filesize) {
    this->filename = filename;
    this->creation_date = creation_date;
    this->creation_time = creation_time;
    this->accessed_date = accessed_date;
    this->accessed_time = accessed_time;
    this->modified_date = modified_date;
    this->cluster = cluster;
    this->filesize = filesize;
}
