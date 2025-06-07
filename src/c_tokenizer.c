#include "c_tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

char index_char;
char next_char;
char *value;
const char *token_keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while", NULL
};
const char *token_enum_name[] = {
    "TOKEN_IDENTIFIER",
    "TOKEN_KEYWORD",
    "TOKEN_NUMBER",
    "TOKEN_STRING",
    "TOKEN_OPERATOR",
    "TOKEN_SEPARATOR",
    "TOKEN_EOF",
    "TOKEN_UNKNOWN",
    NULL
};

static inline int 
is_keyword(const char *value)
{
    for(int i = 0; token_keywords[i] != 0; i++){
        if(strcmp(value,token_keywords[i]) == 0)
            return 1;}
    return 0;
}

static inline token_s
create_token(enum TOKEN_E type, const char *value)
{
    token_s token;
    token.type = type;
    if(value != NULL){
        token.value = malloc(strlen(value) + 1);
        strcpy(token.value, value);
    }else{
        token.value = NULL;}
    return token;
}

static inline token_s
find_token(FILE *file)
{
    /*
    index_char = fgetc(file);
    next_char = fgetc(file);
    fseek(file, -1, SEEK_CUR);
    printf("%c\n", index_char);
    */

    do{
        if(index_char == EOF) return create_token(TOKEN_EOF, NULL);
        index_char = fgetc(file);
        if(index_char == EOF) return create_token(TOKEN_EOF, NULL);
        if(index_char == ' ' || index_char == '\n' || index_char == '\t')
            continue;
        if(IS_SEPARATOR(index_char)){
            char sep[2] = { index_char, '\0' };
            return create_token(TOKEN_SEPARATOR, sep);
            continue;}
        if(isdigit(index_char)){
            int i = 0;
            value[i++] = index_char;
            while(isdigit(index_char = fgetc(file)))
                value[i++] = index_char;
            value[i] = '\0';
            fseek(file, -1, SEEK_CUR);
            return create_token(TOKEN_NUMBER, value);}
        if(isalpha(index_char) || index_char == '_'){
            int i = 0;
            value[i++] = index_char;
            while(isalpha(index_char = fgetc(file)) || index_char == '_')
                value[i++] = index_char;
            value[i] = '\0';
            fseek(file, -1, SEEK_CUR);
            if(!is_keyword(value))
                return create_token(TOKEN_IDENTIFIER, value);
            return create_token(TOKEN_KEYWORD, value);}
        if(index_char == '"'){
            int i = 0;
            index_char = fgetc(file);
            if(index_char == '"'){
                value[0] = '\0';
                return create_token(TOKEN_STRING, value);}
            value[i++] = index_char;
            while((index_char = fgetc(file)) != '"' && index_char != EOF)
                value[i++] = index_char;
            value[i] = '\0';
            return create_token(TOKEN_STRING, value);}
        if(index_char == '\''){
            index_char = fgetc(file);
            if(index_char == '\''){
                value[0] = '\0';
                return create_token(TOKEN_STRING, value);}
            value[0] = index_char;
            if (index_char) {
            
            }
            value[1] = '\0';
            return create_token(TOKEN_STRING, value);}
        if(IS_OPERATOR(index_char)){
            next_char = fgetc(file);
            fseek(file, -1, SEEK_CUR);
            if(next_char != '/' && next_char != '*'){
                char sep[2] = { index_char, '\0' };
                return create_token(TOKEN_OPERATOR, sep);}
            if((index_char == '/') && (next_char == '/')){
                while(index_char!= '\n' && index_char != EOF)
                    index_char = fgetc(file);
                continue;}
            if((index_char == '/') && (next_char == '*')){
                    fseek(file, 1, SEEK_CUR);
                    index_char = fgetc(file);
                    next_char = fgetc(file);
                while(index_char != EOF){
                    if(index_char == '*' && next_char == '/'){
                        break;}
                    fseek(file, -1, SEEK_CUR);
                    index_char = fgetc(file);
                    next_char = fgetc(file);
                    continue;}
                continue;}}
    }while(1);
    return create_token(TOKEN_EOF, NULL);
}

static inline void
print_identifier(token_s *token)
{
    printf("token type:%s\ntoken value:%s\n", token_enum_name[token->type], token->value);
}

extern token_llarr_s
*tokenize_c(FILE *file)
{
    value = calloc(DB_SIZE, sizeof(char));
    token_llarr_s *tokens = calloc(1, sizeof(token_llarr_s));
    token_llarr_s *tokens_first = tokens;
    tokens->arr[0] = find_token(file);
    print_identifier(&tokens->arr[0]);
    for(int i = 1; tokens->arr[i-1].type != TOKEN_EOF; i++){
        if(i%999 == 0){
            puts("allocating next token array");
            tokens->next = calloc(1, sizeof(token_llarr_s));
            tokens->next->previous = tokens;
            tokens = tokens->next;
            i = 0;}
        tokens->arr[i] = find_token(file);
        print_identifier(&tokens->arr[i]);}
    free(value);
    return tokens_first;
}