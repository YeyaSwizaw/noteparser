#ifndef NP_APP_HPP
#define NP_APP_HPP

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "defines.hpp"

typedef std::string Note;

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
    Note parseNote(std::string note);

}; // class App;

#endif // NP_APP_HPP
