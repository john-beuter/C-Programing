#include <windows.h>
#include <stdio.h>

const char * k = "[+]";
const char * e = "[-]";
const char * i = "[*]";

int main(int argc, char * argv[])
{
    PVOID rBuffer = NULL;
    DWORD dwPID = NULL, dwTID = NULL;
    HANDLE hProcess = NULL, hThread = NULL; 

    if(argc < 2)
    {
        printf("%s usage: %s <PID> \n", e, argv[0]); 
        return EXIT_FAILURE;
    }

    dwPID = atoi(argv[1]);
    printf("%s trying to get a handle to the process (%ld)\n", i, dwPID);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); //Privs and PID of proocess we want handle to 
    if(hProcess == NULL)
    {
        printf("failed to get process handle, error: 0x%1x", GetLastError()); //GetLastError is from Microsoft. Value is the calling threads last error code.
        return EXIT_FAILURE;
    }

    unsigned char test[] = "\x61\x61\x61\x61\x61\x61";
    size_t testSize = sizeof(test);

    //Create a space for our process to exist in 
    rBuffer = VirtualAllocEx(hProcess, NULL, testSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE); // (MEM_RESERVE | MEM_COMMIT) allows you to do both
    //Making permission rw first then once it is established (made), use virtualprotect() to make it read write execute
    //Use documenation to do this with virtual protect

    if(rBuffer == NULL)
    {
        printf("%s failed to get allocate buffer, error: 0x%1x", k, GetLastError());
        return EXIT_FAILURE;
    }


    //Make our process happen
    WriteProcessMemory(hProcess, rBuffer, test, testSize, NULL);
    printf("%s wrote shellcode to the buffer\n", k);


    return EXIT_SUCCESS;
}