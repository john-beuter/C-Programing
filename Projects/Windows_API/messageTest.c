#include <windows.h> //Houses windows api functions we can use
#include <stdio.h> //Allows me to print
//#include <sys/wait.h> Windows does not have wait

//Reference Video: https://www.youtube.com/watch?v=aNEqC-U5tHM

int main(void)
{
    // int MessageBox(
    // [in, optional] HWND    hWnd,
    // [in, optional] LPCTSTR lpText,
    // [in, optional] LPCTSTR lpCaption,
    // [in]           UINT    uType
    // );

    //This is a lot like the visual basic script I wrote
    MessageBoxW(
        NULL,
        L"This process will take: 3 HOURS", //Message displayed
        L"Warning", //Message header
        MB_YESNO | MB_ICONWARNING //Specifies that we want a yes, no, cancel
        //Outputs with an exclamation
    );

    //
    // BOOL CreateProcessA(
    // [in, optional]      LPCSTR                lpApplicationName, In this case we are running notepad
    // [in, out, optional] LPSTR                 lpCommandLine,     Not needed here
    // [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,   Not needed here
    // [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,    Not needed here
    // [in]                BOOL                  bInheritHandles,       Not needed
    // [in]                DWORD                 dwCreationFlags,       What level do we want this process to run as?
    // [in, optional]      LPVOID                lpEnvironment,         Not needed
    // [in, optional]      LPCSTR                lpCurrentDirectory,    Not needed
    // [in]                LPSTARTUPINFOA        lpStartupInfo,         Needs a struct to this
    // [out]               LPPROCESS_INFORMATION lpProcessInformation   
    // );

    STARTUPINFOW si = { 0 }; //Specifies unicode, gives the data a uniform formate so it can be understood by other computers
    PROCESS_INFORMATION info = { 0 }; //Had some issues with the formating here. In Windows API there is some different variable types that all looks similar
    //LPPROCESS is different than PROCESS 
    if(MK_SHIFT) //When the left click button is pressed spawn a process. 
    {
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
            printf("(+) process running! pid: %ld", info.dwProcessId);
    }
    else if(WM_LBUTTONDOWN)
    {
         return 0;
    }

    // HANDLE OpenProcess(
    //     [in] DWORD dwDesiredAccess,
    //     [in] BOOL  bInheritHandle,
    //     [in] DWORD dwProcessId
    //     );

    OpenProcess(
        PROCESS_ALL_ACCESS, //Default
        FALSE, 
        //PID
    );


    return EXIT_SUCCESS; 
}
