#ifndef MCMATH_HEADER
#define MCMATH_HEADER

#ifndef MC_SETUP_DONE   // setting up some MyC configurations ===============================
#define MC_SETUP_DONE 1


#ifdef _STDLIB_H // if the standard library is included:

typedef size_t Mc_size_t;

#endif


#if defined(__arm__) || defined(__aarch64__) || defined(MC_INITMACRO_FORCE_ARM) // ARM architectures


#define MC_ARCHITECTURE_ARM 1

#endif


#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || \
    defined(__ppc64__) || defined(MC_INITMACRO_FORCE_64BIT) // 64 bit architectures


    #include <stdint.h>

    #define MC_ARCHITECTURE_64BIT 1

    #ifndef _STDLIB_H // if the standard library is not included:

        typedef uint64_t Mc_size_t;

    #endif

    typedef uint64_t Mc_uint_t;

    typedef int64_t Mc_int_t;

    #define MC_FLOAT double



#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || defined(MC_INITMACRO_FORCE_32BIT) // 32 bit architectures


    #include <stdint.h>

    #define MC_ARCHITECTURE_32BIT 1

    #ifndef _STDLIB_H // if the standard library is not included:

        typedef uint32_t Mc_size_t;

    #endif

    typedef uint32_t Mc_uint_t;

    typedef int32_t Mc_int_t;

#define MC_FLOAT float


#else // [WARNING] Unknown Architecture, this architecture could be unsupported and, if it doesn't support <stdint.h>, for example, it will lead to undefined behavior

    #ifndef _STDLIB_H

        typedef unsigned int Mc_size_t;

    #endif

    typedef unsigned int Mc_uint_t;

    typedef int Mc_int_t;


#endif // END OF ARCHITECTURE DEFINITION

#endif // END OF SETUP =====================================================



#ifndef MC_ABS
#define MC_ABS(X) (((X) > 0)? (X) : -(X))
#endif

#include <string.h>


// função inverso da raiz quadrada original do Quake III, geralmente atribuida a John D. Carmack.
// leia mais em: https://en.wikipedia.org/wiki/Fast_inverse_square_root,
// fast inverse sqrt function from Quake III, most noticibly attributed to John D. Carmack.
// read more in: https://en.wikipedia.org/wiki/Fast_inverse_square_root,
float mc_Q_rsqrt(float number)
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

// função inverso da raiz quadrada original do Quake III, geralmente atribuida a John D. Carmack.
// leia mais em: https://en.wikipedia.org/wiki/Fast_inverse_square_root,
// fast inverse sqrt function from Quake III, most noticibly attributed to John D. Carmack.
// read more in: https://en.wikipedia.org/wiki/Fast_inverse_square_root,
double mc_Q_rsqrt_d(double number) {
    const double x2 = number * 0.5;
    const double threehalfs = 1.5;

    union {
        double d;
        uint64_t i;
    } conv = {number};

    conv.i = 0x5fe6ec85e7de30da - (conv.i >> 1);

    conv.d = conv.d * (threehalfs - (x2 * conv.d * conv.d));
    conv.d = conv.d * (threehalfs - (x2 * conv.d * conv.d));
    conv.d = conv.d * (threehalfs - (x2 * conv.d * conv.d));
    conv.d = conv.d * (threehalfs - (x2 * conv.d * conv.d));
    conv.d = conv.d * (threehalfs - (x2 * conv.d * conv.d));

    return conv.d;
}

float mc_sqrt(float number){return 1.0f / mc_Q_rsqrt_d(number);}

void mc_normalize_vec(MC_FLOAT* vector, unsigned int size){
    MC_FLOAT norm2 = 0;

    for(unsigned int i = 0; i < size; i+=1){
        norm2 += vector[i] * vector[i];
    }

    const MC_FLOAT rnorm = mc_Q_rsqrt_d(norm2);

    for(unsigned int i = 0; i < size; i+=1){
        vector[i] *= rnorm;
    }

}


void mc_mat_transpose(const MC_FLOAT* input, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for(unsigned int j = 0 ; j < sizex; j+=1){
            output[j * sizey + i] = input[i * sizex + j];
        }
    }
}

void mc_mat_scale(MC_FLOAT scalar, const MC_FLOAT* mat, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for (unsigned int j = 0; j < sizex; j+=1){
            output[i * sizex + j] = scalar * mat[i * sizex + j];
        }
    }
}

void mc_mat_sum(const MC_FLOAT* mat1, const MC_FLOAT* mat2, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for (unsigned int j = 0; j < sizex; j+=1){
            output[i * sizex + j] = mat1[i * sizex + j] + mat2[i * sizex + j];
        }
    }
}

void mc_mat_sub(const MC_FLOAT* mat1, const MC_FLOAT* mat2, unsigned int sizex, unsigned int sizey, MC_FLOAT* output){
    for(unsigned int i = 0; i < sizey; i+=1){
        for (unsigned int j = 0; j < sizex; j+=1){
            output[i * sizex + j] = mat1[i * sizex + j] - mat2[i * sizex + j];
        }
    }
}

void mc_mat_mul(const MC_FLOAT* mat_1, unsigned int mat1_sizex, unsigned int mat1_sizey,
const MC_FLOAT* mat_2, unsigned int mat2_sizex, MC_FLOAT* output){

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


// solves the system a * x = y through gauss method, outputing the result to x
// this modifies the memory at a and y
void mc_solve_gauss(MC_FLOAT* a, MC_FLOAT* y, unsigned int size, MC_FLOAT* x){

    #ifdef alloca
    Mc_size_t* index = (Mc_size_t*)alloca(2 * size * sizeof(Mc_size_t));
    #else
    Mc_size_t* index = (Mc_size_t)malloc(2 * size * sizeof(Mc_size_t));
    #endif

    // a variable to store the pivot line
    int64_t t = 0;

    for(unsigned int m = 0; m < size; m += 1){
        t = -1;
        for(unsigned int n = 0; n < size; n += 1){

            if(a[n * size + m] != 0 && t >= 0){ // if a pivot was found

                const MC_FLOAT scale = a[n * size + m] / a[t * size + m];
                // using x to temporarilly store a[t] * (a[n][m] / a[t][m]) {a[n] is a vector/matrice_line}
                mc_mat_scale(scale, a + t * size, size, 1, x);
                // a[n] -= a[t] * (a[n][m] / a[t][m])
                mc_mat_sub(a + n * size, x, size, 1, a + n * size);
                // y[n] -= y[t] * (a[n][m] / a[t][m])
                y[n] -= y[t] * scale;

            } else if(a[n * size + m] != 0){ // otherwise set the pivot
                t = n;
            }
        }
        if(t >= 0){// if there was a pivot send its line to the end

            const MC_FLOAT yy = y[t];

            for(unsigned int n = t + 1; n < size; n+=1){
                y[n - 1] = y[n];
            }

            y[size - 1] = yy;

            for(unsigned int i = 0; i < size; i+=1){
                x[i] = a[t * size + i];

                for(unsigned int n = t + 1; n < size; n+=1){
                    a[(n - 1) * size + i] = a[n * size + i];
                }

                a[(size - 1) * size + i] = x[i];
            }

        }
    }

    for(unsigned int m = 0; m < size; m += 1){
        for(unsigned int n = 0; n < size; n += 1){
            if(a[n * size + m]){
                x[n] = y[m] / a[n * size + m];
            }
        }
    }

    #ifndef alloca
    free(index);
    #endif
}

// takes a vector and outputs an unitary matrice with its first column proportional to that vector
// \returns 0 on success or 1 if the input vector is zero (it still writes a 0 matrice to output)
int mc_make_unitary(const MC_FLOAT* input, unsigned int size, MC_FLOAT* output){

    MC_FLOAT norm2 = 0.0f;
    MC_FLOAT rnorm = 0.0f;

    for(unsigned int i = 0; i < size; i+=1){
        norm2 += input[i] * input[i];
    }

    if(norm2 == 0.0f){
        for(Mc_size_t i = 0; i < size * size; i+=1){
            output[i] = 0.0f;
        }
        return 1;
    }

    rnorm = mc_Q_rsqrt_d(norm2);

    for(unsigned int i = 0; i < size; i+=1){

        output[i * size] = input[i] * rnorm;

    }
    

    unsigned int j = 1;

   while((j < size) && (input[j - 1] == 0.0f)){
        output[(j - 1) * size + j] = 1.0f;
        j += 1;
    }

    for(; j < size; j+=1){

        if(input[j] != 0.0f){
            MC_FLOAT dummy = 0.0f;

            for(unsigned int i = 0; i < j; i+=1){
                output[i * size + j] = input[i];
                dummy -= input[i] * input[i];
            }

            output[j * (size + 1)] = dummy / input[j];

            norm2 = output[j * (size + 1)] * output[j * (size + 1)] - dummy;

            rnorm = mc_Q_rsqrt_d(norm2);

            for(unsigned int i = 0; i < j + 1; i+=1){
                output[i * size + j] *= rnorm;
            }
            
        } else{
            output[j * (size + 1)] = 1.0f;
        }

        
    }

    return 0;
}

int mc_test_unitary(const MC_FLOAT* mat, unsigned int size, double accuracy){
    MC_FLOAT dag[size * size];

    mc_mat_transpose(mat, size, size, dag);

    MC_FLOAT I[size * size];

    mc_mat_mul(mat, size, size, dag, size, I);

    int is_un = 1;

    for(int i = 0; i < size && is_un; i++){
        for(int j = 0; j < size && is_un; j++){
            if(i == j){
                is_un = MC_ABS(I[i * size + j] - 1) < accuracy;
            } else{
                is_un = MC_ABS(I[i * size + j]) < accuracy;
            }
        }
    }
    if(!is_un){
        return 0;
    }

    mc_mat_mul(dag, size, size, mat, size, I);

    for(int i = 0; i < size && is_un; i++){
        for(int j = 0; j < size && is_un; j++){
            if(i == j){
                is_un = MC_ABS(I[i * size + j] - 1) < accuracy;
            } else{
                is_un = MC_ABS(I[i * size + j]) < accuracy;
            }
        }
    }

    return is_un;

}

// X=======================X (DATA ANALYSIS) X==================X

// performs a polynomial interolation and outputs the coefficients to output in order of smallest power coefficient to biggest
// \returns 0 on success, 1 otherwise
int mc_interpol(const MC_FLOAT* x, const MC_FLOAT* y, Mc_size_t number_of_points, MC_FLOAT* output){

    MC_FLOAT* a = (MC_FLOAT*)malloc(number_of_points * number_of_points * sizeof(MC_FLOAT) + number_of_points * sizeof(MC_FLOAT));

    MC_FLOAT* y_ = a + number_of_points * number_of_points;

    memcpy(y_, y, number_of_points * sizeof(MC_FLOAT));

    for(Mc_size_t i = 0; i < number_of_points; i+=1){
        a[i * number_of_points] = 1.0;
        const MC_FLOAT xn = x[i];
        for(Mc_size_t j = 1; j < number_of_points; j+=1){
            a[i * number_of_points + j] = a[i * number_of_points + j - 1] * xn;
        }
    }

    mc_solve_gauss(a, y_, number_of_points, output);

    free(a);

    return 0;
}


// performs a linear fit of the form y = A*x + B
void mc_linear_fit(const MC_FLOAT* x, const MC_FLOAT* y, Mc_size_t number_of_points, MC_FLOAT* a, MC_FLOAT* b, MC_FLOAT* r){

    MC_FLOAT sumx  = 0;
    MC_FLOAT sumx2 = 0;
    MC_FLOAT sumy  = 0;
    MC_FLOAT sumy2 = 0;
    MC_FLOAT sumyx = 0;

    for (Mc_size_t i = 0; i < number_of_points; i++){
        sumx  += x[i];
        sumx2 += x[i] * x[i];
        sumy  += y[i];
        sumy2 += y[i] * y[i];
        sumyx += y[i] * x[i];
    }

    const MC_FLOAT ar_numerador  = (number_of_points * sumyx - sumx * sumy);
    const MC_FLOAT a_denominador = (number_of_points * sumx2 - sumx * sumx);

    *a = ar_numerador / a_denominador;
    *b = (sumy - (*a) * sumx) / number_of_points;
    *r = ar_numerador * mc_Q_rsqrt_d((number_of_points * sumy2 - sumy * sumy) * a_denominador);
}

// performs a wheighted linear fit of the form y = A*x + B
int mc_rlinear_fit(const MC_FLOAT* x, const MC_FLOAT* y, const MC_FLOAT* dx, const MC_FLOAT* dy,
        Mc_size_t number_of_points, MC_FLOAT* a, MC_FLOAT* b, MC_FLOAT* da, MC_FLOAT* db ,MC_FLOAT* r){

    MC_FLOAT sumx  = 0;
    MC_FLOAT sumx2 = 0;
    MC_FLOAT sumy  = 0;
    MC_FLOAT sumy2 = 0;
    MC_FLOAT sumyx = 0;

    for (Mc_size_t i = 0; i < number_of_points; i++){
        sumx  += x[i];
        sumx2 += x[i] * x[i];
        sumy  += y[i];
        sumy2 += y[i] * y[i];
        sumyx += y[i] * x[i];
    }

    const MC_FLOAT a_numerador  = (number_of_points * sumyx - sumx * sumy);
    const MC_FLOAT a_denominador = (number_of_points * sumx2 - sumx * sumx);

    MC_FLOAT av = a_numerador / a_denominador;
    MC_FLOAT bv = (sumy - av * sumx) / number_of_points;
    *r           = a_numerador * mc_Q_rsqrt_d((number_of_points * sumy2 - sumy * sumy) * a_denominador);

    MC_FLOAT last_b = bv + 1;
    MC_FLOAT dav    = 0;
    MC_FLOAT dbv    = 0;

    int iterations = 0;

    while(MC_ABS(bv - last_b) > dbv && iterations++ < 1000){
        last_b = bv;

        MC_FLOAT sumw   = 0;
        MC_FLOAT sumwx  = 0;
        MC_FLOAT sumwx2 = 0;
        MC_FLOAT sumwy  = 0;
        MC_FLOAT sumwxy = 0;

        for (Mc_size_t i = 0; i < number_of_points; i+=1)
        {
            const MC_FLOAT w = 1.0 / (dy[i] * dy[i] + dx[i] * dx[i] * av * av);

            sumw   += w;
            sumwx  += w * x[i];
            sumwx2 += w * x[i] * x[i];
            sumwy  += w * y[i];
            sumwxy += w * x[i] * y[i];

        }

        const MC_FLOAT val = sumw * sumwx2 - sumwx * sumwx;

        av  = (sumw * sumwxy - sumwx * sumwy) / val;
        bv  = (sumwy - sumwx * av) / sumw;
        dav = 1.0 / mc_Q_rsqrt_d(sumw / val);
        dbv = 1.0 / mc_Q_rsqrt_d(sumwx2 / val);
    }
    
    if(iterations > 1000){
        return -1;
    }

    *a  = av;
    *b  = bv;
    *da = dav;
    *db = dbv;

    return 0;
}





#endif //END OF FILE =================================================================