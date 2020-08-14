#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ipc_with_client.h"

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

    return 0;
}