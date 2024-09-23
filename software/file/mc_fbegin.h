#ifndef MC_FILE_BEGIN_HEADER
#define MC_FILE_BEGIN_HEADER

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../core/memory.h"

#if defined(_WIN32) || defined(_WIN64)  // Windows platform
    #include <windows.h>
    #define MC_IS_LITTLE_ENDIAN (!IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE))  // Simple check
#else  // Linux, macOS
    #define MC_IS_LITTLE_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#endif


#if MC_IS_LITTLE_ENDIAN
// conditional byte swapping that only takes place in little endian ordered bytes systems
// this is used for convinience when comparing bytes in numbers that (should) hold file formats or such things
#define MC_LITTLE_ENDIAN_CBSHIFT32(X) swap32(X)
// conditional byte swapping that only takes place in little endian ordered bytes systems
// this is used for convinience when comparing bytes in numbers that (should) hold file formats or such things
#define MC_LITTLE_ENDIAN_CBSHIFT64(X) swap64(X)

#else
// conditional byte swapping that only takes place in little endian ordered bytes systems
// this is used for convinience when comparing bytes in numbers that (should) hold file formats or such things
#define MC_LITTLE_ENDIAN_CBSHIFT32(X)
// conditional byte swapping that only takes place in little endian ordered bytes systems
// this is used for convinience when comparing bytes in numbers that (should) hold file formats or such things
#define MC_LITTLE_ENDIAN_CBSHIFT64(X)
#endif


typedef enum MC_FileFormat{

MC_FILEFORMAT_NONE,

// image file formats

MC_FILEFORMAT_JPEG,
MC_FILEFORMAT_PNG,
MC_FILEFORMAT_GIF,
MC_FILEFORMAT_BMP,
MC_FILEFORMAT_TIFF,

// document formats

MC_FILEFORMAT_PDF,
MC_FILEFORMAT_ZIP,
MC_FILEFORMAT_RAR,
MC_FILEFORMAT_TAR,

// audio formats

MC_FILEFORMAT_MP3,
MC_FILEFORMAT_WAV,
MC_FILEFORMAT_FLAC,
MC_FILEFORMAT_OGG,

} MC_FileFormat;


typedef struct MC_File{
    MC_FileFormat format;
    unsigned char* data;
    size_t size;
} MC_File;



MC_FileFormat mc_get_file_format(uint64_t magic_number){
    if      (0XFFD8FFE0 == (magic_number >> 32))   return MC_FILEFORMAT_JPEG;
    else if (0x89504E470D0A1A0A == magic_number)   return MC_FILEFORMAT_PNG;
    else if (0x47494638 == (magic_number >> 32))   return MC_FILEFORMAT_GIF;
    else if (0x424D == (magic_number >> 48))       return MC_FILEFORMAT_BMP;
    else if (0x49492A00 == (magic_number >> 32))   return MC_FILEFORMAT_TIFF;
    else if (0x25504446 == (magic_number >> 32))   return MC_FILEFORMAT_PDF;
    else if (0x504B0304 == (magic_number >> 32))   return MC_FILEFORMAT_ZIP;
    else if (0x52617221 == (magic_number >> 32))   return MC_FILEFORMAT_RAR;
    else if (0x7573746172 == (magic_number >> 24)) return MC_FILEFORMAT_TAR;
    else if (0x494433 == (magic_number >> 40))     return MC_FILEFORMAT_MP3;
    else if (0x52494646 == (magic_number >> 32))   return MC_FILEFORMAT_WAV;
    else if (0x664C6143 == (magic_number >> 32))   return MC_FILEFORMAT_FLAC;
    else if (0x4F676753 == (magic_number >> 32))   return MC_FILEFORMAT_OGG;
    
    return MC_FILEFORMAT_NONE;
}

void mc_print_file_format(MC_FileFormat format){
    switch (format)
    {
    case MC_FILEFORMAT_JPEG: printf("JPEG\n"); break;
    case MC_FILEFORMAT_PNG: printf("PNG\n"); break;
    case MC_FILEFORMAT_GIF: printf("GIF\n"); break;
    case MC_FILEFORMAT_BMP: printf("BMP\n"); break;
    case MC_FILEFORMAT_TIFF: printf("TIFF\n"); break;
    case MC_FILEFORMAT_PDF: printf("PDF\n"); break;
    case MC_FILEFORMAT_ZIP: printf("ZIP\n"); break;
    case MC_FILEFORMAT_RAR: printf("RAR\n"); break;
    case MC_FILEFORMAT_TAR: printf("TAR\n"); break;
    case MC_FILEFORMAT_MP3: printf("MP3\n"); break;
    case MC_FILEFORMAT_WAV: printf("WAV\n"); break;
    case MC_FILEFORMAT_FLAC: printf("FLAC\n"); break;
    case MC_FILEFORMAT_OGG: printf("OGG\n"); break;

    default:
        printf("Unknown Format\n");
        break;
    }
}

MC_File mc_fopen(const char* file_path){
    FILE* file = fopen(file_path, "rb");

    if(!file){
        fprintf(stderr, "[ERROR] Could Not Open File '%s'\n", file_path);
        return (MC_File){.format = MC_FILEFORMAT_NONE, .data = NULL, .size = 0};
    }

    const size_t bfcap = 1000000 * sizeof(unsigned char);

    MC_File output_file = (MC_File){.data = (unsigned char*)malloc(1000000 * sizeof(unsigned char)), .size = 0};

    size_t fcap = bfcap;

    
    while (!feof(file))
    {
       output_file.size += fread(output_file.data + output_file.size, 1, bfcap, file);
        if(output_file.size == fcap){
            fcap *= 2;
            unsigned char* data = output_file.data;
            output_file.data = (unsigned char*)malloc(fcap);
            for(size_t i = 0; i < output_file.size; i+=1){
                output_file.data[i] = data[i];
            }
            free(data);
        }

    }    
    fclose(file);

    uint64_t magic_number = 0X0000000000000ULL;
    if(output_file.size < sizeof(magic_number)){
        
        for(int i = 0; i < output_file.size; i+=1){
            #if MC_IS_LITTLE_ENDIAN
                magic_number |= ((uint64_t)(output_file.data[i]) << ((output_file.size - i - 1) * 8));
            #else
                magic_number |= ((uint64_t)(output_file.data[i]) << (i * 8));
            #endif
        }
        
    } else{
        for(int i = 0; i < sizeof(magic_number); i+=1){
            #if MC_IS_LITTLE_ENDIAN
                magic_number |= ((uint64_t)(output_file.data[i]) << ((sizeof(magic_number) - i - 1) * 8));
            #else
                magic_number |= ((uint64_t)(output_file.data[i]) << (i * 8));
            #endif
        }
    }

    output_file.format = mc_get_file_format(magic_number);

    return output_file;
}

void mc_fclose(MC_File file){
    free(file.data);
}




#endif // END OF FILE ========================================================