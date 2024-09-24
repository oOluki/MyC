#ifndef MC_STREAM_HEADER
#define MC_STREAM_HEADER

#include <stdlib.h>

#include "setup.h"
#include "memory.h"



typedef struct Mc_stream_t{
    // data buffer
    Mc_byte_t*  data;
    // size of the stream in bytes
    Mc_size_t   size;
    // capacity of the stream in bytes
    Mc_size_t   capacity;
} Mc_stream_t;



#define MC_GET_ELEMENT_FROM_STREAM(STREAM, TYPE, POS) (*(TYPE*)(STREAM.data + POS))
#define MC_PUSH_ELEMENT_TO_STREAM(STREAM, TYPE, ELEMENT) do{\
    if(sizeof(TYPE) + STREAM->size > STREAM->capacity){\
        Mc_byte_t* old_data = STREAM->data;\
        STREAM->capacity *= 1 + (Mc_size_t)((size + STREAM->size) / STREAM->capacity);\
        STREAM->data = (Mc_byte_t*)malloc(STREAM->capacity);\
        memcpy(STREAM->data, old_data, STREAM->size);\
        free(old_data);\
    }\
    *(TYPE*)(STREAM->data + STREAM->size) = ELEMENT;\
    STREAM->size += sizeof(TYPE)\
    }while (0);
    



// streams size bytes of data to stream
void mc_stream(Mc_stream_t* stream, const void* data, Mc_size_t size){
    if(size + stream->size > stream->capacity){
        Mc_byte_t* old_data = stream->data;
        stream->capacity *= 1 + (Mc_size_t)((size + stream->size) / stream->capacity);
        stream->data = (Mc_byte_t*)malloc(stream->capacity);
        memcpy(stream->data, old_data, stream->size);
        free(old_data);
    }
    memcpy(stream->data + stream->size, data, size);
    stream->size += size;
}

// works like mc_stream but streams a null treminated string
void mc_stream_str(Mc_stream_t* stream, const char* data){
    Mc_size_t size = 0;

    while (data[size++]);

    mc_stream(stream, data, size * sizeof(char));
}

// reads size bytes from stream to output
void mc_read_stream(void* output, const Mc_stream_t stream, Mc_size_t offset, Mc_size_t size){

    memcpy(output, stream.data + offset, size);

}

// \returns (Mc_stream_t){.data = (Mc_byte_t*)malloc(capacity), .size = 0, .capacity = capacity};
Mc_stream_t mc_create_stream(Mc_size_t capacity){
    return (Mc_stream_t){.data = (Mc_byte_t*)malloc(capacity), .size = 0, .capacity = capacity};
}

// this simply passes stream.data to free. free(stream.data)
void mc_destroy_stream(Mc_stream_t stream){
    free(stream.data);
}

// changes the capacity of the stream to new_cap, resizing it accordingly
void mc_change_stream_cap(Mc_stream_t* stream, Mc_size_t new_cap){
    stream->capacity = new_cap;

    Mc_byte_t* old_data = stream->data;

    stream->data = (Mc_byte_t*)malloc(new_cap);

    if(new_cap < stream->size) stream->size = new_cap;

    memcpy(stream->data, old_data, stream->size);

    free(old_data);
}


#endif // END OF FILE