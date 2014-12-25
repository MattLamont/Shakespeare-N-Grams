#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;

void PrintUsage(char* file_name)
{
	std::cout << "Usage: " << file_name << " input-file n\n";
}

// ProcessArguments returns true if and only if the supplied arguments are valid.
bool ProcessArguments(char** argv, ifstream& input, int& gram_order)
{
	stringstream arg_ss;
	arg_ss << argv[2];
	arg_ss >> gram_order;

	if (arg_ss.fail() || gram_order < 1 || gram_order > 10)
		return false;

	input.open(argv[1]);
	return input.is_open();
}

// Turn the "queue" of words into a single phrase:
string ConstructNGram(const deque<string>& words)
{
	string result = words[0];
	for (size_t i = 1; i < words.size(); ++i) 
	{
		result += " " + words[i];
	}

	return result;
}

// Functor used to sort and display results:
struct PairFn 
{
	// Used for sorting:
	bool operator()(const pair<string, int>& lhs, const pair<string, int>& rhs)
	{
		// Sort first by count in decreasing order:
		if (lhs.second != rhs.second)
		return lhs.second > rhs.second;

		// Otherwise, sort reverse alphabetically:
		return lhs.first > rhs.first;
	}

	// Used for display:
	string operator()(const pair<string, int>& ngram)
	{
		stringstream display_ss;
		display_ss << "\"" << ngram.first << "\" [" << ngram.second << "]";
		return display_ss.str();
	}
};

int main(int argc, char** argv)
{
	if (argc != 3) 
	{
		PrintUsage(argv[0]);
		return -1;
	}

	ifstream input;
	int gram_order;
	if (!ProcessArguments(argv, input, gram_order)) 
	{
		cout << "Bad arguments. ";
		PrintUsage(argv[0]);
		return -1;
	}

	map<string, int> ngram_counts;
	deque<string> previous_words;
	string word;
	for (int i = 0; i < gram_order - 1; ++i)
	{
		input >> word;
		previous_words.push_back(word);
	}

	// Read the n-gram and record them in the map:
	int i = 0;
	while (input >> word) 
	{
		if (isalpha(word[i]))
		{
			previous_words.push_back(word);
			++ngram_counts[ConstructNGram(previous_words)];
			previous_words.pop_front();
		}
	}

	input.close();

	// Use a vector to sort and display the results:
	vector<pair<string, int> > results(ngram_counts.begin(), ngram_counts.end());
	sort(results.begin(), results.end(), PairFn());
	transform(results.begin(), results.end(), ostream_iterator<string>(cout, "\n"), PairFn());

	return 0;
}
