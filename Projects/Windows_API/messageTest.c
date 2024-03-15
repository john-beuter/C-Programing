#include <windows.h> //Houses windows api functions we can use
#include <stdio.h> //Allows me to print

//Reference Video: https://www.youtube.com/watch?v=aNEqC-U5tHM

int main(void)
{
    //This is a lot like the visual basic script I wrote
    MessageBoxW(
        NULL,
        L"100 Malicious Files Found", //Message displayed
        L"COMPUTE HAS BEEN INFECTED", //Message header
        MB_YESNOCANCEL | MB_ICONEXCLAMATION //Specifies that we want a yes, no, cancel
        //Outputs with an exclamation
    );

    // BOOL CreateProcessA(
    // [in, optional]      LPCSTR                lpApplicationName,
    // [in, out, optional] LPSTR                 lpCommandLine,
    // [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
    // [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
    // [in]                BOOL                  bInheritHandles,
    // [in]                DWORD                 dwCreationFlags,
    // [in, optional]      LPVOID                lpEnvironment,
    // [in, optional]      LPCSTR                lpCurrentDirectory,
    // [in]                LPSTARTUPINFOA        lpStartupInfo,
    // [out]               LPPROCESS_INFORMATION lpProcessInformation
    // );

    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION info = { 0 };

    if(!CreateProcessW( //Spawns a notepad instance!!
        L"C:\\Windows\\System32\\notepad.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        BELOW_NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &si,
        &info))
    {
        printf("(-) failed to create a process, error %ld", GetLastError()); //Retrieves the calling threads error code
    }

    return EXIT_SUCCESS; 
}
