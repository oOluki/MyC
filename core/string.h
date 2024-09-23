#ifndef MC_STRING_HEADER
#define MC_STRING_HEADER
#include "setup.h"



static inline int mc_compare_str(const char* str1, const char* str2){
    Mc_uint_t i = 0;
    for( ; str1[i]; i+=1){
        if(str1[i] != str2[i]) return 0;
    }
    return !str2[i];
}

// returns the first found character position relative to the offset, or -1 if none are found
static inline Mc_int_t mc_find_char(const char* str, char c, Mc_int_t offset){
    str += offset;
    for(Mc_int_t i = 0; str[i]; i+=1){
        if(c == str[i]) return i;
    }
    return -1;
}

// works similar to cym_find_char but does so to any of the characters in the charbuff str
static inline Mc_int_t mc_find_chars(const char* str, const char* charbuff, Mc_int_t offset){
    str += offset;
    for(Mc_int_t i = 0; str[i]; i+=1){
        for(Mc_int_t j = 0; charbuff[j]; j+=1){
            if(charbuff[j] == str[i]) return i;
        }
    }
    return -1;
}




#endif // END OF FILE

