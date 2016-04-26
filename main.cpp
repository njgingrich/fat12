#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "FAT.h"

using std::cout;
using std::cin;
using std::istringstream;
using std::endl;
using std::string;
using std::vector;


/**
 * Read in the command line input.
 *
 * @return vector<string> of input, first element is command, rest are args.
 */
vector<string> read_cmdline() {
    string t;
    vector<string> input;
    getline(cin, t);
    istringstream iss(t);
    string word;
    while (iss >> word) {
        input.push_back(word);
    }
}

int main(int argc, char** argv) {
    FAT fat = FAT();
/*    vector<string> input;
    bool cont = true;
    do {
        input = read_cmdline();
    } while (cont);
*/
    char* fs = fat.dir("floppy.img");
    //cout << "\n";
    fat.get_entries(fs);
}





