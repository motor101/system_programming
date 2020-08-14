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

	~WorkersPool();

private:
	const char* dllPath;
	int threadsCount;
	TasksPool& tasksPool;
	const char* delimiters;
	const char* data;
	map_func_t mapFunction;
};



#endif // WORKERS_POOL_H
