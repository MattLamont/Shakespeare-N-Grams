//dotMaker.cpp
//Author: Eric Nguyen
//Created: 4/20/2014
//Description: This program will prompt the user for a two-gram text file
//			   to read from and the name of the output file to store results
//			   into. This program will process the two-gram.txt file from
//			   a word 5       into        "a" -> "word" [weight=5];
//             It will only store the top 500 most used two-grams.

//Last Modified: 4/20/2014

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

template<typename T>
bool pairCompare(const T& lhs, const T& rhs){return (lhs.second > rhs.second); }


int main()
{
    typedef std::pair<std::string, unsigned> ngram;
    std::vector<ngram> Q;
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
        std::string line, word, fmWord;
        getline( inFile, line);

        //split into words and store formatted results into outFile
        std::stringstream ss(line);
        int position=0;
        while(ss >> word)
        {
            ++position;
            //formatting word
            if(position==1)
            {
                    fmWord += '\t';
                    fmWord += '\"';
                    fmWord +=word;
                    fmWord += '\"';
                    fmWord += " -> ";
            }
            //formatting word
            if(position==2)
            {
                    fmWord +=  '\"';
                    fmWord +=  word;
                    fmWord +=  '\"';
                    fmWord +=  "[weight=";
            }
            //insert word into Q
            if(position==3)
            {
                std::stringstream ss1(word);
                unsigned weight1=0;
                ss1 >> weight1;
                if(weight1 > 8)
                {
                    Q.push_back(std::make_pair(fmWord, weight1) );
                }
            }
        }
    }

    //Sort Q from greatest to least
    std::sort(Q.begin(), Q.end(), pairCompare<ngram> );

    //Output first 500 words of Q
    for(unsigned i=0; (i<500) && (i<Q.size()) ; ++i)
    {
        outFile << Q[i].first << Q[i].second << "]\n";
    }

    outFile << "}";

    std::cout << "\n\nDone outputting your file.\n\n";

    return EXIT_SUCCESS;
}
