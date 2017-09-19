#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

/*input: reads a file
 *in child: launches the process
 *in parent: waits for the child to finish, reads
 *process start up time from file, finds difference in times.
 *output: writes to a file.*/
int main(void) {
	struct timeval process_call;
	int process_start = 0;
	pid_t pid;
	FILE *read_start, *write_result; 
	int time_diff = 0;
	
	pid = fork();
	gettimeofday(&process_call, NULL);
	if (pid > 0) {
		waitpid(pid, NULL, 0);
		
		read_start = fopen("timer_process_start.txt", "r");
		fscanf(read_start, "%d", &process_start);
		time_diff = process_start - process_call.tv_usec;
		fclose(read_start);
		remove("timer_process_start.txt");
		
		write_result = fopen("timer_results.txt", "a");
		fprintf(write_result, "%d\n", time_diff);
		fclose(write_result);
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
