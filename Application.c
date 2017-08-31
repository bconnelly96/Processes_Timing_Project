#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_RANGE_MIN 33
#define CHAR_RANGE_MAX 127
#define REC_SIZE 120

char *gen_rand_rec(void) {
	char *p1 = malloc(sizeof(char) * REC_SIZE);
	char *p2 = p1;
	
	for (int i = 0; i < REC_SIZE; i++) {
		*p2 = (char)(rand() % (CHAR_RANGE_MAX - CHAR_RANGE_MIN) + CHAR_RANGE_MIN);
		p2++;
	}
	return p1;
}

int main(void) {
	FILE *fp = fopen("test.txt", "w+");
	
	char *rec = NULL;
	for (int i = 0; i < 10; i++) {
		rec = gen_rand_rec();
		fwrite(rec, sizeof(char), REC_SIZE, fp);
		fputc('\n', fp);
		free(rec);
	}
	
	rewind(fp);
	
	char *test = malloc(sizeof(char) * REC_SIZE);
	
	fgets(test, REC_SIZE + 1, fp);
	
	puts(test);
	
	return 0;
}
