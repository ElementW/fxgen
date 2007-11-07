/* Definitions useful for the "gforge" fractal surface generator */
/* 7/14/95 jpb */

#undef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#undef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#undef abs
#define abs(a) ((a) >= 0 ? (a) : -(a))
#undef odd
#define odd(n) ((n) & 1)

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif
#ifndef M_E
#define M_E     2.7182818284590452354
#endif

/* Definitions used to address real and imaginary parts in a two-dimensional
   array of complex numbers as stored by fourn(). */

#define Real(v, x, y)  v[1 + (((x) * meshsize) + (y)) * 2]
//#define Imag(v, x, y)  v[2 + (((x) * meshsize) + (y)) * 2]
#define Imag(v,x,y) v[x*meshheight+y]


/* Definition for obtaining random numbers. */

#define Nrand 4                       /* Gauss() sample count */
#define Cast(low, high) ((low)+(((high)-(low)) * ((rand() & 0x7FFF) / arand)))
/* Utility definition to get an  array's  element  count  (at  compile
   time).   For  example:

       int  arr[] = {1,2,3,4,5};
       ...
       printf("%d", ELEMENTS(arr));

   would print a five.  ELEMENTS("abc") can also be used to  tell  how
   many  bytes are in a string constant INCLUDING THE TRAILING NULL. */

#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

/*  Data types  */

typedef int Boolean;
#define FALSE 0
#define TRUE 1
#define V     (void)
#define U unsigned int

double sgn1(double x);
float ran1();   /* random number routine */
void seed_ran1(int seed);
