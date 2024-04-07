#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main()
{
    int server_socket;
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[500] = "hello\n";
    char received[500];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9001);
    server_addr.sin_addr.s_addr = inet_addr("190.100.60.201");

    bind(server_socket, (struct server_addr*) &server_addr, sizeof(server_addr));
    listen(server_socket, 5); //Listens on the socket and sets the number of connections

	while (1)
	{
		client_socket = accept(server_socket, NULL, NULL); //Looking for connections comming in
		printf("Client connected!\n");

		//on a connection I want to send infromation

		send(client_socket, buffer, sizeof(buffer), 0); //Whwat we want to pass
		ssize_t received_bytes = recv(client_socket, received, sizeof(received), 0);
		if (received_bytes < 0) 
		{
            perror("Error receiving data from client");
            exit(0);
        } 
		else if (received_bytes == 0) 
		{
            printf("Client disconnected\n");
            close(client_socket);
            break;
        } 
		else 
		{
            received[received_bytes] = '\0'; // Null-terminate the received string
            printf("Client has sent the following: %s", received);
        }

		close(client_socket); //Needed to close the socket after the connection
	}	

	close(server_socket);
    return 0 ;

}
