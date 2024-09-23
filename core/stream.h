#ifndef MC_STREAM_HEADER
#define MC_STREAM_HEADER

#include <stdlib.h>

#include "setup.h"
#include "memory.h"



typedef struct Mc_stream_t{
    // data buffer
    unsigned char*  data;
    // size of the stream in bytes
    Mc_size_t       size;
    // capacity of the stream in bytes
    Mc_size_t          capacity;
} Mc_stream_t;





// streams size bytes of data to stream
void mc_stream(Mc_stream_t* stream, void* data, Mc_size_t size){
    if(size + stream->size > stream->capacity){
        unsigned char* old_data = stream->data;
        stream->capacity *= 1 + (Mc_size_t)((size + stream->size) / stream->capacity);
        stream->data = (unsigned char*)malloc(stream->capacity);
        mc_memcpy(stream->data, old_data, stream->size);
        free(old_data);
    }
    mc_memcpy(stream->data + stream->size, data, size);
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

    mc_memcpy(output, stream.data + offset, size);

}



#endif // END OF FILE