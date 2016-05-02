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

bool check_commands(string cmd, vector<string> args, FAT fat, char* fs) {
    if (args.empty()) { // fill the args array with empty data
        args.push_back("");
        args.push_back("");
    }

    if (cmd == "dir") {
        fat.dir(args.front(), fs);
    } else if (cmd == "info") {
        fat.info(args.front(), fs);
    } else if (cmd == "quit") {
        return false;
    } else {
        cout << cmd << ": unrecognized command" << endl;
    }
    return true;
}

int main(int argc, char** argv) {
    (void)argc;
    FAT fat = FAT();
    char* fs = fat.open_file(argv[1]);
    fat.init_entries(fs);
    vector<string> input;
    bool cont = true;
    string directory = "/";
    string command = "";

    do {
        prompt(directory);
        input = read_cmdline();
        command = input.front();
        input.erase(input.begin());
        cont = check_commands(command, input, fat, fs);
    } while (cont);
/*
    //cout << "\n";
    fat.get_entries(fs);
    for (int i = 0; i < 10; i++) {
        fat.info(i, fs);
    }
*/
}





