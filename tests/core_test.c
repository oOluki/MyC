#include <stdio.h>
#include "../include/MyC.h"

int main(){

    Mc_token_t* tokens = mc_create_token_buffer(100, 1000); // tokenization test ===============

    unsigned int token_count = 0;

    while(token_count < 240){
        token_count += mc_tokenize(
            &tokens,
            token_count,
            "example 1 of tokenization,// testing mc_tokenizer method//\n"
            "this is the second line of the first example\n"
            "look at me // this part is commented out\n"
            "this isn't =) \n",
            " \n\t",
            ",",
            "//"
        );
    }

    const char* expected[] = {
        "example", "1", "of", "tokenization", ",", "this", "is", "the",
        "second", "line", "of", "the", "first", "example", "look", "at", "me",
        "this", "isn't", "=)"
    } ;

    if(token_count % (sizeof(expected) / sizeof(const char*))){
        fprintf( stderr,
            "[ERROR] Expected To Read n * %zu Tokens, Read %u Instead\n",
            (sizeof(expected) / sizeof(const char*)), token_count
        );
        mc_destroy_token_buffer(tokens);
        return 1;
    }

    const char** string_buffer = (const char**)malloc(token_count * sizeof(char*));
    mc_buffer_token_data(string_buffer, tokens);

    for(unsigned int i = 0; i < token_count; i += 1){
        if(0 == mc_compare_str(string_buffer[i], expected[i % (sizeof(expected) / sizeof(const char*))], 0)){
            fprintf( stderr,
                "[ERROR] Token Number %u '%s' Differs From Expected '%s'\n",
                i, string_buffer[i], expected[i % (sizeof(expected) / sizeof(const char*))]
            );
            mc_destroy_token_buffer(tokens);
            free(string_buffer);
            return 1;
        }
    }

    mc_destroy_token_buffer(tokens);
    free(string_buffer);
    


    return 0;
}