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
		int pipefd[2];
		pipe(pipefd);
		int saved_stdout = dup(1);
		//read will be pipe[0] | write will be pipe[1]
		for (int i=1; i < argc; i++) {
			int code = fork();
			//beginning case
			if (i==1) {
				if (code==0) {
					execlp(argv[i], argv[i], NULL);
				}
				else {
					close(pipefd[0]);
					close(1);
					dup(pipefd[1]);
					close(pipefd[1]);
				}
			}
			else if (i==argc-1) { //end case
				if (code==0) {
					close(pipefd[1]);
					close(0);
					dup(pipefd[0]);
					close(pipefd[0]);
					execlp(argv[i], argv[i], NULL);
				}
				else {
					close(pipefd[0]);
					close(1);
					dup(saved_stdout);
					close(pipefd[1]);
				}

			}
			else {//general case
				if (code==0) { //child
					close(pipefd[1]);
					close(0);
					dup(pipefd[0]);
					close(pipefd[0]);
					execlp(argv[i], argv[i], NULL);
				}
				else { //parent
					close(pipefd[0]);
					close(1);
					dup(pipefd[1]);
					close(pipefd[1]);
				}
			}

		}	

	}
	return 0;
}
