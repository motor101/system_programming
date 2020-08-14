#include <windows.h>
#include <iostream>
#include <exception>
#include <thread>
#include <vector>
#include "workers_pool.h"
#include "worker.h"

using namespace std;

WorkersPool::WorkersPool(const char* dllPath, int threadsCount, TasksPool& tasksPool,
	const char* delimiters, const char* data)
	:dllPath(dllPath),
	threadsCount(threadsCount),
	tasksPool(tasksPool),
	delimiters(delimiters),
	data(data)
{
	HMODULE handle = LoadLibraryA(dllPath);
	if (handle == nullptr) {
		throw exception("The DLL could not be loaded");
	}

	cout << "The DLL was successfully loaded" << endl;

	mapFunction = reinterpret_cast<map_func_t>(GetProcAddress(handle, "map"));

	if (mapFunction == nullptr) {
		throw exception("The map function wasn't found in the dll");
	}

	cout << "The map function address was successfully retreived from the DLL" << endl;
}

void WorkersPool::startWorkers()
{
	vector<thread> workerThreads;

	for (int i = 0; i < threadsCount; ++i) {
		workerThreads.push_back(thread(&Worker::start,
			Worker(tasksPool, delimiters, data, mapFunction)));
	}
	for (int i = 0; i < threadsCount; ++i) {
		workerThreads[i].join();
	}
}

WorkersPool::~WorkersPool()
{

}