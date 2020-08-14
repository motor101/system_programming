#include "worker.h"
#include <iostream>

using namespace std;

Worker::Worker(TasksPool& tasksPool, const char* delimiters,
	const char* data, map_func_t mapFunction,
	multimap<string, string>& workerMultimap)
	: tasksPool(tasksPool),
	delimiters(delimiters),
	data(data),
	mapFunction(mapFunction),
	workerMultimap(workerMultimap)
{}

void Worker::start()
{
	std::pair<int, int> taskScope;

	while (tasksPool.nextTask(taskScope)) {
		
		// Each word that we want to process lies in the interval [begin, end)
		// We want to find the values of begin and end;
		int begin = taskScope.first;

		while (begin < taskScope.second) {

			// shile begin is a delimiter, move on to the next symbol
			while (strchr(delimiters, data[begin]) != nullptr) {
				++begin;
				if (begin >= taskScope.second) {
					break;
				}
			}
			if (begin >= taskScope.second) {
				break;
			}

			int end = begin + 1;
			// while end is not a delimiter and not outside the task's scope
			while ((strchr(delimiters, data[begin]) == nullptr) 
				&& end < taskScope.second) {
				++end;
			}

			// now the characters with indexes in the range [begin, end) represent
			// a valid input string for the map function


			// inputName will always be empty in my implementation
			// of the map reduce engine
			const string inputName = "";
			string inputValue(data + begin, static_cast<size_t>(end - begin));
			string outputName;
			string outputValue;
			mapFunction(inputName, inputValue, outputName, outputValue);

			workerMultimap.insert(pair<string, string>(outputName, outputValue));

			begin = end;
		}
	}
}