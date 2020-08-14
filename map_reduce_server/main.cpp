#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ipc_with_client.h"
#include "tasks_pool.h"
#include "workers_pool.h"

using namespace std;

int main()
{
    IPCWithClient clientCommunication;

    if (!clientCommunication.waitForClientToConnect() 
        && GetLastError() != ERROR_PIPE_CONNECTED)
    {
        cerr << "Error during connection to the data pipe. System error code is "
            << GetLastError() << '\n';

        return 1;
    }

    cout << "Client connected successfully" << endl;
    
    cout << "Reading client input" << endl;

    clientCommunication.readClientInput();

    cout << "Client input read successfully" << endl;

    cout << "Dividing the input data into tasks" << endl;
    TasksPool tasksPool(IPCWithClient::getDataFilePath().c_str(),
                        clientCommunication.getDelimeters(),
                        clientCommunication.getProcessingEntity(),
                        clientCommunication.getInputBlockDivisionSizeInBytes());
    cout << "Tasks are ready to be given to the workers" << endl;
    
    WorkersPool workerPool(clientCommunication.getDLLFilePath().c_str(),
        clientCommunication.getThreadsCount(), tasksPool,
        clientCommunication.getDelimeters(), tasksPool.getData());

    workerPool.startWorkers();

    cout << "Saving the map reduce output in " << IPCWithClient::mapReduceOutputFilePath << endl;
    
    workerPool.writeMapReduceOutputToFile(IPCWithClient::mapReduceOutputFilePath);

    cout << "Sending the map reduce output to the client " << endl;

    clientCommunication.sendOutputToClient();

    // wait 2 seconds for the client to read the map reduce output
    Sleep(2000);

    return 0;
}