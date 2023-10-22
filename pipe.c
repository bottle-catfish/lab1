#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if (argc<2) {
		exit(EINVAL);
	}
	if (argc==2) {
		if(execlp(argv[1], argv[1], NULL)==-1) {
			exit(errno);
		}	
	}
	else if (argc>2) {
		for (int i = 1; i < argc; i++) {
			int piper[2];
			int check = pipe(piper);
			if (check==-1) {
				exit(errno);
			}
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
			else if (pid > 0) {
				int status = 0;
				waitpid(pid, &status, 0);
				if (WEXITSTATUS(status)!=0) {
					exit(WEXITSTATUS(status));
				}
				// check the status here to do error handling chief
				close(piper[1]);
				close(0);
				dup(piper[0]);
				close(piper[0]);
			}
			else {
				exit(errno);
			}
		}
	}
	return 0;
}
