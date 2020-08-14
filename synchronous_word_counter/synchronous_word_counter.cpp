#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <set>

using namespace std;


int main(int argc, char **argv)
{
	const set<char> delimeters({' ', '\t', '\n', ',', '.', '?', '!', '"' ,'(', ')', ':', '[', ']'});

	const set<char>::iterator end = delimeters.end();

	map<string, int> wordFrequencies;

	const char* fileName = argv[1];

	ifstream file(fileName);

	string word;

	int wordCount = 0;

	while (file) {
		int symbol = file.get();

		if (symbol == -1) {
			break;
		}
		
		if (delimeters.find(static_cast<char>(symbol)) == end) {
			word += symbol;
		}

		else if(word.length() > 0) {
			++wordFrequencies[word];
			word = "";
		}
	}
	map<string, int>::iterator itr;
	for (itr = wordFrequencies.begin(); itr != wordFrequencies.end(); ++itr) {
		cout << itr->first << " " << itr->second << endl;
	}

	return 0;
}
