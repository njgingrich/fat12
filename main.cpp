#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
    istringsteam iss(t);
    string word;
    while (iss >> word) {
        input.insert(word);
    }
}

int main(int argc, char** argv) {
    FATDriver fat = FATDriver();
    vector<string> input;
    bool continue = true;
    do {
        input = read_cmdline();
    } while (continue);

    fat.dir(input.front());
}





