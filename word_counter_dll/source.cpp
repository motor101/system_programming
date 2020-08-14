#include<map>
#include<string>

using namespace std;

extern "C" __declspec(dllexport) void map(const string & inputName, const string & inputValue,
	string & outputName, string & outputValue)
{
	outputName = inputValue;
	outputValue = to_string(1);
}

// All iterators in the interval [begin, end) are pointers to
// pairs<string, string> have the same return value for their
// "first" member variable. That is, itr->first if the inputName and
// the list ot all itr->second is the inputValue.
extern "C" __declspec(dllexport) void reduce(const multimap<string, string>::iterator& begin,
	const multimap<string, string>::iterator& end, string& outputName,
	string& outputValue)
{
	int sum = 0;
	
	for (multimap<string, string>::iterator itr = begin; itr != end; ++itr) {
		sum += stoi(itr->second);
	}

	outputName = begin->first;
	outputValue = to_string(sum);
}
