#include "inc/app.hpp"

App::App() {

}

int App::run(int argc, char* argv[]) {
    /* Parse command line arguments
     * Get input and output filenames */
    std::string ifname, ofname;
    if(parseArgs(argc, argv, ifname, ofname) < 0) {
        return -1;

    }

    std::cout << "Input: " << ifname << std::endl;
    std::cout << "Output: " << ofname << std::endl;

    /* Open input file */
    std::ifstream ifile;
    ifile.open(ifname);

    /* Check input file opened correctly */
    if(!ifile.is_open()) {
        std::cerr << "Error opening input file (" << ifname << ")" << std::endl;
        return -1;

    }

    /* Read input file */
    if(readInput(ifile) < 0) {
        ifile.close();
        return -1;

    }

    ifile.close();

    /* Parse input */
    if(parseBlocks() < 0) {
        return -1;

    }

    if(calculateFrequencies() < 0) {
        return -1;
    }

    for(auto& block : parsedBlocks) {
        for(auto& row : block) {
            for(auto& s : row) {
                std::cout << s << " ";

            }

            std::cout << std::endl;

            }
    
        std::cout << std::endl;

    }

    return 0;

}

int App::parseArgs(int argc, char* argv[], std::string& ifname, std::string& ofname) {
    if(argc < 3) {
        std::cerr << "Not enough arguments provided" << std::endl;
        std::cerr << "Usage: " << NP_PROGNAME << " [OPTIONS] infile outfile" << std::endl;
        return -1;

    }

    ofname = argv[argc - 1];
    ifname = argv[argc - 2];

    return 0;

}

int App::readInput(std::ifstream& ifile) {
    std::vector<std::string> block;

    for(std::string line; std::getline(ifile, line); ) {
        if(ifile.bad()) {
            return -1;

        }

        if(line.empty() && !block.empty()) {
            blocks.push_back(block);
            block.clear();

        }
        else {
            block.push_back(line);

        }

    }

    if(!block.empty()) {
        blocks.push_back(block);

    }

    block.clear();

    return 0;

}

int App::parseBlocks() {
    for(auto& block : blocks) {
        if(parseBlock(block) < 0) {
            std::cerr << "Error parsing notes" << std::endl;
            return -1;

        }

    }

    return 0;

}

int App::parseBlock(std::vector<std::string>& block) {
    std::vector<std::vector<Note>> parsedBlock;

    std::string str;
    for(int i = 0; i < block.size(); i++) {
        parsedBlock.push_back(std::vector<Note>());

        std::istringstream line(block[i]);
        while(line >> str) {
            Note n;
            if(parseNote(str, n) < 0) {
                std::cerr << "Error: \"" << str << "\" invalid" << std::endl;
                return -1;

            }

            parsedBlock.back().push_back(n);

        }

    }

    double blockDuration = -1;
    double chanDuration;
    for(auto& chan : parsedBlock) {
        chanDuration = 0;
        for(auto& note : chan) {
            chanDuration += note.duration;

        }

        if(blockDuration == -1) {
            blockDuration = chanDuration;

        }
        else if(chanDuration != blockDuration) {
            std::cerr << "Error: channel duration incorrect" << std::endl;;
            std::cerr << "Expected: " << blockDuration << " Actually: " << chanDuration << std::endl;
            for(auto& note : chan) {
                std::cerr << note << " ";
            }
            std::cerr << std::endl;

            return -1;
        }

    }

    parsedBlocks.push_back(parsedBlock);
    return 0;

}

int App::parseNote(std::string note, Note& ret) {
    std::regex r("(\\d*\\.?\\d+)?([a-gA-G][',]?)(\\d*)");
    std::smatch m;

    if(!std::regex_match(note, m, r)) {
        return -1;
    }

    else {
        ret.duration = std::stod(((std::string)m[1]).empty() ? "1" : (std::string)m[1]);
        ret.note = m[2];
        ret.octave = std::stoi(m[3]);
    }

    return 0;

}

int App::calculateFrequencies() {
    for(auto& block : parsedBlocks) {
        for(auto& chan : block) {
            for(auto& note : chan) {
                int octavesFrom4 = note.octave - 4; // 0
                int stepsFromA; //jk

                try {
                    stepsFromA = stepsFromAMap.at(note.note);
                } catch(std::out_of_range& err) {
                    std::cerr << "Error: invalid note: " << note.note << std::endl;
                    return -1;
                }

                int n = (octavesFrom4 * 12) + stepsFromA;
                double a = (double)n / 12.0;
                double b = pow(2.0, a); 
                note.hertz = 440 * b;
            }
        }
    }

    return 0;
}

std::ostream& operator<<(std::ostream& o, Note n) {
    return o << "{" << n.duration << ":" << n.note << ":" << n.octave << ":" << n.hertz << "Hz" << "}";

}
