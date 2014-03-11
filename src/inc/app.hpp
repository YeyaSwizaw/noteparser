#ifndef NP_APP_HPP
#define NP_APP_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "defines.hpp"

class App {
public:
    App();
    int run(int argc, char* argv[]);

private:
    std::vector<std::vector<std::string>> blocks;
    std::vector<std::vector<std::vector<std::string>>> parsedBlocks;

    int parseArgs(int argc, char* argv[], std::string& ifname, std::string& ofname);
    int readInput(std::ifstream& ifile);
    int parseBlocks();
    int parseBlock(std::vector<std::string>& block);

}; // class App;

#endif // NP_APP_HPP