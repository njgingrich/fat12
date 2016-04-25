ifndef __FAT_DRIVER__
#define __FAT_DRIVER__

#include <iostream>
#include <string>

class FAT {
    public:
        FATDriver();
        void dir(std::string dir_name);
        void read_sector(int num, bool is_root_dir);
        void get_next_sector(int num, bool is_root_dir);
        
    private:
        char* open_file(std::string filename);
};

#endif
