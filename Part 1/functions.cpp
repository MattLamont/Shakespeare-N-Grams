#include "functions.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <set>

namespace functions
{
    void errorcheck(int argc, char* argv)
    {
        if(argc!=2)
        {
            std::cout << "Invalid number of arguments.";
            exit ( EXIT_FAILURE);
        }

        std::ifstream infile( argv );
        if( !infile )
        {
            std::cerr << "Error opening file.";
            exit( EXIT_FAILURE );
        }
    }

    void getdata(int argc, char* argv, std::vector<std::string>& book)
    {
        std::set<std::string> puncuation;
        std::ifstream in( argv );
        errorcheck( argc , argv );
        std::string line;
        std::cout << "Reading file..." << std::endl;
        int linecount = 0;
        int blanklinecount = 0;
        int wordcount = 0;
        while(!in.eof())
        {
            getline(in,line);
            std::string buffer;
            std::stringstream ss(line);

            while(ss>>buffer)
            {
                bool isword = false;
                for(int i = 0; i < buffer.size(); i++ )
                {
                    if(isalpha(buffer[i]))
                    {
                        isword = true;
                        break;
                    }
                }
                if(isword == true)
                    ++wordcount;
                else
                {
                    puncuation.insert(buffer);
                }
            }

            book.push_back(line);
            ++linecount;
            if(line == "")
                ++blanklinecount;
        }
        std::cout << "Read " << linecount << " lines." << std::endl;
        std::cout << "There were " << blanklinecount << " blank lines." << std::endl;
        std::cout << "There were " << wordcount << " words." << std::endl;
        std::cout << "Punctuation: (";
        std::set<std::string>::iterator it = puncuation.begin();
        while(it!=puncuation.end())
        {
            std::cout << '"' << *it << '"' << " ";
            ++it;
        }
        std::cout << ").";
    }
}
