#include<windows.h>
#include<cstdint>

#ifndef IPC_WITH_SERVER
#define IPC_WITH_SERVER

enum ProcessingEntity
{
	SYMBOL_ENTITY,
	WORD_ENTITY,
	LINE_ENTITY,
	PROCESSING_ENTITY_COUNT
};

class IPCWithServer
{
public:
	IPCWithServer(const char *pathToDll, const char *pathToDataFile,
		ProcessingEntity processingEntity, const char* delimeters);

	void sendInputToServer();

	~IPCWithServer();

private:
	static LPCSTR pipeName;
	HANDLE pHandle;

	const char *pathToDll;
	const char *pathToDataFile;
	const char *delimeters;
	const ProcessingEntity processingEntity;

	char readAndWriteBuffer[512];
	const DWORD nBufferSize = sizeof(readAndWriteBuffer);
};

#endif // IPC_WITH_SERVER
