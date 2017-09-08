#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define CHAR_RANGE_MIN 33
#define CHAR_RANGE_MAX 127
#define REC_SIZE 120
#define MAX_SECS 10.0

/*generates a psuedorandom 120 char record and returns a pointer to it.*/
char *gen_rand_rec() {
	char *p1 = malloc(REC_SIZE);
	char *p2 = p1;
	
	for (int i = 0; i < REC_SIZE; i++, p2++) {
		*p2 = (char)(rand() % (CHAR_RANGE_MAX - CHAR_RANGE_MIN) + CHAR_RANGE_MIN);
	}
	return p1;
}

/*reads a random record from file into a string.
 *rereads the record comparing it to the string in mem. char by char.
 *returns 1 if the string saved in mem. is the same as record read from file.*/
int cmp_rec(FILE *fp) {
	char *cmp_rec = malloc(REC_SIZE);
	char *rec = cmp_rec;
	int rand_offset = (rand() % 10) * (REC_SIZE + 1); 
	int recs_equal = 1;
	int curr_char;
	
	fseek(fp, rand_offset, SEEK_SET); 
	
	while (curr_char = getc(fp) != '\n') {
		*rec = (char)curr_char;
		rec++;
	}
	
	fseek(fp, rand_offset, SEEK_SET);
	
	while  (curr_char = getc(fp) != '\n') {
		if (*cmp_rec != curr_char) {
			recs_equal = 0;
		}
	}
	
	free(cmp_rec);
	
	return recs_equal;
}

/*opens a new text file, and continuously writes records to it, reads records from it, and compares them.
 *operates for approx. two minutes before closing and removing the file.*/
int main(void) {
	FILE *fp = fopen("test.txt", "w+"); 
	char *rec; /*record to write to file*/
	time_t loop_start = time(NULL);
	time_t loop_end;
	int in_time_lim = 1; 
	
	/*for use in getting startup time and writing to file*/
	struct timeval process_start;
	FILE *ps_fp = fopen("process_start.txt", "w+");
	gettimeofday(&process_start, NULL);
	fprintf(ps_fp, "%d\n", process_start.tv_usec);
	fclose(ps_fp);
	
	/*loop runs for approx. two minutes before exiting*/
	srand(time(NULL));
	while (in_time_lim) {
		for (int i = 0; i < 10; i++) {
			rec = gen_rand_rec();
			fwrite(rec, 1, REC_SIZE, fp);
			fputc('\n', fp);
			free(rec);
		}
		
		rewind(fp);
		
		cmp_rec(fp);
		
		loop_end = time(NULL);
		if (difftime(loop_end, loop_start) >= MAX_SECS) {
			in_time_lim = 0;
		}
	}
	
	fclose(fp);
	remove("test.txt");
	
	return 0;
}
