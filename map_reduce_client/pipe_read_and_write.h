#include<windows.h>
#include<cstdint>


#ifndef PIPE_READ_AND_WRITE_H
#define PIPE_READ_AND_WRITE_H

BOOL writeFileToPipe(HANDLE pHandle, const char* filePath, char* buffer, const DWORD nBufferSize);
BOOL readFileFromPipe(HANDLE pHandle, const char* filePath, char* buffer, const DWORD nBufferSize);

BOOL writeStringToPipe(HANDLE pHandle, const char* str);
BOOL readStringFromPipe(HANDLE pHandle, char* str);

BOOL writeNumberToPipe(HANDLE pHandle, uint32_t number);
BOOL readNumberFromPipe(HANDLE pHandle, uint32_t* pNumber);

#endif //PIPE_READ_AND_WRITE_H