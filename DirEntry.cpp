#include <ctime>
#include <iostream>
#include <string>

#include "DirEntry.h"

using std::string;

DirEntry::DirEntry(
            std::string filename,
            std::string modified_date,
            int attributes,
            bool is_dir,
            int cluster,
            int filesize) {
    this->filename = filename;
    this->modified_date = modified_date;
    this->attributes = attributes;
    this->is_dir = is_dir;
    this->cluster = cluster;
    this->filesize = filesize;
}

DirEntry::DirEntry() {

}
