#ifndef MCMATH_UNITARY_MATRICE_HEADER
#define MCMATH_UNITARY_MATRICE_HEADER

#include "setup.h"


#ifndef MC_ABS
#define MC_ABS(X) (((X) > 0)? X : -X)
#endif



// função inverso da raiz quadrada original do Quake III, geralmente atribuida a John D. Carmack.
// leia mais em: https://en.wikipedia.org/wiki/Fast_inverse_square_root,
// fast inverse sqrt function from Quake III, most noticibly attributed to John D. Carmack.
// read more in: https://en.wikipedia.org/wiki/Fast_inverse_square_root,
float Q_rsqrt(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  //y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y;
}

float mc_sqrt(float number){return 1.0f / Q_rsqrt(number);}

void mc_normalize_vec(MC_FLOAT* vector, unsigned int size){
    MC_FLOAT norm2 = 0;

    for(unsigned int i = 0; i < size; i+=1){
        norm2 += vector[i] * vector[i];
    }

    const MC_FLOAT rnorm = Q_rsqrt(norm2);

    for(unsigned int i = 0; i < size; i+=1){
        vector[i] *= rnorm;
    }

}


void mc_mat_transpose(MC_FLOAT* input, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for(unsigned int j = 0 ; j < sizex; j+=1){
            output[j * sizey + i] = input[i * sizex + j];
        }
    }
}

void mc_mat_scale(MC_FLOAT scalar, MC_FLOAT* mat, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for (unsigned int j = 0; j < sizex; j+=1){
            output[i * sizex + j] = scalar * mat[i * sizex + j];
        }
    }
}

void mc_mat_sum(MC_FLOAT* mat1, MC_FLOAT* mat2, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for (unsigned int j = 0; j < sizex; j+=1){
            output[i * sizex + j] = mat1[i * sizex + j] + mat2[i * sizex + j];
        }
    }
}

void mc_mat_sub(MC_FLOAT* mat1, MC_FLOAT* mat2, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for (unsigned int j = 0; j < sizex; j+=1){
            output[i * sizex + j] = mat1[i * sizex + j] - mat2[i * sizex + j];
        }
    }
}

void mc_mat_mul(MC_FLOAT* mat_1, unsigned int mat1_sizex, unsigned int mat1_sizey,
MC_FLOAT* mat_2, unsigned int mat2_sizex, MC_FLOAT* output){

    const unsigned long ran = mat1_sizey * mat2_sizex;

    for(unsigned long i = 0; i < ran; i +=1){
        output[i] = 0;
    }

    for(unsigned int i = 0; i < mat1_sizey; i += 1){
        for(unsigned int j = 0; j < mat2_sizex; j += 1)
        for(unsigned int j1 = 0; j1 < mat1_sizex; j1 += 1){
            output[i * mat2_sizex + j] += mat_1[i * mat1_sizex + j1] * mat_2[j1 * mat2_sizex + j];
        }
    }

}






#endif //END OF FILE =================================================================