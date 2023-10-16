#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	if (argc==2) {
		execlp(argv[1], argv[1], NULL);	
	}	
	else if (argc>2) {
		int pipefd=2;
		pipe(pipefd);
		//read will be pipe[0] | write will be pipe[1]
		

	}
	return 0;
}
