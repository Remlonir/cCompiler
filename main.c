#include <stdio.h>
#include "src/c_tokenizer.h"

int
main(int argc, char *argv[])
{
	FILE *file = fopen(argv[1], "r");
    if(!file){
       	fprintf(stderr, "Failed to open file: %s\n", argv[1]);
       	return -1;}
	token_llarr_s *tokens = tokenize_c(file);
	//free(tokens);
	return 0;
}