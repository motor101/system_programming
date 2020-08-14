#include<fstream>
#include<algorithm>
#include "tasks_pool.h"

using namespace std;

TasksPool::TasksPool(const char* dataFilePath, const char* delimeters,
	ProcessingEntity processingEntity, int inputBlockDivisionSizeInBytes)
	:delimeters(delimeters),
	processingEntity(processingEntity),
	inputBlockDivisionSizeInBytes(inputBlockDivisionSizeInBytes)
{
	ifstream file(dataFilePath, ifstream::ate);

	fileSize = static_cast<int>(file.tellg());

	data = new char[fileSize];

	file.seekg(0);

	file.read(data, fileSize);

	file.close();

	arrangeTheDataIntoTasks();
}

bool TasksPool::nextTask(pair<int, int>& argument)
{
	bool result = true;
	
	mtx.lock();

	if (firstFreeTaskIndex < tasks.size()) {
		argument = tasks[firstFreeTaskIndex++];
	} else {
		result = false;
	}

	mtx.unlock();

	return result;
}

const char* TasksPool::getData()
{
	return data;
}

int TasksPool::getTasksCount()
{
	mtx.lock();

	int result = tasks.size();

	mtx.unlock();

	return result;
}

// arrange the client input data into ALMOST
// equally sized chnunks. The borders of the chunks must not separate
// processing entities in half.
void TasksPool::arrangeTheDataIntoTasks()
{
	int begin = 0;

	do {
		int end = std::min(begin + inputBlockDivisionSizeInBytes, fileSize);

		if (end < fileSize) {
			while (end < fileSize && strchr(delimeters, data[end]) == nullptr) {
				++end;
			}
		}

		tasks.push_back(std::pair<int, int>(begin, end));
		
		begin = end;

	} while (begin < fileSize);
}

TasksPool::~TasksPool()
{
	delete[] data;
}