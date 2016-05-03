#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Command.h"
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

Command check_commands(string cmd) {
    if (cmd == "cat") {
        return Command::CAT;
    } else if (cmd == "cd") {
        return Command::CD;
    } else if (cmd == "copy") {
        return Command::COPY;
    } else if (cmd == "del") {
        return Command::DEL;
    } else if (cmd == "dir") {
        return Command::DIR;
    } else if (cmd == "help") {
        return Command::HELP;
    } else if (cmd == "info") {
        return Command::INFO;
    } else if (cmd == "quit") {
        return Command::QUIT;
    } else {
        cout << cmd << ": unrecognized command" << endl;
        return Command::UNKNOWN;
    }
}

int main(int argc, char** argv) {
    (void)argc;
    FAT fat = FAT();
    char* fs = fat.open_file(argv[1]);
    fat.init_entries((fs + FAT::DIR_OFFSET), 224, true);
    vector<string> input;
    string directory = "/";
    string command = "";
    bool cont = true;

    do {
        prompt(directory);
        input = read_cmdline();
        command = input.front();
        input.erase(input.begin());
        Command cmd = check_commands(command);
        if (cmd == Command::CAT) {
            fat.cat(input.front(), fs);
        } else if (cmd == Command::CD) {
            directory = fat.cd(input.front(), fs);
        } else if (cmd == Command::COPY) {
            fat.copy(input.front(), input.at(1));
        } else if (cmd == Command::DEL) {
            fat.del(input.front());
        } else if (cmd == Command::DIR) {
            fat.dir(input.front(), directory);
        } else if (cmd == Command::HELP) {
            fat.help();
        } else if (cmd == Command::INFO) {
            fat.info(input.front());
        } else if (cmd == Command::QUIT) {
            cont = false;
        }
    } while (cont);
/*
    //cout << "\n";
    fat.get_entries(fs);
    for (int i = 0; i < 10; i++) {
        fat.info(i, fs);
    }
*/
}





