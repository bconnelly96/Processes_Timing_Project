#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHAR_RANGE_MIN 33
#define CHAR_RANGE_MAX 127
#define REC_SIZE 120
#define MAX_SECS 120.0

/*generates a psuedorandom 120 char record and returns a pointer to it*/
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
	int rand_step = (rand() % 10) * REC_SIZE; /*gives the num. of characters that come before a certain line in the file.*/
	int char_pos; /*the char position in the file.*/
	int count = 0; /*count of chars traversed over.*/
	char *cmp_rec = malloc(REC_SIZE); /*120 char string saved to mem.*/		
	char *rec = cmp_rec; 
	int recs_equal = 1; /*truth value for two comparison of records.*/
	
	/*get to the line of the text file of the record to be read from ignoring newlines.*/
	while (count <= rand_step) {
		char_pos = getc(fp);
		if (!(char_pos == '\n')) {
			count++;
		}
	}
	
	/*read record into string.*/
	while(char_pos = getc(fp) != '\n') {
		*rec = (char)char_pos;
		rec++;
	}
	
	rewind(fp);
	count = 0;
	
	/*get back to line of record to be read from.*/
	while (count <= rand_step) {
		char_pos = getc(fp);
		if(!(char_pos == '\n')) 
			count++;
	}
	
	/*compare string in mem. and record being read from file.*/
	while (char_pos = getc(fp) != '\n' && recs_equal) {
		if (!(*cmp_rec == char_pos)) {
			recs_equal = 0;
		}
	}
	
	free(cmp_rec);
	
	return recs_equal;
}

int main(void) {
	FILE *fp = fopen("test.txt", "w+"); 
	char *rec; /*record to write to file*/
	time_t loop_start = time(NULL);
	time_t loop_end;
	int test;
	int in_time_lim = 1; 
	
	/*loop runs for two minutes before exiting*/
	while (in_time_lim) {
		for (int i = 0; i < 10; i++) {
			rec = gen_rand_rec();
			fwrite(rec, 1, REC_SIZE, fp);
			fputc('\n', fp);
			free(rec);
		}
		
		rewind(fp);
		
		int test = cmp_rec(fp);
		
		loop_end = time(NULL);
		if (difftime(loop_end, loop_start) >= MAX_SECS) {
			in_time_lim = 0;
		}
	}
	
	fclose(fp);
	remove("test.txt");
	
	return 0;
}
