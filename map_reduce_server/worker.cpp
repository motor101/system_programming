#include "worker.h"
#include <iostream>

using namespace std;

Worker::Worker(TasksPool& tasksPool, const char* delimiters,
	const char* data, map_func_t mapFunction)
	: tasksPool(tasksPool),
	delimiters(delimiters),
	data(data),
	mapFunction(mapFunction)
{}

void Worker::start()
{
	std::pair<int, int> taskScope;

	while (tasksPool.nextTask(taskScope)) {
		
		cout << taskScope.first << " " << taskScope.first << "\n";
	}
}