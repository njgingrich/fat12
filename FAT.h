ifndef __FAT_DRIVER__
#define __FAT_DRIVER__

#include <iostream>
#include <string>

class FAT {
    public:
        FATDriver();
        void dir(std::string dir_name);
    private:
        char* open_file(std::string filename);
};

#endif
