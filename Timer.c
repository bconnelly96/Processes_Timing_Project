#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	struct timeval process_call;
	int process_start = 0;
	pid_t pid;
	FILE *read_start;
	int time_diff = 0;
	
	pid = fork();
	gettimeofday(&process_call, NULL);
	if (pid > 0) {
		waitpid(pid, NULL, 0);
		
		read_start = fopen("process_start.txt", "r");
		fscanf(read_start, "%d", &process_start);
		time_diff = process_start - process_call.tv_usec;
		printf("\nThe process launched in %d microseconds\n", time_diff);
		
		fclose(read_start);
	}
	else if (pid == 0) {
		if (execlp("./application", "application", NULL) == -1) {
			printf("Process Execution Error has Occured\n");
		}
	}
	else {
		printf("Process Startup Error has Occured\n");
	}
	
	return 0;
}











