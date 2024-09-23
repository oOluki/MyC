#ifndef MC_MEMORY_HEADER
#define MC_MEMORY_HEADER

#include <stdint.h>
#include "setup.h"


typedef unsigned char Mc_byte_t;


int mc_memcmp(const void* mem1, const void* mem2, Mc_size_t size){

    for(Mc_size_t i = 0; i < size; i += sizeof(Mc_size_t)){
        if(*(Mc_size_t*)((Mc_byte_t*)(mem1) + i) != *(Mc_size_t*)((Mc_byte_t*)(mem1) + i)){
            return 0;
        }
    }
    for(Mc_size_t i = sizeof(Mc_size_t) * (Mc_size_t)(size / sizeof(Mc_size_t)); i < size; i+=1){
        if(((Mc_byte_t*)mem1)[i] != ((Mc_byte_t*)mem2)[i]){
            return 0;
        }
    }
    return 1;
}

void mc_memcpy(void* dest, const void* src, Mc_size_t size){
    for(Mc_size_t i = 0; i < size; i += sizeof(Mc_size_t)){
        *(Mc_size_t*)((Mc_byte_t*)(dest) + i) = *(Mc_size_t*)((Mc_byte_t*)(src) + i);
    }
    for(Mc_size_t i = sizeof(Mc_size_t) * (Mc_size_t)(size / sizeof(Mc_size_t)); i < size; i+=1){
        ((Mc_byte_t*)dest)[i] = ((Mc_byte_t*)src)[i];
    }
}



static inline uint32_t mc_swap32(uint32_t x){
    return (
        ((x & 0X000000FF) << 24) |
        ((x & 0X0000FF00) << 8) |
        ((x & 0X00FF0000) >> 8) |
        ((x & 0XFF000000) >> 24)
    );
}



static inline uint64_t mc_swap64(uint64_t x) {
    return ((x & 0x00000000000000FFULL) << 56) |
        ((x & 0x000000000000FF00ULL) << 40) |
        ((x & 0x0000000000FF0000ULL) << 24) |
        ((x & 0x00000000FF000000ULL) << 8)  |
        ((x & 0x000000FF00000000ULL) >> 8)  |
        ((x & 0x0000FF0000000000ULL) >> 24) |
        ((x & 0x00FF000000000000ULL) >> 40) |
        ((x & 0xFF00000000000000ULL) >> 56);
}




#endif // END OF FILE ===============================================================