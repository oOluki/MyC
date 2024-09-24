#ifndef MC_TOKENIZER_HEADER
#define MC_TOKENIZER_HEADER

#include "../core/stream.h"
#include "../core/string.h"


typedef struct Mc_token_t
{
    // null terminated
    char* string;
    int p;

} Mc_token_t;


// returns an array of the generated tokens and writes the number of tokens to 'token_count'
// the memory at the returned pointer has to be later freed via mc_destroy_tokens (do not use free)
// \param string the string of to be tokenized
// \param __token_count the total number of tokens generated
// \param __ignored the string of the characters to be ignored, pass null to ignore trivials '\ ' '\\n' '\\t'
// \param __special_characters the characters that should be considered tokens on their own
// \param __line_comment the string that represents a line comment
// \returns the array of generated tokens
Mc_token_t* mc_tokenize(const char* string, unsigned int* __token_count, const char* __ignored,
    const char* __special_characters, const char* __line_comment){

        Mc_stream_t stream = mc_create_stream(1000 * 1000);
        Mc_stream_t tokens = mc_create_stream(1000 * sizeof(Mc_token_t));

        const char* ignored = __ignored? __ignored : " \t\n";               // avoiding null dereferencing
        const char* special_characters = __special_characters? __special_characters : "";
        const char* line_comment = __line_comment? __line_comment : "";

        //printf("configuration:\n");
        //printf("\tignored = '%s'\n", ignored);
        //printf("\tspecial characters = '%s'\n", special_characters);
        //printf("\tline_comment = '%s'\n", line_comment);   

        Mc_size_t last_token = 0;
        Mc_size_t counter = 0;
        int last_was_ignored = 0;
        unsigned int tkn_count = 0;

        for(; string[counter]; counter+=1){
            const char c = string[counter];

            if(c == line_comment[0]){
                int skip = mc_compare_str(string + counter, line_comment, 1);
                
                if(skip){
                    Mc_int_t to_ = mc_find_char(string, '\n', counter);

                    if(to_ < 0) break;
                    counter += to_;
                    last_token = counter + 1;
                    continue;
                }
            }

            Mc_byte_t is_ignored = (mc_find_char(ignored, c, 0) >= 0)? 1 : 0;
            if(is_ignored){
                if(!last_was_ignored && (last_token < counter)){ // add last token only if it wasn't a special or ignored character
                    Mc_token_t token = (Mc_token_t){.string = (char*)(stream.size)};    // be carefull, you cant simply point to the string in the stream,
                    mc_stream(&stream, string + last_token, counter - last_token);      // as the stream is dynamically reallocating memory
                    const char null_termc = '\0';                                       // instead the pointer will hold the index of the string in the stream
                    mc_stream(&stream, &null_termc, sizeof(null_termc));  
                    mc_stream(&tokens, &token, sizeof(token));
                    tkn_count += 1;
                }
                last_token = counter + 1;
                continue;
            } else last_was_ignored = 0;

            Mc_byte_t is_special = (mc_find_char(special_characters, c, 0) >= 0)? 1 : 0;

            if(is_special){
                if(last_token < counter){   // add last token only if it wasn't a special character
                    Mc_token_t token = (Mc_token_t){.string = (char*)(stream.size)};    // be carefull, you cant simply point to the string in the stream,
                    mc_stream(&stream, string + last_token, counter - last_token);      // as the stream is dynamically reallocating memory
                    const char null_termc = '\0';                                       // instead the pointer will hold the index of the string in the stream
                    mc_stream(&stream, &null_termc, sizeof(null_termc));  
                    mc_stream(&tokens, &token, sizeof(token));
                    tkn_count += 1;
                }
                Mc_token_t token = (Mc_token_t){.string = (char*)(stream.size)};
                const char dummybuff[2] = {c, '\0'};
                mc_stream(&stream, dummybuff, sizeof(dummybuff));
                mc_stream(&tokens, &token, sizeof(token));
                last_token = counter + 1;
                tkn_count += 1;
            }

        }

        if(last_token < counter){   // add last token only if appropriate
            Mc_token_t token = (Mc_token_t){.string = (char*)(stream.size)};    // be carefull, you cant simply point to the string in the stream,
            mc_stream(&stream, string + last_token, counter - last_token);      // as the stream is dynamically reallocating memory
            const char null_termc = '\0';                                       // instead the pointer will hold the index of the string in the stream
            mc_stream(&stream, &null_termc, sizeof(null_termc));  
            mc_stream(&tokens, &token, sizeof(token));
            tkn_count += 1;
        }

        mc_change_stream_cap(&stream, stream.size);

        if(__token_count) *__token_count = tkn_count;

        void* buff = malloc(tokens.size + sizeof(void*));    // allocates the tokens in a smaller memory chunk that fits them
        *(char**)(buff) = stream.data;                        // the pointer to the memory containing the strings will be placed
        Mc_token_t* output = (Mc_token_t*)((char**)buff + 1); // at the begining of the memory chunk, so it can be later freed also

        for(Mc_size_t i = 0; i < tkn_count; i += 1){
            const Mc_size_t strpos = (Mc_size_t)(((Mc_token_t*)tokens.data)[i].string);
            output[i] = (Mc_token_t){.string = (stream.data + strpos)};
        }

        return output;        
}


// frees all the memory related to the tokenization process of tokens
// (their strings and the memory chunk containing them and the tokens)
// \param the pointer to the tokens to destroy
void mc_destroy_tokens(Mc_token_t* tokens){
    void* mem_chunk = (void*)((char**)(tokens) - 1);
    char* tokens_str = *((char**)mem_chunk);
    free(tokens_str);
    free(mem_chunk);
}



#endif // =====================  END OF FILE MC_TOKENIZER_HEADER ===========================