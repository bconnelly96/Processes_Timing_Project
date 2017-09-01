#include <stdio.h>
#include <stdlib.h>

#define CHAR_RANGE_MIN 33
#define CHAR_RANGE_MAX 127
#define REC_SIZE 120

/*generates a psuedorandom 120 character "record" and returns a pointer to it*/
char *gen_rand_rec() {
	char *p1 = malloc(REC_SIZE);
	char *p2 = p1;
	
	/*fill the string with random chars*/
	for (int i = 0; i < REC_SIZE; i++, p2++) {
		*p2 = (char)(rand() % (CHAR_RANGE_MAX - CHAR_RANGE_MIN) + CHAR_RANGE_MIN);
	}
	return p1;
}

/*reads a random "record" from file into a string and then rereads the record comparing it character by character
 *to the string saved in memory. returns true if the string saved in memory is same as record read from the file*/
int cmp_rec(FILE *fp) {
	int rand_step = (rand() % 10) * REC_SIZE;	/*gives the number of characters that come before a certain line in the file*/
	int char_pos; /*the character position in the file*/
	int count = 0; /*count of characters traversed over*/
	
	/*get to the position of the record to be read from*/
	while (count <= rand_step) {
		char_pos = getc(fp);
		if (!(char_pos == '\n')) {
			count++;
		}
	}
	
	char *cmp_rec = malloc(REC_SIZE); /*string to be compared to record read from file*/
	char *rec = cmp_rec; 
	
	/*read the line from file into string character by character*/
	while(char_pos = getc(fp) != '\n') {
		*rec = (char)char_pos;
		rec++;
	}
	
	/*reset file pointer to begining of file*/
	rewind(fp);
	
	/*reset count of characters traversed over*/
	count = 0;
	
	/*get back to the position of the first char in the record to be read from*/
	while (count <= rand_step) {
		char_pos = getc(fp);
		if(!(char_pos == '\n')) 
			count++;
	}
	
	/*compare string in mem. and record being read from file character by character*/
	int recs_equal = 1;
	while (char_pos = getc(fp) != '\n' && recs_equal) {
		if (!(*cmp_rec == char_pos)) 
			recs_equal = 0;
	}
	
	/*free mem allocated for comparison string*/
	free(cmp_rec);
	
	return recs_equal;
}

int main(void) {
	FILE *fp = fopen("test.txt", "w+");
	
	char *rec;
	
	/*write "records" to file*/	
	for (int i = 0; i < 10; i++) {
		rec = gen_rand_rec();
		fwrite(rec, 1, REC_SIZE, fp);
		fputc('\n', fp);
		free(rec);
	}
	
	rewind(fp);
	
	int test = cmp_rec(fp);
	
	return 0;
}

