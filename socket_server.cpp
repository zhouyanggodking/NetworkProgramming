#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <string>
#include <iostream>
#include <cstdlib> //for atoi
#include <cstring> //for memset

void die_with_error(const std::string &msg);
void handle_client_connetion(int sock_client);
int main(int argc, char *argv[])
{
	//parameters check
	if(argc != 2)
	{
		std::cout<<"Usage: "<<argv[0]<<" <port>"<<std::endl;
		exit(1);
	}
	//construct socket
	int sock_server;
	if((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			die_with_error("socket() failed");
	//construct network address of local server
	struct sockaddr_in addr_server;
	std::memset(&addr_server, 0, sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(std::atoi(argv[1]));
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind to port
	if(bind(sock_server, (struct sockaddr*)&addr_server,sizeof(addr_server)) < 0)
		die_with_error("bind() failed");
	//listen
	const int MAX_PENDING_CONNECTIONS = 5;
	if(listen(sock_server, MAX_PENDING_CONNECTIONS) < 0)
		die_with_error("listen() failed");
	//accept
	//receive data
	//processing data
	while(1)
	{
		struct sockaddr_in addr_client;
		std::memset(&addr_client, 0, sizeof(addr_client));
		uint32_t addr_len = sizeof(addr_client);
		int sock_client;
		//accept is a blocking method
		if((sock_client = accept(sock_server, (struct sockaddr*) &addr_client, &addr_len))<0)
			die_with_error("accept() failed");

		handle_client_connetion(sock_client);
	}

	return 0;
}

void die_with_error(const std::string &msg)
{
	std::cout<<msg<<std::endl;
	exit(1);
}

void handle_client_connetion(int sock_client)
{
	const uint32_t BUF_LEN = 32;
	char buffer[BUF_LEN];
	int recv_bytes = 0;
	do
	{
		if((recv_bytes = recv(sock_client, buffer, BUF_LEN -1, 0))<0)
			die_with_error("recv() failed");
		buffer[recv_bytes] = '\0';
		std::cout<<buffer<<std::endl;
		//send back
		if(send(sock_client, buffer, recv_bytes, 0) != recv_bytes)
			die_with_error("send() failed");
	}while(recv_bytes > 0);

	close(sock_client);
}

