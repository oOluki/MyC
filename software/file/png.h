#ifndef MC_FILE_PNG_HEADER
#define MC_FILE_PNG_HEADER

#include "begin.h"




enum MC_PngChunkTypes{
MC_PNG_CHUNK_IHDR = 0x49484452,
MC_PNG_CHUNK_IDAT = 0x49444154,
MC_PNG_CHUNK_sRGB = 0x73524742,
MC_PNG_CHUNK_PLTE = 0x504c5445,
MC_PNG_CHUNK_IEND = 0x49454e44,
};

enum MC_PngColorTypes{
MC_PNG_COLORTYPE_GRAYSCALE = 0,
MC_PNG_COLORTYPE_RGB = 2,
MC_PNG_COLORTYPE_INDEXED = 3,
MC_PNG_COLORTYPE_GREYSCALEALPHA = 4,
MC_PNG_COLORTYPE_RGBA = 6
};

typedef struct MC_PngInfo{
uint32_t w;
uint32_t h;
uint8_t bit_depth;
uint8_t colortype;
uint8_t compression_method;
uint8_t filter_method;
uint8_t interlance_method;
} MC_PngInfo;



MC_PngInfo mc_get_png_file_info(const MC_File file){
    size_t IHDR_pos = 8;

    while (IHDR_pos < file.size){

        const uint32_t chunk_data_size = *(uint32_t*)(file.data + IHDR_pos);

        if(*(uint32_t*)(file.data + IHDR_pos + 4) == MC_LITTLE_ENDIAN_CBSHIFT32(MC_PNG_CHUNK_IHDR))
            break;

        IHDR_pos += chunk_data_size + 12;
    }
    

    MC_PngInfo output = (MC_PngInfo){
        .w =  MC_LITTLE_ENDIAN_CBSHIFT32(*(uint32_t*)(file.data + IHDR_pos + 8)),
        .h = MC_LITTLE_ENDIAN_CBSHIFT32(*(uint32_t*)(file.data + IHDR_pos + 12)),
        .bit_depth = *(uint8_t*)(file.data + IHDR_pos + 16),
        .colortype = *(uint8_t*)(file.data + IHDR_pos + 17),
        .compression_method = *(uint8_t*)(file.data + IHDR_pos + 18),
        .filter_method = *(uint8_t*)(file.data + IHDR_pos + 19),
        .interlance_method = *(uint8_t*)(file.data + IHDR_pos + 20)
    };
    return output;
}


// TODO: returns a buffer with the pixels in RGBA8888 format on success or NULL on failure
void* mc_get_png_data(const MC_File file, const MC_PngInfo info){

    size_t IDAT_pos = 8, counter = 0;

    uint32_t chunk_data_size = 0;

    while (IDAT_pos < file.size){

        chunk_data_size = MC_LITTLE_ENDIAN_CBSHIFT32(*(uint32_t*)(file.data + IDAT_pos));

        if(*(uint32_t*)(file.data + IDAT_pos + 4) == MC_LITTLE_ENDIAN_CBSHIFT32(MC_PNG_CHUNK_IDAT))
            break;

        IDAT_pos += chunk_data_size + 12;
    }

    for(size_t i = 0; i < chunk_data_size; i++){
        printf("%X, ", *(unsigned char*)(file.data + IDAT_pos + 8 + i));
    } printf("\n");

    return NULL;


}



#endif // END OF FILE =======================================