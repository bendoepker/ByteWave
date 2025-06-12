#include "bw-fft.h"
#include <math.h>
#include <complex.h>

#ifdef _MSC_VER
#define ComplexFloat _Fcomplex
#define ComplexFloatMult(x, y) _FCmulcc((x), (y))
#define ComplexFloatAdd(x, y) _FCbuild(crealf((x)) + crealf((y)), cimagf((x)) + cimagf((y)))
#define ComplexFloatSub(x, y) _FCbuild(crealf((x)) - crealf((y)), cimagf((x)) - cimagf((y)))
#define ComplexFloatFromReal(x) _FCbuild((x), 0)
#define ComplexFloatFromImag(x) _FCbuild(0, (x))
#define ComplexFloatFromBoth(x, y) _FCbuild((x), (y))
#else
#define ComplexFloat float complex
#define ComplexFloatMult(x, y) ((x) * (y))
#define ComplexFloatAdd(x, y) ((x) + (y))
#define ComplexFloatSub(x, y) ((x) - (y))
#define ComplexFloatFromReal(x) (x)
#define ComplexFloatFromImag(x) (0 + (x * I))
#define ComplexFloatFromBoth(x, y) ((x), ((y) * I))
#endif

#define PI 3.14159265358979323846f

/*
* Cooley-Tukey FFT Algorithm 
* (Wiki)[https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm]
*/
void __impl_fft(ComplexFloat* buf, int n){
    if(n == 1) {
        return;
    }
    /*         Input Buffer     Output Buffer      N      Stride     */
    ComplexFloat even_buf[n/2];
    ComplexFloat odd_buf[n/2];

    for(int i = 0; i < n / 2; i++) {
        even_buf[i] = ComplexFloatFromReal(crealf(buf[(2 * i)]));
        odd_buf[i] = ComplexFloatFromReal(crealf(buf[(2 * i) + 1]));
    }
    __impl_fft(even_buf, n / 2);    //Even FFT
    __impl_fft(odd_buf, n / 2);     //Odd FFT

    /*
    *   Function from Wiki:
    *
    *       X[k] = X[k] + e^(-2 * π * k / n) * X[k + n/2]
    *
    *   Can be written as
    *
    *       X[k] = X[k] + e^(-2 * π / n)^k * X[k + n/2]
    *
    *   Using Euler's formula:
    *
    *       e^(-2 * π / n)
    *
    *           becomes
    *
    *       cos(-2 * π / n) + i * sin(-2 * π / n)
    *
    *   Extracting (-2 * π / n) as 'theta' and reinserting into formula:
    *
    *       X[k] = X[k] + (cos(theta) + i * sin(theta))^k * X[k + n/2]
    *
    *   Evaluating k == 0 (First case):
    *
    *       (cos(theta) + i * sin(theta)) => 1   (no complex part)
    *
    *   Thus we can begin with base case 'bc' as ComplexFloat 1+0j
    *   and on each iteration we can multiply 'bc' by the multiplier 'mc' giving us
    *   the same result as if we were to calculate theta, raise it to the
    *   power 'k' and store it in the value 'bc'
    *
    */

    ComplexFloat bc = ComplexFloatFromReal(1);
    float theta = 2.0f * PI / (float)n;
    ComplexFloat mc = ComplexFloatFromBoth(cos(theta), sin(theta));
    for(int k = 0; k < (n / 2); k++) {
        //ComplexFloat p = buf_out[k]
        ComplexFloat q = ComplexFloatMult(bc, odd_buf[k]);
        buf[k] = ComplexFloatAdd(even_buf[k], q);
        buf[k + (n / 2)] = ComplexFloatSub(even_buf[k], q);

        //mc^k
        bc = ComplexFloatMult(bc, mc);
    }
}

void fft(float* input_buffer, float* output_buffer, int num_elements) {
    ComplexFloat internal_buffer[num_elements];
    for(int i = 0; i < num_elements; i++) {
        internal_buffer[i] = ComplexFloatFromReal(input_buffer[i]);
    }
    __impl_fft(internal_buffer, num_elements);
    for(int i = 0; i < num_elements; i++) {
        output_buffer[i] = crealf(internal_buffer[i]);
    }
}
