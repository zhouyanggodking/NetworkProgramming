#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
void die_with_error(const std::string &msg)
{
	std::cout<<msg<<std::endl;
	exit(1);
}
int main(int argc, char** argv)
{
	//argument checks
	if(4 != argc)
	{
		std::cout<<"Usage: "<<argv[0]
			<<"<Server IP> <Port> <Message>"<<std::endl;
		exit(1);
	}
	//construct socket
	int sock_client;
	if((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
		die_with_error("socket() failed");
	//construct remote server address structure
	struct sockaddr_in addr_server;
	std::memset(&addr_server, 0, sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(atoi(argv[2]));
	addr_server.sin_addr.s_addr = inet_addr(argv[1]);
	//connect
	if(connect(sock_client, (struct sockaddr*)&addr_server, sizeof(addr_server)) < 0)
		die_with_error("connect() failed");

	//send
	const uint32_t BUF_LEN = 20;
	char buffer[BUF_LEN];
	int total_bytes=0, recv_bytes = 0;
	int msg_len = sizeof(argv[3]);
	if(send(sock_client, buffer, BUF_LEN -1, 0) != msg_len)
		die_with_error("send() failed");

	//receive
	while(total_bytes < msg_len)
	{
		if((recv_bytes = recv(sock_client, buffer, BUF_LEN -1, 0)) <0)
			die_with_error("recv() failed");
		total_bytes += recv_bytes;
		buffer[recv_bytes] = '\0';
		std::cout<<buffer<<std::endl;
	}

	close(sock_client);

	return 0;
}
