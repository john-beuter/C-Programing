#include <netinet/in.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <unistd.h>

#define PORT 8081


int main(int argc, char const* argv[])
{
    WSADATA wsa;
    if(WSASocket(AF_INET))


}
