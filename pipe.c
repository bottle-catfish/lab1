#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	if (argc==2) {
		printf("Only one argument");
		execlp(argv[1], argv[1], NULL);	
	}
	else if (argc>2) {
		int o_stdin = dup(0);
		int o_stdout = dup(1);
		int piper [2];
		pipe(piper);
		for (int i = 1; i < argc; i++) {
			if (i==1) {
				int pid = fork();
				if (pid==0) {
					close(piper[0]);
					close(piper[1]);
					execlp(argv[i], argv[i], NULL);
				}
				else {
					close(piper[0]);
					close(1);
					dup(piper[1]);
					close(piper[1]);
					int status = 0;
					waitpid(pid, &status, 0);
				}
			}
			else if (i==argc-1) {
				int pid = fork();
				printf("Last Argument\n");
				if (pid==0) {
					close(piper[1]);
					close(0);
					dup(piper[0]);
					close(piper[0]);
					execlp(argv[i], argv[i], NULL);
				}
				else {
					dup2(o_stdout, 1);
					close(piper[0]);
					close(piper[1]);
					int status = 0;
					waitpid(pid, &status, 0);
				}
			}
			else {
				int pid = fork();
				printf("And beyond!\n");
				if (pid==0) {
					close(piper[1]);
					close(0);
					dup(piper[0]);
					close(piper[0]);
					execlp(argv[i], argv[i], NULL);
				}
				else {
					close(piper[0]);
					close(1);
					dup(piper[1]);
					close(piper[1]);
					int status = 0;
					waitpid(pid, &status, 0);
				}

			}
		}
		
		//parent output reset?
		//child reading must be reset every single time
	}





	return 0;
}
