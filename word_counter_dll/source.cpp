#include<map>
#include<string>

using namespace std;

extern "C" __declspec(dllexport) void map(const string & inputName, const string & inputValue,
	string & outputName, string & outputValue)
{
	outputName = inputValue;
	outputValue = to_string(5);
}

extern "C" __declspec(dllexport) int test(int a) {
	return a;
}

//extern "C" __declspec(dllexport) pair<string, string> reduce(const string& name)
//{
//	
//}