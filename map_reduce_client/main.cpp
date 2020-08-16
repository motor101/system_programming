#include <iostream>
#include <cstdlib>
#include <cstdint>
#include "ipc_with_server.h"

using namespace std;

int main()
{
    const char delimeters[] = { ' ', '\t', '\n', '\r', ',', '.', '?', '!', '"' ,'(', ')', ':', '[', ']', '\0' };

    uint32_t threadsCount = 0;
    uint32_t inputBlockDivisionSizeInBytes = 10;

    IPCWithServer ipc("../word_counter_dll/x64/Release/word_counter_dll.dll",
        "../input_data/karamazov.txt", "output/map_reduce.txt", WORD_ENTITY, delimeters,
        threadsCount, inputBlockDivisionSizeInBytes);
    
    ipc.sendInputToServer();

    ipc.waitForOutputFromServer();

    return 0;
}
