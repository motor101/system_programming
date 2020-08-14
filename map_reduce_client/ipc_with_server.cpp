#include<iostream>
#include<fstream>
#include "ipc_with_server.h"
#include "pipe_read_and_write.h"

using namespace std;

LPCSTR IPCWithServer::pipeName = "\\\\.\\pipe\\map_reduce";

IPCWithServer::IPCWithServer(const char* pathToDll, const char* pathToDataFile,
    ProcessingEntity processingEntity, const char* delimeters)
    :pathToDll(pathToDll),
    pathToDataFile(pathToDataFile),
    processingEntity(processingEntity),
    delimeters(delimeters)
{
    pHandle = CreateFileA(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (pHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Waiting for connection with the server..." << endl;
        
        if (!WaitNamedPipeA(pipeName, 10000))
        {
            throw exception("Error connecting to pipe. System error code is " + GetLastError());
        }

        cout << "Successfull connection with the server" << endl;
    }
}

void IPCWithServer::sendInputToServer()
{
    //cout << writeNumberToPipe(pHandle, 123456) << endl;
    //cout << writeFileToPipe(pHandle, "hello.txt", readAndWriteBuffer, nBufferSize);
    ////writeFileToPipe(pHandle, pathToDll, readAndWriteBuffer, nBufferSize);
    //cout << writeStringToPipe(pHandle, "hello there");

    writeFileToPipe(pHandle, pathToDll, readAndWriteBuffer, nBufferSize);
    writeFileToPipe(pHandle, pathToDataFile, readAndWriteBuffer, nBufferSize);
    writeNumberToPipe(pHandle, static_cast<uint32_t>(processingEntity));
    writeStringToPipe(pHandle, delimeters);
}

IPCWithServer::~IPCWithServer()
{
    CloseHandle(pHandle);
}


