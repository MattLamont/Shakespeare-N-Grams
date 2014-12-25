//dotMaker.cpp
//Author: Eric Nguyen
//Created: 4/20/2014
//Description: This program will prompt the user for a two-gram text file
//			   to read from and the name of the output file to store results
//			   into. This program will process the two-gram.txt file from
//			   a word 5       into        "a" -> "word" [weight=5];

//Last Modified: 4/20/2014

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

int main()
{
    std::string iName, oName;

    //Prompting for and testing input file
    std::cout << "Please enter filename of your input file: ";
    std::cin >> iName;
    std::ifstream inFile( iName.c_str() );
    if(!inFile)
    {
        std::cout << "\nCouldn't open " << iName << ".\nPlease enter valid filename!\n\n";
        inFile.clear();
        exit(EXIT_FAILURE);
    }

    //Prompting for and testing output file
    std::cout << "Please enter filename for your output: ";
    std::cin >> oName;
    std::ofstream outFile;
    outFile.open(oName.c_str() , std::ofstream::out | std::ofstream::trunc);
    if(!outFile)
    {
        std::cout << "\nCouldn't open " << oName << ".\nPlease enter valid filename!\n\n";
        outFile.clear();
        exit(EXIT_FAILURE);
    }

    outFile << "digraph words {\n";

    //Read file until end of file
    while( !inFile.eof() )
    {
        //store line by line
        std::string line, word;
        getline( inFile, line);

        //split into words and store formatted results into outFile
        std::stringstream ss(line);
        int position=0;
        while(ss >> word)
        {
            ++position;
            if(position==1)
            {
                outFile << '\t' << '\"' << word << '\"' << " -> ";
            }
            if(position==2)
            {
                outFile << '\"' << word << '\"' << "[weight=";
            }
            if(position==3)
            {
                outFile << word << "];\n";
            }
        }
    }
    outFile << "}";

    std::cout << "\n\nDone outputting your file.\n\n";

    return EXIT_SUCCESS;
}
