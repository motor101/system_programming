#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ipc_with_client.h"
#include "tasks_pool.h"

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

    TasksPool tasksPool(IPCWithClient::getDataFilePath().c_str(),
                        clientCommunication.getDelimeters(),
                        clientCommunication.getProcessingEntity(),
                        clientCommunication.getInputBlockDivisionSizeInBytes());
    
    cout << "tasks count is " << tasksPool.getTasksCount() << endl;

    pair<int, int> argument;

    const char* data = tasksPool.getData();

    int j = 0;

    while (tasksPool.nextTask(argument)) {
        
        cout << "task " << j++ << ": ";
        
        for (int i = argument.first; i < argument.second; ++i) {
            cout << data[i];
        }
        
        cout << endl;
    }


    return 0;
}