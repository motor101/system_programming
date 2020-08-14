#include <string>
#include "tasks_pool.h"

#ifndef WORKERS_POOL_H
#define WORKERS_POOL_H

class WorkersPool
{
public:
	WorkersPool(const char* dllPath, int threadsCount, TasksPool& tasksPool,
		const char* delimiters, const char* data);

	void startWorkers();

	void writeMapReduceOutputToFile(const char* filePath);

	~WorkersPool();

private:
	std::map<std::string, std::string> mapReduceFinalOutput;

	const char* dllPath;
	int threadsCount;
	TasksPool& tasksPool;
	const char* delimiters;
	const char* data;
	map_func_t mapFunction;
	reduce_func_t reduceFunction;
};



#endif // WORKERS_POOL_H
