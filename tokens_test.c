#include <stdio.h>
#include "software/tokenizer.h"


int main(){

    unsigned int token_count;

    Mc_token_t* tokens = mc_tokenize(
        "example 1 of tokenization,// testing mc_tokenizer method//\n"
        "this is the second line of the first example\n"
        "look at me // this part is commented out\n"
        "this isn't =) \n",
        &token_count,
        " \n\t",
        ",",
        "//"
    );

    printf("token_count=%u\n", token_count);

    for(Mc_size_t i = 0; i < token_count; i += 1){
        printf("%s\n", tokens[i].string);
    }

    mc_destroy_tokens(tokens);


    return 0;
}