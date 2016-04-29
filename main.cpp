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
    return input;
}

void prompt(string dir) {
    cout << dir << ">";
}

bool check_commands(string cmd) {
    if (cmd == "dir") {
        dir();
    } else if (cmd == "info") {
        info();
    } else if (cmd == "quit") {
        return false;
    } else {
        cout << cmd << ": unrecognized command" << endl;
    }
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    FAT fat = FAT();
    char* fs = fat.open_file(argv[1]);
    vector<string> input;
    bool cont = true;
    string directory = "/";

    do {
        prompt(directory);
        input = read_cmdline();
        cont = check_commands(input.front());
    } while (cont);
/*
    //cout << "\n";
    fat.get_entries(fs);
    for (int i = 0; i < 10; i++) {
        fat.info(i, fs);
    }
*/
}





