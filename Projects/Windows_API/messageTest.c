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
    return EXIT_SUCCESS; 
}
