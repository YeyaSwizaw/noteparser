#include "inc/app.hpp"

App::App() {

} // App::App();

int App::run(int argc, char* argv[]) {
    /* Parse command line arguments
     * Get input and output filenames */
    std::string ifname, ofname;
    if(parseArgs(argc, argv, ifname, ofname) < 0) {
        return -1;

    } // if(parseArgs(argc, argv, ifname, ofname) < 0);

    std::cout << "Input: " << ifname << std::endl;
    std::cout << "Output: " << ofname << std::endl;

    /* Open input file */
    std::ifstream ifile;
    ifile.open(ifname);

    /* Check input file opened correctly */
    if(!ifile.is_open()) {
        std::cerr << "Error opening input file (" << ifname << ")" << std::endl;
        return -1;

    } // if(!ifile.is_open || !ifile.good());

    /* Read input file */
    if(readInput(ifile) < 0) {
        ifile.close();
        return -1;

    } // if(readInput(ifile) < 0);

    ifile.close();

    if(parseBlocks() < 0) {
        return -1;

    } // if(parseBlocks() < 0);

    for(auto& block : parsedBlocks) {
        for(auto& row : block) {
            for(auto& s : row) {
                std::cout << s << "; ";

            } // for(auto& s : row);

            std::cout << std::endl;

            } // for(auto& row : parsedBlocks[0]);
    
        std::cout << std::endl;

    } // for(auto& block : parsedBlocks);

    return 0;

} // int App::run(int argc, char* argv[]);

int App::parseArgs(int argc, char* argv[], std::string& ifname, std::string& ofname) {
    if(argc < 3) {
        std::cerr << "Not enough arguments provided" << std::endl;
        std::cerr << "Usage: " << NP_PROGNAME << " [OPTIONS] infile outfile" << std::endl;
        return -1;

    } // if(argc < 3);

    ofname = argv[argc - 1];
    ifname = argv[argc - 2];

    return 0;

} // int App::parseArgs(int argc, char* argv[], std::string& ifname, std::string& ofname);

int App::readInput(std::ifstream& ifile) {
    std::vector<std::string> block;

    for(std::string line; std::getline(ifile, line); ) {
        if(ifile.bad()) {
            return -1;

        } // if(ifile.bad());

        if(line.empty() && !block.empty()) {
            blocks.push_back(block);
            block.clear();

        } // if(line.empty() && !block.empty());
        else {
            block.push_back(line);

        } // else;

    } // for(std::string line; std::getline(file, line); );

    if(!block.empty()) {
        blocks.push_back(block);

    } // if(!block.empty());

    block.clear();

    return 0;

} // int App::readInput(std::ifstream& ifile);

int App::parseBlocks() {
    for(auto& block : blocks) {
        if(parseBlock(block) < 0) {
            std::cerr << "Error parsing notes" << std::endl;
            return -1;

        } // if(parseBlock(block) < 0);

    } // for(auto& block : blocks);

    return 0;

} // int App::parseBlocks();

int App::parseBlock(std::vector<std::string>& block) {
    std::vector<std::vector<Note>> parsedBlock;

    std::string str;
    for(int i = 0; i < block.size(); i++) {
        parsedBlock.push_back(std::vector<std::string>());

        std::istringstream line(block[i]);
        while(line >> str) {
            parsedBlock.back().push_back(parseNote(str));

        } // while(line >> str);

    } // for(int i = 0; i < block.size(); i++);

    parsedBlocks.push_back(parsedBlock);
    return 0;

} // int App::parseBlock(std::vector<std::string> block);

Note App::parseNote(std::string note) {
    std::regex r("(\\d*\\.?\\d+)([a-gA-G][',]?)(\\d*)");
    return note + (std::regex_match(note, r) ? "YAY" : "OHH");

} // Note App::parseNote(std::string note);
