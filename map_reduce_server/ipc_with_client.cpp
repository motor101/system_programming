#include <fstream>
#include <iostream>
#include <cstdint>
#include "ipc_with_client.h"
#include "pipe_read_and_write.h"

using namespace std;

const std::string IPCWithClient::clientInputDirName = "client_input/";
const std::string IPCWithClient::dllFileName = "map_reduce.dll";
const std::string IPCWithClient::dataFileName = "data.txt";
LPCSTR IPCWithClient::dataPipeName = "\\\\.\\pipe\\map_reduce";
LPCSTR IPCWithClient::debugPipeName = "\\\\.\\pipe\\map_reduce_debug";

IPCWithClient::IPCWithClient()
{
	cout << nBufferSize << endl;
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

void IPCWithClient::readClientInput()
{
	//uint32_t number;
	//cout << readNumberFromPipe(dataPipeHandle, &number) << endl;
	//cout << "number is " << number << endl;
	//
	//cout << readFileFromPipe(dataPipeHandle, "hello.txt", dataPipeReadAndWriteBuffer, nBufferSize) << endl;
	//
	//char arr[100];
	//cout << readStringFromPipe(dataPipeHandle, arr);
	//cout << "Array is " << arr << endl;

	const char* pathToDll = (clientInputDirName + dllFileName).c_str();
	readFileFromPipe(dataPipeHandle, pathToDll, dataPipeReadAndWriteBuffer, nBufferSize);

	const char* pathToDataFile = (clientInputDirName + dataFileName).c_str();
	readFileFromPipe(dataPipeHandle, pathToDataFile, dataPipeReadAndWriteBuffer, nBufferSize);

	uint32_t tmp;
	readNumberFromPipe(dataPipeHandle, &tmp);
	processingEntity = static_cast<ProcessingEntity>(tmp);

	readStringFromPipe(dataPipeHandle, delimeters);
}

void IPCWithClient::closePipe(HANDLE pipeHandle)
{
	FlushFileBuffers(pipeHandle);
	DisconnectNamedPipe(pipeHandle);
	CloseHandle(pipeHandle);
}

IPCWithClient::~IPCWithClient()
{
	closePipe(debugPipeHandle);
	closePipe(dataPipeHandle);
}