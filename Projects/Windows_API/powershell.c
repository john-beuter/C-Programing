#include <windows.h>
#include <stdio.h>

int main() {
    // Command to execute PowerShell
    LPSTR command = "powershell -Command Get-Process | ?{$_.Name -eq ''} | Select-Object -ExpandProperty Id";
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
    printf("Notepad process ID: %s\n", buffer);

    return 0;
}
