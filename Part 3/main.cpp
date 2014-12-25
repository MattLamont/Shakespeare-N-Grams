
#include "hw3functions.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <map>

int main(int argc , char* argv[])
{
    std::vector<std::string> book;
    parseData( argv[1] , book );

    //finds all the 1-grams
    std::cout << "\n\n\nN-Gram Analysis\n";
    std::map<std::string , unsigned int> gram_map = calculateN_Grams( book , 1 , false );
    std::vector< gram_pair > topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 1 );

    //finds all the 2-grams
    gram_map = calculateN_Grams( book , 2 , false );
    exportTwoGrams( gram_map );
    topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 2 );

    //finds all the 3-grams
    gram_map = calculateN_Grams( book , 3 , false );
    topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 3 );

    //finds all the 4-grams
    gram_map = calculateN_Grams( book , 4 , false );
    topGrams = extractPopularGrams( gram_map );
    displayPopularGrams( gram_map , topGrams , 4 );

    return 0;
}
