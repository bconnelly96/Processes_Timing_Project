#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

/*input: reads a file
 *function: forks the process twice, executing the second fork in the parent process 
 *of the first fork. 
 *records startup times of the processes in microseconds.
 *output: writes results to a file*/
int main(void) {
	pid_t pid_A, pid_B;	
	struct timeval A_call, B_call;
	int A_start = 0, B_start = 0;
	FILE *read_start, *write_result;
	int time_diff_A = 0, time_diff_B = 0;
	
	gettimeofday(&A_call, NULL);
	pid_A = fork();
	
	if (pid_A == 0) {
		execlp("./application", "application", NULL);
	}	
	else if (pid_A > 0) {
		gettimeofday(&B_call, NULL);
		pid_B = fork();
		
		if (pid_B == 0) {
			execlp("./application", "application", NULL);
		}
		else if (pid_B > 0) {
			waitpid(pid_B, NULL, 0);
			
			read_start = fopen("timer_process_start.txt", "r");
			fscanf(read_start, "%d", &A_start);
			fscanf(read_start, "%d", &B_start);
			fclose(read_start);
			remove("timer_process_start.txt");
			
			time_diff_A = A_start - A_call.tv_usec;
			time_diff_B = B_start - B_call.tv_usec;
			
			write_result = fopen("timer2_results.txt", "a");
			fprintf(write_result, "Process A startup: %d\n", time_diff_A);
			fprintf(write_result, "Process B startup: %d\n", time_diff_B);
			fclose(write_result);
		}
	}
	
	return 0;
}
