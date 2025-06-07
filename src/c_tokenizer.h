#ifndef C_TOKENIZER
#define C_TOKENIZER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DARR_SIZE 1024
#define DB_SIZE 128

typedef enum TOKEN_E{
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_EOF,
    TOKEN_UNKNOWN
}TOKEN_E;

typedef struct token_s{
	enum TOKEN_E type;
	char *value;
}token_s;

typedef struct token_llarr_s{
	struct token_llarr_s *previous;
	struct token_llarr_s *next;
	struct token_s arr[DARR_SIZE];
}token_llarr_s;

static inline int 
is_keyword(const char *str);

static inline token_s 
create_token(enum TOKEN_E type, const char *value);

static inline void
print_identifier(token_s *token);

extern token_llarr_s 
*tokenize_c(FILE *file);

#endif /* C_TOKENIZER */