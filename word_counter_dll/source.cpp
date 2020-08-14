#include<map>
#include<string>

using namespace std;

extern "C" __declspec(dllexport) void map(const string & inputName, const string & inputValue,
	string & outputName, string & outputValue)
{
	outputName = inputValue;
	outputValue = to_string(1);
}

//extern "C" __declspec(dllexport) pair<string, string> reduce(const string& name)
//{
//	
//}
