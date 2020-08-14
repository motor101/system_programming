#include<windows.h>
#include<cstdint>
#include "processing_entity.h"

#ifndef IPC_WITH_SERVER
#define IPC_WITH_SERVER

class IPCWithServer
{
public:
	IPCWithServer(const char *pathToDll, const char *pathToDataFile,
		const char* pathToOutputFile, ProcessingEntity processingEntity,
		const char* delimeters, uint32_t threadsCount,
		uint32_t inputBlockDivisionSizeInBytes);

	void sendInputToServer();

	void waitForOutputFromServer();

	~IPCWithServer();

private:
	uint32_t threadsCount;
	uint32_t inputBlockDivisionSizeInBytes;

	static LPCSTR pipeName;
	HANDLE pHandle;

	const char *pathToDll;
	const char *pathToDataFile;
	const char *pathToOutputFile;
	const char *delimeters;
	const ProcessingEntity processingEntity;

	char readAndWriteBuffer[512];
	const DWORD nBufferSize = sizeof(readAndWriteBuffer);
};

#endif // IPC_WITH_SERVER
