#include <vector>
#include <mutex>
#include <map>
#include "processing_entity.h"

#ifndef TASKS_POOL_H
#define TASKS_POOL_H

typedef void (*map_func_t)(const std::string&, const std::string&, std::string&, std::string&);
typedef void (*reduce_func_t)(const std::multimap<std::string, std::string>::iterator&,
	const std::multimap<std::string, std::string>::iterator&,
	std::string&, std::string&);

class TasksPool
{
public:
	TasksPool(const char* dataFilePath, const char *delimeters,
		ProcessingEntity processingEntity, int inputBlockDivisionSizeInBytes);

	bool nextTask(std::pair<int, int>& argument);

	const char* getData();

	int getTasksCount();

	~TasksPool();

private:

	std::mutex mtx;

	const int inputBlockDivisionSizeInBytes;

	const char* delimeters;

	ProcessingEntity processingEntity;

	char* data;

	int fileSize;

	std::vector<std::pair<int, int>> tasks;

	int firstFreeTaskIndex = 0;

	void arrangeTheDataIntoTasks();
};

#endif //TASKS_POOL_H