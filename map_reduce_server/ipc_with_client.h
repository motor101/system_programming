#include <windows.h>
#include <string>
#include <vector>

#ifndef IPC_WITH_CLIENT_H
#define IPC_WITH_CLIENT_H

enum ProcessingEntity
{
	SYMBOL_ENTITY,
	WORD_ENTITY,
	LINE_ENTITY,
	PROCESSING_ENTITY_COUNT
};

class IPCWithClient
{
public:
	static const std::string clientInputDirName;
	static const std::string dllFileName;
	static const std::string dataFileName;

	IPCWithClient();

	BOOL waitForClientToConnect();

	void readClientInput();

	~IPCWithClient();

private:
	static void closePipe(HANDLE pipeHandle);

	void createDataPipe();

	void createDebugPipe();
	
	static LPCSTR dataPipeName;
	static LPCSTR debugPipeName;

	ProcessingEntity processingEntity;

	HANDLE dataPipeHandle;
	HANDLE debugPipeHandle;

	char dataPipeReadAndWriteBuffer[512];
	char debugPipeReadAndWriteBuffer[512];
	
	const DWORD nBufferSize = sizeof(dataPipeReadAndWriteBuffer);

	char delimeters[100] = {};
};

#endif //IPC_WITH_CLIENT_H
