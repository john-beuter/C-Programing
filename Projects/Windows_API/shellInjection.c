#include <windows.h>
#include <stdio.h>

const char * k = "[+]";
const char * e = "[-]";
const char * i = "[*]";

int main(int argc, char * argv[])
{
    void * buffer = NULL;
    DWORD pid = NULL; //32 bit unsigned integer
    DWORD tid = NULL;
    HANDLE process = NULL;
    HANDLE thread = NULL;

    if(argc < 2)
    {
        printf("%s usage: %s <PID> \n", e, argv[0]); 
        return EXIT_FAILURE;
    }

    pid = atoi(argv[1]);
    printf("%s trying to get a handle to the process (%ld)\n", i, pid);

    return EXIT_SUCCESS;
}