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



    return 0;
}





//cout << "tasks count is " << tasksPool.getTasksCount() << endl;
//
//    pair<int, int> argument;
//
//    const char* data = tasksPool.getData();
//
//    int j = 0;
//
//    while (tasksPool.nextTask(argument)) {
//        
//        cout << "task " << j++ << ": ";
//        
//        for (int i = argument.first; i < argument.second; ++i) {
//            cout << data[i];
//        }
//        
//        cout << endl;
//    }
