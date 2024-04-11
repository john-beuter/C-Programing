#include <windows.h> //Houses windows api functions we can use
#include <stdio.h> //Allows me to print
//#include <sys/wait.h> Windows does not have wait

//Reference Video: https://www.youtube.com/watch?v=aNEqC-U5tHM

int main(void)
{
    MessageBoxW(
        NULL,
        L"Please exit and uninstall", //Message displayed
        L"Access to this application denied", //Message header
        MB_ICONERROR //Specifies error icon
    );
}