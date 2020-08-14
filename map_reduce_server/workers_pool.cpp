#include <windows.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <thread>
#include <vector>
#include <string>
#include <map>
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
	dllHandle = LoadLibraryA(dllPath);
	if (dllHandle == nullptr) {
		throw exception("The DLL could not be loaded");
	}

	cout << "The DLL was successfully loaded" << endl;

	mapFunction = reinterpret_cast<map_func_t>(GetProcAddress(dllHandle, "map"));

	if (mapFunction == nullptr) {
		throw exception("The map function wasn't found in the dll");
	}

	reduceFunction = reinterpret_cast<reduce_func_t>(GetProcAddress(dllHandle, "reduce"));
	
	if (reduceFunction == nullptr) {
		throw exception("The reduce function wasn't found in the dll");
	}

	cout << "The map and reduce function addresses were successfully retreived from the DLL" << endl;

}

void WorkersPool::startWorkers()
{
	cout << "Creating a multimap for each worker" << endl;
	
	vector<multimap<string, string>> workerMultimaps;
	for (int i = 0; i < threadsCount; ++i) {
		workerMultimaps.push_back(multimap<string, string>());
	}

	cout << "Starting each worker thread" << endl;

	vector<thread> workerThreads;
	for (int i = 0; i < threadsCount; ++i) {
		workerThreads.push_back(thread(&Worker::start,
			Worker(tasksPool, delimiters, data, mapFunction, workerMultimaps[i])));
	}

	cout << "Joining all worker threads" << endl;
	for (int i = 0; i < threadsCount; ++i) {
		workerThreads[i].join();
	}

	multimap<string, string> mainMultimap;

	cout << "Inserting every element from the workers' multimaps to the main multimap" << endl;

	// insert every element from the workers' multimaps to the main multimap
	for (int i = 0; i < threadsCount; ++i) {
		for (multimap<string, string>::iterator itr = workerMultimaps[i].begin();
			itr != workerMultimaps[i].end(); ++itr) {
			mainMultimap.insert(*itr);
		}
	}

	for (multimap<string, string>::iterator itr = mainMultimap.begin();
		itr != mainMultimap.end();) {
		multimap<string, string>::iterator itr2 = mainMultimap.upper_bound(itr->first);
		
		string name;
		string value;

		reduceFunction(itr, itr2, name, value);
		mapReduceFinalOutput.insert(pair<string, string>(name, value));
	
		itr = itr2;
	}


}

void WorkersPool::writeMapReduceOutputToFile(const char* filePath)
{
	ofstream file(filePath);
	for (map<string, string>::iterator itr = mapReduceFinalOutput.begin();
		itr != mapReduceFinalOutput.end(); ++itr) {
		file << itr->first << " " << itr->second << endl;
	}
}

WorkersPool::~WorkersPool()
{
	FreeLibrary(dllHandle);
}