#include "pipe_read_and_write.h"
#include<fstream>
#include<cstring>

using namespace std;

BOOL writeFileToPipe(HANDLE pHandle, const char* filePath, char* buffer, const DWORD nBufferSize)
{
    ifstream file(filePath, ifstream::ate | ifstream::binary);

    uint32_t fileSizeInBytes = static_cast<uint32_t>(file.tellg());

    BOOL result = writeNumberToPipe(pHandle, fileSizeInBytes);

    if (!result) {
        return result;
    }

    file.seekg(0);

    while (fileSizeInBytes > 0) {
        DWORD bytesToRead = min(fileSizeInBytes, nBufferSize);
        file.read(buffer, bytesToRead);

        DWORD bytesToWrite = bytesToRead;
        DWORD bytesWritten;
        result = WriteFile(pHandle, buffer, bytesToWrite, &bytesWritten, nullptr);

        if (!result || (bytesToWrite != bytesWritten)) {
            return FALSE;
        }

        fileSizeInBytes -= bytesToRead;
    }

    return TRUE;
}

BOOL readFileFromPipe(HANDLE pHandle, const char* filePath, char* buffer, const DWORD nBufferSize)
{
    uint32_t fileSize;
    
    BOOL result = readNumberFromPipe(pHandle, &fileSize);

    if (!result) {
        return result;
    }

    ofstream file(filePath, ofstream::binary);

    while (fileSize > 0) {
        DWORD bytesToRead = min(fileSize, nBufferSize);
        DWORD bytesRead;

        result = ReadFile(pHandle, buffer, bytesToRead, &bytesRead, nullptr);

        if (!result || (bytesToRead != bytesRead)) {
            return FALSE;
        }

        DWORD bytesToWrite = bytesRead;

        file.write(buffer, bytesToWrite);

        fileSize -= bytesRead;
    }
    
    return TRUE;
}

// str must be a null terminated string
BOOL writeStringToPipe(HANDLE pHandle, const char* str)
{
    DWORD bytesToWrite = strlen(str);
    DWORD bytesWritten;

    BOOL result = writeNumberToPipe(pHandle, static_cast<uint32_t>(bytesToWrite));

    if (!result) {
        return result;
    }

    result = WriteFile(pHandle, str, bytesToWrite, &bytesWritten, nullptr);

    return result && (bytesWritten == bytesToWrite);
}

BOOL readStringFromPipe(HANDLE pHandle, char* str)
{
    uint32_t bytesToRead;
    BOOL result = readNumberFromPipe(pHandle, &bytesToRead);

    if (!result) {
        return result;
    }

    DWORD bytesRead;

    result = ReadFile(pHandle, str, static_cast<DWORD>(bytesToRead), &bytesRead, nullptr);

    BOOL returnValue = result && (bytesRead == bytesToRead);

    if (returnValue) {
        str[bytesRead] = '\0';
    }

    return returnValue;
}

BOOL writeNumberToPipe(HANDLE pHandle, uint32_t number)
{
    DWORD bytesWritten;
    const DWORD bytesToWrite = sizeof(number);

    BOOL result = WriteFile(pHandle, &number, bytesToWrite, &bytesWritten, nullptr);

    return result && (bytesWritten == bytesToWrite);
}

BOOL readNumberFromPipe(HANDLE pHandle, uint32_t *pNumber)
{
    DWORD bytesRead;
    const DWORD bytesToRead = sizeof(*pNumber);
    
    BOOL result = ReadFile(pHandle, pNumber, bytesToRead, &bytesRead, nullptr);

    return result && (bytesRead == bytesToRead);
}