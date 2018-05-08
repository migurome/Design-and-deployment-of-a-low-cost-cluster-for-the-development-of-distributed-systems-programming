#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define MAX_MSG_LENGTH 256		// Maximum length of the message


/** Function that shows an error message */
void showError(const char *errorMsg){
    perror(errorMsg);
    exit(0);
}

int main(int argc, char *argv[]){

    int socketfd;						/** Socket descriptor */
    unsigned int port;					/** Port number */
    struct sockaddr_in server_address;	/** Server address structure */
    char* serverIP;						/** Server IP */
    char message [MAX_MSG_LENGTH];		/** Message sent to the server side */
    int msgLength;						/** Length of the message */


		// Check arguments!
		if (argc < 3){
			fprintf(stderr,"ERROR wrong number of arguments\n");
		   	fprintf(stderr,"Usage:\n$>%s host port\n", argv[0]);
		   	exit(0);
		}

		// Get the port
		port = atoi(argv[2]);

		// Create socket
		socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// Check if the socket has been successfully created
		if (socketfd < 0)
			showError("ERROR while creating the socket");

		// Get the server address
		serverIP = argv[1];

		// Fill server address structure
		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(serverIP);
		server_address.sin_port = htons(port);

		// Connect with server
		if (connect(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
			showError("ERROR while establishing connection");

		// Init and read the message
		printf("Enter a message: ");
		memset(message, 0, MAX_MSG_LENGTH);
		fgets(message, MAX_MSG_LENGTH-1, stdin);

		// Send message to the server side
		msgLength = send(socketfd, message, strlen(message), 0);

		// Check the number of bytes sent
		if (msgLength < 0)
			showError("ERROR while writing to the socket");

		// Init for reading incoming message
		memset(message, 0, MAX_MSG_LENGTH);
		msgLength = recv(socketfd, message, MAX_MSG_LENGTH-1, 0);

		// Check bytes read
		if (msgLength < 0)
			showError("ERROR while reading from the socket");

		// Show the returned message
		printf("%s\n",message);

		// Close socket
		close(socketfd);

	return 0;
}
