#include "tasks_pool.h"

#ifndef WORKER_H
#define WORKER_H

class Worker
{
public:
	Worker(TasksPool& tasksPool, const char* delimiters,
		const char* data, map_func_t mapFunction);

	void start();

private:
	TasksPool& tasksPool;
	const char* delimiters;
	const char* data;
	map_func_t mapFunction;
};


#endif //WORKER_H