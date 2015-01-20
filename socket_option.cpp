#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <string>
#include <iostream>
#include <cstdlib> //for atoi
#include <cstring> //for memset

#include "common.hpp"

void die_with_error(const std::string &msg);
void handle_client_connetion(int sock_client);
void print_socket_options(int sock);
int main(int argc, char *argv[])
{	
	int sock_server;
	if((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			die_with_error("socket() failed");
	print_socket_options(sock_server);	

	return 0;
}

void die_with_error(const std::string &msg)
{
	std::cout<<msg<<std::endl;
	exit(1);
}

void print_socket_options(int sock)
{
	//int getsockopt(int socket, int level, int optName, void *optVal, unsigned int *optLen)
	int buffer_size = 0;
	unsigned int option_len = sizeof(buffer_size);
	if(getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&buffer_size, &option_len) < 0)
		die_with_error("getsockopt() failed");
	std::cout<<"Socket default receive buffer size: "<<buffer_size<<std::endl;
	
	buffer_size = 20 * 1024;
	if(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&buffer_size, sizeof(buffer_size)) < 0)
		die_with_error("setsockopt() failed");
	display(buffer_size);
	buffer_size = 0;	
	option_len = sizeof(buffer_size);
	if(getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&buffer_size, &option_len) < 0)
			die_with_error("getsockopt() failed");
	//the buffer_size will be doubled (determined by kernel algorithm)
	std::cout<<"set socket default receive buffer size to: "<<buffer_size<<std::endl; //40960
	
	
}

