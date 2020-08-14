#include <fstream>
#include <iostream>
#include <cstdint>
#include "ipc_with_client.h"
#include "pipe_read_and_write.h"

using namespace std;

const std::string IPCWithClient::clientInputDirName = "client_input/";
const std::string IPCWithClient::dllFileName = "map_reduce.dll";
const std::string IPCWithClient::dataFileName = "data.txt";
const char* IPCWithClient::mapReduceOutputFilePath = "output/map_reduce.txt";
LPCSTR IPCWithClient::dataPipeName = "\\\\.\\pipe\\map_reduce";
LPCSTR IPCWithClient::debugPipeName = "\\\\.\\pipe\\map_reduce_debug";


string IPCWithClient::getDataFilePath()
{
	return clientInputDirName + dataFileName;
}

string IPCWithClient::getDLLFilePath()
{
	return clientInputDirName + dllFileName;
}

IPCWithClient::IPCWithClient()
{
	createDebugPipe();
	createDataPipe();
}

void IPCWithClient::createDebugPipe()
{

}

void IPCWithClient::createDataPipe()
{
	dataPipeHandle = CreateNamedPipeA(
		dataPipeName,
		PIPE_ACCESS_DUPLEX,
		0,
		1,
		nBufferSize,
		nBufferSize,
		0,
		nullptr
	);

	if (dataPipeHandle == INVALID_HANDLE_VALUE) {
		throw exception("Error creating a data pipe. System error code is " + GetLastError());
	}
}

BOOL IPCWithClient::waitForClientToConnect()
{
	cout << "Waiting for client to connect ..." << endl;
	return ConnectNamedPipe(dataPipeHandle, nullptr);
}

void IPCWithClient::sendOutputToClient()
{
	writeFileToPipe(dataPipeHandle, mapReduceOutputFilePath, 
		dataPipeReadAndWriteBuffer, nBufferSize);
}

void IPCWithClient::readClientInput()
{
	const char* pathToDll = (clientInputDirName + dllFileName).c_str();
	readFileFromPipe(dataPipeHandle, pathToDll, dataPipeReadAndWriteBuffer, nBufferSize);

	const char* pathToDataFile = (clientInputDirName + dataFileName).c_str();
	readFileFromPipe(dataPipeHandle, pathToDataFile, dataPipeReadAndWriteBuffer, nBufferSize);

	uint32_t tmp;
	readNumberFromPipe(dataPipeHandle, &tmp);
	processingEntity = static_cast<ProcessingEntity>(tmp);

	readStringFromPipe(dataPipeHandle, delimeters);

	readNumberFromPipe(dataPipeHandle, &threadsCount);

	// If the client hasn't specified the number of threads, then
	// the server determines the number of threads by querying
	// the system for the number of CPU cores. The number of threads will
	// be the number of CPU cores minus 1 (1 core is used for the main thread
	// the task scheduler).
	if (threadsCount == 0) {
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		threadsCount = sysinfo.dwNumberOfProcessors - 1;
	}
	
	readNumberFromPipe(dataPipeHandle, &inputBlockDivisionSizeInBytes);
	if (inputBlockDivisionSizeInBytes == 0) {
		inputBlockDivisionSizeInBytes = 4096;
	}

	cout << "threadsCount is "<<threadsCount << endl;
	cout << "inputBlockDivisionSizeInBytes is "<< inputBlockDivisionSizeInBytes << endl;
}

void IPCWithClient::closePipe(HANDLE pipeHandle)
{
	FlushFileBuffers(pipeHandle);
	DisconnectNamedPipe(pipeHandle);
	CloseHandle(pipeHandle);
}

int IPCWithClient::getThreadsCount()
{
	return threadsCount;
}

int IPCWithClient::getInputBlockDivisionSizeInBytes()
{
	return inputBlockDivisionSizeInBytes;
}

ProcessingEntity IPCWithClient::getProcessingEntity()
{
	return processingEntity;
}

const char* IPCWithClient::getDelimeters()
{
	return delimeters;
}

IPCWithClient::~IPCWithClient()
{
	closePipe(debugPipeHandle);
	closePipe(dataPipeHandle);
}