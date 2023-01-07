#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define PORT 8000
#define IP "225.0.0.37"
using namespace std;

void *sender(void *);
void *receiver(void *);

// Variable declaration
int multicast_socket_fd,vector_array[3] = {0,0,0};
struct sockaddr_in multicast_sender_address, multicast_receiver_address;
struct ip_mreq mr;
char * msg_input;

//thread for sending the messages to all the processes
void *sender(void *arg)
{
	int p1_count = 0, p2_count = 0, p3_count = 0, p4_count = 0;
	int process;
	cout << endl;
	cout << "Priority: " << *((int *) arg) << endl;
	cout << "Message Input: " << msg_input << endl;
	while(1)
	{
		for (int i = 0; i < 4; i++) {
			if(i == *((int *) arg)) {
				if(i == 0) {
					p1_count++;
					process = 1;
					cout << "Process: " << process;
					vector_array[i] = p1_count;
				}
				if(i == 1) {
					p2_count++;
					process = 2;
					cout << "Process: " << process;
					vector_array[i] = p2_count;
				}
				if(i == 2) {
					p3_count++;
					process = 3;
					cout << "Process: " << process;
					vector_array[i] = p3_count;
				}
				if(i == 3) {
					p4_count++;
					process = 4;
					cout << "Process: " << process;
					vector_array[i] = p4_count;
				}
			}
		}
		char sender_buffer[256];
		sprintf(sender_buffer,"%d,%d,%d,%d,%d,%s", process, vector_array[0], vector_array[1], vector_array[2], vector_array[3], msg_input);
		cout << "\nMessage [SenderProcessID, Process1 Local Clock, Process2 Local Clock, Process3 Local Clock, Message]: " << sender_buffer << endl;
		sendto(multicast_socket_fd, sender_buffer, sizeof(sender_buffer), 0, (struct sockaddr *) &multicast_sender_address, sizeof(multicast_sender_address));
		sleep(15);
	}
	return 0;
}

//Thread for receiving the messages from all the processes
void *receiver(void *arg)
{
	unsigned int order = *((unsigned int *) arg);
	char receiver_buffer[256],receiver_message[256];
	int processid,arr[4];
	int vector_clock[4] = {0,0,0,0};
	char *message;
	socklen_t receiver_length=sizeof(multicast_receiver_address);
	while(recvfrom(multicast_socket_fd, receiver_buffer, sizeof(receiver_buffer), 0, (struct sockaddr *) &multicast_receiver_address, &receiver_length) >= 0)
	{
		cout << endl;
		cout << "----------Causal ordering----------"<< endl;
		cout << "Message received: " << receiver_buffer << endl;
		message = strtok(receiver_buffer, ",");
		if(message != NULL)
		{
			processid = atoi(message);
			cout << "Process ID: " << processid << endl;
// Citation : This code is referred from stackoverflow (stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c) 
			message = strtok(NULL, ",");
			arr[0] = atoi(message);
			message = strtok(NULL, ",");
			arr[1] = atoi(message);
			message = strtok(NULL, ",");
			arr[2] = atoi(message);
                        message = strtok(NULL, ",");
			arr[3] = atoi(message);
			message = strtok(NULL, " ,");
			strcpy(receiver_message, message);
			cout << endl;
			cout << "Message: " << receiver_message << endl;
			cout << "-------------Vector value--------------- " << arr[0] << "," << arr[1] <<"," << arr[2]<<"," << arr[3] << endl;

		}		
		
// Comparison of every vector clock with individual local clocks of their processes		
		if (order  == 0) {
			if (processid == 1) {	
		
				cout <<"Receiving Message from Process 2: " << endl;
				if (vector_clock[1] > arr[1])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
		
			}
			else if (processid == 2) {
				cout <<"Receiving Message from Process 3: " << endl;
				if (vector_clock[2] > arr[2])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
			
			
			}
			else if (processid == 3) {
			       cout <<"Receiving Message from Process 4: " << endl;
			       if (vector_clock[3] > arr[3])
			       {
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
			
			}
		}
		
		if (order == 1) {
		
			if (processid == 0)
			{
				cout <<"Receiving Message from Process 1: " << endl;
				if (vector_clock[0] > arr[0])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
			}
			else if (processid == 2)
			{
				cout <<"Receiving Message from Process 3: " << endl;
				if (vector_clock[2] > arr[2])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
			}
			else if (processid == 3)
			{
				cout <<"Receiving Message from Process 4: " << endl;
				if (vector_clock[3] > arr[3])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
			}
		
		}
		
		if (order == 2) {
			if (processid == 0)
			{
				cout <<"Receiving Message from Process 1: " << endl;
				if (vector_clock[0] > arr[0])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
			}
			else if (processid == 1)
			{
				cout <<"Receiving Message from Process 2: " << endl;
				if (vector_clock[1] > arr[1])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2] <<"," << vector_clock[3] << endl;
				}
			}
			else if (processid == 3)
			{
				cout <<"Receiving Message from Process 4: " << endl;
				if (vector_clock[3] > arr[3])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2] <<"," << vector_clock[3] << endl;
				}
			}	
		
		}
		
		if (order == 3) {
			if (processid == 0)
			{
				cout <<"Receiving Message from Process 1: " << endl;
				if (vector_clock[0] > arr[0])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2]<<"," << vector_clock[3] << endl;
				}
			}
			else if (processid == 1)
			{
				cout <<"Receiving Message from Process 2: " << endl;
				if (vector_clock[1] > arr[1])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2] <<"," << vector_clock[3] << endl;
				}
			}
			else if (processid == 2)
			{
				cout <<"Receiving Message from Process 3: " << endl;
				if (vector_clock[2] > arr[2])
				{
					cout << "Message has been buffered" << endl;
				}
				else
				{
					vector_clock[0] = vector_clock[0] + arr[0];
					vector_clock[1] = vector_clock[1] + arr[1];
					vector_clock[2] = vector_clock[2] + arr[2];
					vector_clock[3] = vector_clock[3] + arr[3];
					cout << endl;
					cout << "Message Delivered Successfully" << endl;
					cout << "Vector value: " << vector_clock[0] << "," << vector_clock[1] <<"," << vector_clock[2] <<"," << vector_clock[3] << endl;
				}
			}	
 }
		sleep(15);
		bzero(receiver_buffer, 256);
	}
	return 0;
}


int main(int argc, char *argv[])
{
	pthread_t sender_thread, receiver_thread;

	if (argc < 3){
		cout << "Please enter all the input parameters : PROGRAM ORDER MESSAGE" << endl;
		exit(1);
	}

	int opt_val = 1;
	unsigned int rank = atoi(argv[1]);
	msg_input = argv[2];
	
	//Multicast sender socket creation
	multicast_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(multicast_socket_fd < 0)
	{
		cout << "Error in creating socket" << endl;
		exit(1);
	}

	//Assigning multicast sender address
	memset((char*) &multicast_sender_address, 0, sizeof(multicast_sender_address));
	multicast_sender_address.sin_family = AF_INET;
	multicast_sender_address.sin_addr.s_addr = inet_addr(IP);
	multicast_sender_address.sin_port = htons(PORT);

	//Assigning multicast receiver address
	memset((char*) &multicast_receiver_address, 0, sizeof(multicast_receiver_address));
	multicast_receiver_address.sin_family = AF_INET;
	multicast_receiver_address.sin_addr.s_addr =htonl(INADDR_ANY);
	multicast_receiver_address.sin_port = htons(PORT);

	//Allowing multiple sockets to use the same addresses
	if(setsockopt(multicast_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) < 0)
	{
		cout << "Error in using the reusable addresses" << endl;
		exit(1);
	}

	//Binding to destination address
	if (bind(multicast_socket_fd, (struct sockaddr *) &multicast_receiver_address, sizeof(multicast_receiver_address)) < 0) {
		cout << "Error in binding the socket" << endl;
		exit(1);
	}
	
	// Citation : Referred this link for creating multicast socket: http://www.cs.unc.edu/~jeffay/dirt/FAQ/comp249-001-F99/mcast-socket.html
	//use setsockopt() to request the kernel to join the multicast group
	mr.imr_multiaddr.s_addr = inet_addr(IP);
	mr.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(multicast_socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mr, sizeof(mr)) < 0)
	{
		cout << "Error in using the reusable addresses" << endl;
		exit(1);
	}

	//Create threads for sender and reciever
	if(pthread_create(&sender_thread,NULL,sender,&rank)<0)
	{
	  cout << "Error in creating a thread for sender" << endl;
	  exit(1);
	}
	if(pthread_create(&receiver_thread,NULL,receiver,&rank)<0)
	{
	  cout << "Error in creating a thread for receiver" << endl;
	  exit(1);
	}
	pthread_join(sender_thread,NULL);
	pthread_join(receiver_thread,NULL);
	cout.flush();
	return 0;
}

