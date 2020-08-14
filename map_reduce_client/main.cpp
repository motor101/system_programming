#include <iostream>
#include <cstdlib>
#include <cstdint>
#include "ipc_with_server.h"

using namespace std;

int main()
{
    const char delimeters[] = { ' ', '\t', '\n', ',', '.', '?', '!', '"' ,'(', ')', ':', '[', ']', '\0' };

    IPCWithServer ipc("../word_counter_dll/x64/Release/word_counter_dll.dll",
        "../input_data/karamazov.txt", WORD_ENTITY, delimeters);
    
    ipc.sendInputToServer();

    return 0;
}
