#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	//TO DO : ERROR HANDLING IN GENERAL 
	//TO DO : ERROR HANDLING FOR TOO FEW ARGUMENTS
	if (argc<2) {
		return EINVAL;
	}
	if (argc==2) {
		printf("Only one argument");
		execlp(argv[1], argv[1], NULL);	
	}
	else if (argc>2) {
		for (int i = 1; i < argc; i++) {
			int piper[2];
			pipe(piper);
			int pid = fork();
			if (pid==0) {
				if (i!=argc-1) {
					close(1);
					dup(piper[1]);
					close(piper[1]);
				}
				if (execlp(argv[i], argv[i], NULL)==-1) {
					exit(errno);
				}
				
			}
			else {
				int status = 0;
				waitpid(pid, &status, 0);
				if (!WIFEXITED(status)) {
					return WEXITSTATUS(status);
				}
				// check the status here to do error handling chief
				close(piper[1]);
				close(0);
				dup(piper[0]);
				close(piper[0]);
			}
		}
	}
	return 0;
}
