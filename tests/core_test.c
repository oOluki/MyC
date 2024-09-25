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

    const char** string_buffer = (const char**)alloca(token_count * sizeof(char*));
    mc_buffer_token_data(string_buffer, tokens);


    printf("token_count=%u\n", token_count);

    Mc_tkn_metadata_t* meta = (Mc_tkn_metadata_t*)(tokens) - 1;
    Mc_stream_t stream = meta->streamstring;
    Mc_stream_t tknstrm = meta->tokn_strm_bff;

    printf(
        "meta:\n"
        "\tstream %p: size = %zu, cap = %zu\n"
        "\tbuffer: size = %zu, cap = %zu\n",
        stream.data, stream.size, stream.capacity,
        tknstrm.size, tknstrm.capacity
    );

    printf("[");

    for(Mc_size_t i = 0; i < token_count; i += 1){
        printf(" '%s', ", string_buffer[i]);
    }
    printf("]\n");

    mc_destroy_token_buffer(tokens);


    return 0;
}