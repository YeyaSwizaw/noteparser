#ifndef NP_APP_HPP
#define NP_APP_HPP

#include <cmath>
#include <map>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "defines.hpp"

struct Note {
    double duration;
    std::string note;
    int octave;

    double hertz;
    int milliseconds;

}; // struct Note;

const std::map<std::string, int> stepsFromAMap = {
    { "a", 0 },
    { "a'", 1 }, { "b,", 1 },
    { "b", 2 },

    { "c", -9 },
    { "c'", -8 }, { "d,", -8 },
    { "d", -7 }, 
    { "d'", -6 }, { "e,", -6 },
    { "e", -5 },
    { "f", -4 },
    { "f'", -3 }, { "g,", -3 },
    { "g", -2 },
    { "g'", -1 }, { "a,", -1 }
};

class App {
public:
    App();
    int run(int argc, char* argv[]);

private:
    std::vector<std::vector<std::string>> blocks;
    std::vector<std::vector<std::vector<Note>>> parsedBlocks;

    int parseArgs(int argc, char* argv[], std::string& ifname, std::string& ofname);
    int readInput(std::ifstream& ifile);
    int parseBlocks();
    int parseBlock(std::vector<std::string>& block);
    int parseNote(std::string note, Note& ret);
    int calculateFrequencies();

}; // class App;

std::ostream& operator<<(std::ostream& o, Note n);

#endif // NP_APP_HPP
