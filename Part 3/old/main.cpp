//Author: Matthew Lamont
//File: main.cpp
//Last Modified: 4/21/2014
//Description: Driver file of part3 in Algorithms PS#3.
//             Reads from a text file (argv[1]) and calculates most common n-grams.
//             Stores all 2-grams into a file called two-grams.txt



#include "hw3functions.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <map>

int main(int argc , char* argv[])
{
    std::vector<std::string> book;
    parseData( argv[1] , book , true );

    //finds all the 1-grams
    std::cout << "\n\n\nN-Gram Analysis\n\n";
    std::map<std::string , unsigned int> gram_map = calculateN_Grams( book , 1 );
    std::vector< gram_pair > topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 1 );

    //finds all the 2-grams
    gram_map = calculateN_Grams( book , 2 );
    exportTwoGrams( gram_map );
    topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 2 );

    //Why is 2-grams getting exported twice? Remove below
    //exportTwoGrams( gram_map );

    //finds all the 3-grams
    gram_map = calculateN_Grams( book , 3 );
    topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 3 );

    //finds all the 4-grams
    gram_map = calculateN_Grams( book , 4 );
    topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 4 );

    return 0;
}
