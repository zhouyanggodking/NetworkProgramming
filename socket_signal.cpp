#include <signal.h>
#include <unistd.h>
#include <iostream>

#include <string>
#include <cstdlib>

void die_with_error(const std::string &msg);
void interrupt_signal_handler(int signal_type);

int main(int argc, char **argv)
{
//	struct sigaction
//	{
//		void (*signal_handler)(int);
//		sigset_t sa_mask;
//		int sa_flags;
//		void (*sa_restorer)(void);
//	};
	struct sigaction signal_handler;
	signal_handler.sa_handler = interrupt_signal_handler;

	if(sigfillset(&signal_handler.sa_mask) < 0)
		die_with_error("sigfillset() failed");
	signal_handler.sa_flags = 0;

	//int sigaction(int whichSignal, const struct sigaction *newAction, struct sigaction *oldAction)
	//set handler for interrupt signal
	if(sigaction(SIGINT, &signal_handler, 0) < 0)
		die_with_error("sigaction() failed");

	while(1)
	{
		pause();
	}

	return 0;
}

void die_with_error(const std::string &msg)
{
	std::cout<<msg<<std::endl;
	exit(1);
}

void interrupt_signal_handler(int signal_type)
{
	std::cout<<"Interrupt signal received. Exiting program"<<std::endl;
	//demo for mutilple interrupt signla coming in
	sleep(3);// in <unistd.h>
}

//to kill this program, use 'pgrep -l socket_signal' to get the pid of process
//then kill <pid>

