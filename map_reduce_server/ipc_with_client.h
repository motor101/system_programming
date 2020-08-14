#include <windows.h>
#include <string>
#include <vector>
#include "processing_entity.h"


#ifndef IPC_WITH_CLIENT_H
#define IPC_WITH_CLIENT_H

class IPCWithClient
{
public:
	static const std::string clientInputDirName;
	static const std::string dllFileName;
	static const std::string dataFileName;
	static const char* mapReduceOutputFilePath;

	static std::string getDataFilePath();

	static std::string getDLLFilePath();

	IPCWithClient();

	BOOL waitForClientToConnect();

	void readClientInput();

	void sendOutputToClient();

	int getThreadsCount();

	int getInputBlockDivisionSizeInBytes();

	ProcessingEntity getProcessingEntity();

	const char* getDelimeters();

	~IPCWithClient();

private:
	static void closePipe(HANDLE pipeHandle);

	void createDataPipe();

	void createDebugPipe();
	
	static LPCSTR dataPipeName;
	static LPCSTR debugPipeName;

	uint32_t threadsCount;
	uint32_t inputBlockDivisionSizeInBytes;

	ProcessingEntity processingEntity;

	HANDLE dataPipeHandle;
	HANDLE debugPipeHandle;

	char dataPipeReadAndWriteBuffer[512];
	char debugPipeReadAndWriteBuffer[512];
	
	const DWORD nBufferSize = sizeof(dataPipeReadAndWriteBuffer);

	char delimeters[100] = {};
};

#endif //IPC_WITH_CLIENT_H
