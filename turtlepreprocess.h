# 0 "include/turtle.h"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "include/turtle.h"
# 11 "include/turtle.h"
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
       
# 11 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3


# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/crtdefs.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/crtdefs.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_mac.h" 1 3
# 98 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_mac.h" 3
             
# 107 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_mac.h" 3
             
# 316 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_mac.h" 3
       
# 395 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_mac.h" 3
       
# 11 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 2 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_secapi.h" 1 3
# 12 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 2 3
# 282 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/vadefs.h" 1 3
# 9 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/vadefs.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 1 3
# 661 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sdks/_mingw_ddk.h" 1 3
# 662 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 2 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/vadefs.h" 2 3




#pragma pack(push,_CRT_PACKING)
# 24 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/vadefs.h" 3
  
# 24 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/vadefs.h" 3
 typedef __builtin_va_list __gnuc_va_list;






  typedef __gnuc_va_list va_list;
# 103 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/vadefs.h" 3
#pragma pack(pop)
# 283 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 2 3
# 580 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 3
void __attribute__((__cdecl__)) __debugbreak(void);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
{

  __asm__ __volatile__("int {$}3":);







}
# 601 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 3
void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) __fastfail(unsigned int code);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) __fastfail(unsigned int code)
{

  __asm__ __volatile__("int {$}0x29"::"c"(code));
# 615 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 3
  __builtin_unreachable();
}
# 641 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw.h" 3
const char *__mingw_get_crt_info (void);
# 11 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 2 3




#pragma pack(push,_CRT_PACKING)
# 35 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
__extension__ typedef unsigned long long size_t;
# 45 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
__extension__ typedef long long ssize_t;






typedef size_t rsize_t;
# 62 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
__extension__ typedef long long intptr_t;
# 75 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
__extension__ typedef unsigned long long uintptr_t;
# 88 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
__extension__ typedef long long ptrdiff_t;
# 98 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
typedef unsigned short wchar_t;







typedef unsigned short wint_t;
typedef unsigned short wctype_t;





typedef int errno_t;




typedef long __time32_t;




__extension__ typedef long long __time64_t;
# 138 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
typedef __time64_t time_t;
# 430 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct,*_locale_t;



typedef struct tagLC_ID {
  unsigned short wLanguage;
  unsigned short wCountry;
  unsigned short wCodePage;
} LC_ID,*LPLC_ID;




typedef struct threadlocaleinfostruct {





  int refcount;
  unsigned int lc_codepage;
  unsigned int lc_collate_cp;
  unsigned long lc_handle[6];
  LC_ID lc_id[6];
  struct {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
  } lc_category[6];
  int lc_clike;
  int mb_cur_max;
  int *lconv_intl_refcount;
  int *lconv_num_refcount;
  int *lconv_mon_refcount;
  struct lconv *lconv;
  int *ctype1_refcount;
  unsigned short *ctype1;
  const unsigned short *pctype;
  const unsigned char *pclmap;
  const unsigned char *pcumap;
  struct __lc_time_data *lc_time_curr;

} threadlocinfo;
# 501 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt.h" 3
#pragma pack(pop)
# 11 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/crtdefs.h" 2 3
# 14 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 2 3

struct _exception;

#pragma pack(push,_CRT_PACKING)
# 119 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  typedef union __mingw_dbl_type_t {
    double x;
    unsigned long long val;
    __extension__ struct {
      unsigned int low, high;
    } lh;
  } __mingw_dbl_type_t;

  typedef union __mingw_flt_type_t {
    float x;
    unsigned int val;
  } __mingw_flt_type_t;

  typedef union __mingw_ldbl_type_t
  {
    long double x;
    __extension__ struct {
      unsigned int low, high;
      int sign_exponent : 16;
      int res1 : 16;
      int res0 : 32;
    } lh;
  } __mingw_ldbl_type_t;
# 150 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern double * __imp__HUGE;
# 163 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  struct _exception {
    int type;
    const char *name;
    double arg1;
    double arg2;
    double retval;
  };

  void __mingw_raise_matherr (int typ, const char *name, double a1, double a2,
         double rslt);
  void __mingw_setusermatherr (int (__attribute__((__cdecl__)) *)(struct _exception *));
  __attribute__ ((__dllimport__)) void __setusermatherr(int (__attribute__((__cdecl__)) *)(struct _exception *));



  double __attribute__((__cdecl__)) sin(double _X);
  double __attribute__((__cdecl__)) cos(double _X);
  double __attribute__((__cdecl__)) tan(double _X);
  double __attribute__((__cdecl__)) sinh(double _X);
  double __attribute__((__cdecl__)) cosh(double _X);
  double __attribute__((__cdecl__)) tanh(double _X);
  double __attribute__((__cdecl__)) asin(double _X);
  double __attribute__((__cdecl__)) acos(double _X);
  double __attribute__((__cdecl__)) atan(double _X);
  double __attribute__((__cdecl__)) atan2(double _Y,double _X);
  double __attribute__((__cdecl__)) exp(double _X);
  double __attribute__((__cdecl__)) log(double _X);
  double __attribute__((__cdecl__)) log10(double _X);
  double __attribute__((__cdecl__)) pow(double _X,double _Y);
  double __attribute__((__cdecl__)) sqrt(double _X);
  double __attribute__((__cdecl__)) ceil(double _X);
  double __attribute__((__cdecl__)) floor(double _X);


  extern float __attribute__((__cdecl__)) fabsf (float x);
  extern long double __attribute__((__cdecl__)) fabsl (long double);
  extern double __attribute__((__cdecl__)) fabs (double _X);
# 238 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  double __attribute__((__cdecl__)) ldexp(double _X,int _Y);
  double __attribute__((__cdecl__)) frexp(double _X,int *_Y);
  double __attribute__((__cdecl__)) modf(double _X,double *_Y);
  double __attribute__((__cdecl__)) fmod(double _X,double _Y);

  void __attribute__((__cdecl__)) sincos (double __x, double *p_sin, double *p_cos);
  void __attribute__((__cdecl__)) sincosl (long double __x, long double *p_sin, long double *p_cos);
  void __attribute__((__cdecl__)) sincosf (float __x, float *p_sin, float *p_cos);



  int __attribute__((__cdecl__)) abs(int _X);
  long __attribute__((__cdecl__)) labs(long _X);



  double __attribute__((__cdecl__)) atof(const char *_String);
  double __attribute__((__cdecl__)) _atof_l(const char *_String,_locale_t _Locale);
# 265 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  struct _complex {
    double x;
    double y;
  };


  double __attribute__((__cdecl__)) _cabs(struct _complex _ComplexA);
  double __attribute__((__cdecl__)) _hypot(double _X,double _Y);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _j0(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _j1(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _jn(int _X,double _Y);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _y0(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _y1(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _yn(int _X,double _Y);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _matherr (struct _exception *);
# 292 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _chgsign (double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _copysign (double _Number,double _Sign);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _logb (double);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _nextafter (double, double);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _scalb (double, long);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _finite (double);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fpclass (double);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _isnan (double);






__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) j0 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) j1 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) jn (int, double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) y0 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) y1 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) yn (int, double) ;

__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) chgsign (double);
# 322 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) finite (double);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) fpclass (double);
# 367 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
typedef float float_t;
typedef double double_t;
# 402 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern int __attribute__((__cdecl__)) __fpclassifyl (long double);
  extern int __attribute__((__cdecl__)) __fpclassifyf (float);
  extern int __attribute__((__cdecl__)) __fpclassify (double);
# 515 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern int __attribute__((__cdecl__)) __isnan (double);
  extern int __attribute__((__cdecl__)) __isnanf (float);
  extern int __attribute__((__cdecl__)) __isnanl (long double);
# 602 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern int __attribute__((__cdecl__)) __signbit (double);
  extern int __attribute__((__cdecl__)) __signbitf (float);
  extern int __attribute__((__cdecl__)) __signbitl (long double);
# 659 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern float __attribute__((__cdecl__)) sinf(float _X);
  extern long double __attribute__((__cdecl__)) sinl(long double);

  extern float __attribute__((__cdecl__)) cosf(float _X);
  extern long double __attribute__((__cdecl__)) cosl(long double);

  extern float __attribute__((__cdecl__)) tanf(float _X);
  extern long double __attribute__((__cdecl__)) tanl(long double);
  extern float __attribute__((__cdecl__)) asinf(float _X);
  extern long double __attribute__((__cdecl__)) asinl(long double);

  extern float __attribute__((__cdecl__)) acosf (float);
  extern long double __attribute__((__cdecl__)) acosl (long double);

  extern float __attribute__((__cdecl__)) atanf (float);
  extern long double __attribute__((__cdecl__)) atanl (long double);

  extern float __attribute__((__cdecl__)) atan2f (float, float);
  extern long double __attribute__((__cdecl__)) atan2l (long double, long double);


  extern float __attribute__((__cdecl__)) sinhf(float _X);



  extern long double __attribute__((__cdecl__)) sinhl(long double);

  extern float __attribute__((__cdecl__)) coshf(float _X);



  extern long double __attribute__((__cdecl__)) coshl(long double);

  extern float __attribute__((__cdecl__)) tanhf(float _X);



  extern long double __attribute__((__cdecl__)) tanhl(long double);



  extern double __attribute__((__cdecl__)) acosh (double);
  extern float __attribute__((__cdecl__)) acoshf (float);
  extern long double __attribute__((__cdecl__)) acoshl (long double);


  extern double __attribute__((__cdecl__)) asinh (double);
  extern float __attribute__((__cdecl__)) asinhf (float);
  extern long double __attribute__((__cdecl__)) asinhl (long double);


  extern double __attribute__((__cdecl__)) atanh (double);
  extern float __attribute__((__cdecl__)) atanhf (float);
  extern long double __attribute__((__cdecl__)) atanhl (long double);



  extern float __attribute__((__cdecl__)) expf(float _X);



  extern long double __attribute__((__cdecl__)) expl(long double);


  extern double __attribute__((__cdecl__)) exp2(double);
  extern float __attribute__((__cdecl__)) exp2f(float);
  extern long double __attribute__((__cdecl__)) exp2l(long double);



  extern double __attribute__((__cdecl__)) expm1(double);
  extern float __attribute__((__cdecl__)) expm1f(float);
  extern long double __attribute__((__cdecl__)) expm1l(long double);


  extern float frexpf(float _X,int *_Y);



  extern long double __attribute__((__cdecl__)) frexpl(long double,int *);




  extern int __attribute__((__cdecl__)) ilogb (double);
  extern int __attribute__((__cdecl__)) ilogbf (float);
  extern int __attribute__((__cdecl__)) ilogbl (long double);


  extern float __attribute__((__cdecl__)) ldexpf(float _X,int _Y);



  extern long double __attribute__((__cdecl__)) ldexpl (long double, int);


  extern float __attribute__((__cdecl__)) logf (float);
  extern long double __attribute__((__cdecl__)) logl(long double);


  extern float __attribute__((__cdecl__)) log10f (float);
  extern long double __attribute__((__cdecl__)) log10l(long double);


  extern double __attribute__((__cdecl__)) log1p(double);
  extern float __attribute__((__cdecl__)) log1pf(float);
  extern long double __attribute__((__cdecl__)) log1pl(long double);


  extern double __attribute__((__cdecl__)) log2 (double);
  extern float __attribute__((__cdecl__)) log2f (float);
  extern long double __attribute__((__cdecl__)) log2l (long double);


  extern double __attribute__((__cdecl__)) logb (double);
  extern float __attribute__((__cdecl__)) logbf (float);
  extern long double __attribute__((__cdecl__)) logbl (long double);
# 862 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern float __attribute__((__cdecl__)) modff (float, float*);
  extern long double __attribute__((__cdecl__)) modfl (long double, long double*);


  extern double __attribute__((__cdecl__)) scalbn (double, int);
  extern float __attribute__((__cdecl__)) scalbnf (float, int);
  extern long double __attribute__((__cdecl__)) scalbnl (long double, int);

  extern double __attribute__((__cdecl__)) scalbln (double, long);
  extern float __attribute__((__cdecl__)) scalblnf (float, long);
  extern long double __attribute__((__cdecl__)) scalblnl (long double, long);



  extern double __attribute__((__cdecl__)) cbrt (double);
  extern float __attribute__((__cdecl__)) cbrtf (float);
  extern long double __attribute__((__cdecl__)) cbrtl (long double);


  extern double __attribute__((__cdecl__)) hypot (double, double) ;
  extern float __attribute__((__cdecl__)) hypotf (float x, float y);



  extern long double __attribute__((__cdecl__)) hypotl (long double, long double);


  extern float __attribute__((__cdecl__)) powf(float _X,float _Y);



  extern long double __attribute__((__cdecl__)) powl (long double, long double);


  extern float __attribute__((__cdecl__)) sqrtf (float);
  extern long double sqrtl(long double);


  extern double __attribute__((__cdecl__)) erf (double);
  extern float __attribute__((__cdecl__)) erff (float);
  extern long double __attribute__((__cdecl__)) erfl (long double);


  extern double __attribute__((__cdecl__)) erfc (double);
  extern float __attribute__((__cdecl__)) erfcf (float);
  extern long double __attribute__((__cdecl__)) erfcl (long double);


  extern double __attribute__((__cdecl__)) lgamma (double);
  extern float __attribute__((__cdecl__)) lgammaf (float);
  extern long double __attribute__((__cdecl__)) lgammal (long double);

  extern int signgam;


  extern double __attribute__((__cdecl__)) tgamma (double);
  extern float __attribute__((__cdecl__)) tgammaf (float);
  extern long double __attribute__((__cdecl__)) tgammal (long double);


  extern float __attribute__((__cdecl__)) ceilf (float);
  extern long double __attribute__((__cdecl__)) ceill (long double);


  extern float __attribute__((__cdecl__)) floorf (float);
  extern long double __attribute__((__cdecl__)) floorl (long double);


  extern double __attribute__((__cdecl__)) nearbyint ( double);
  extern float __attribute__((__cdecl__)) nearbyintf (float);
  extern long double __attribute__((__cdecl__)) nearbyintl (long double);



extern double __attribute__((__cdecl__)) rint (double);
extern float __attribute__((__cdecl__)) rintf (float);
extern long double __attribute__((__cdecl__)) rintl (long double);


extern long __attribute__((__cdecl__)) lrint (double);
extern long __attribute__((__cdecl__)) lrintf (float);
extern long __attribute__((__cdecl__)) lrintl (long double);

__extension__ long long __attribute__((__cdecl__)) llrint (double);
__extension__ long long __attribute__((__cdecl__)) llrintf (float);
__extension__ long long __attribute__((__cdecl__)) llrintl (long double);
# 1029 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern double __attribute__((__cdecl__)) round (double);
  extern float __attribute__((__cdecl__)) roundf (float);
  extern long double __attribute__((__cdecl__)) roundl (long double);


  extern long __attribute__((__cdecl__)) lround (double);
  extern long __attribute__((__cdecl__)) lroundf (float);
  extern long __attribute__((__cdecl__)) lroundl (long double);
  __extension__ long long __attribute__((__cdecl__)) llround (double);
  __extension__ long long __attribute__((__cdecl__)) llroundf (float);
  __extension__ long long __attribute__((__cdecl__)) llroundl (long double);



  extern double __attribute__((__cdecl__)) trunc (double);
  extern float __attribute__((__cdecl__)) truncf (float);
  extern long double __attribute__((__cdecl__)) truncl (long double);


  extern float __attribute__((__cdecl__)) fmodf (float, float);
  extern long double __attribute__((__cdecl__)) fmodl (long double, long double);


  extern double __attribute__((__cdecl__)) remainder (double, double);
  extern float __attribute__((__cdecl__)) remainderf (float, float);
  extern long double __attribute__((__cdecl__)) remainderl (long double, long double);


  extern double __attribute__((__cdecl__)) remquo(double, double, int *);
  extern float __attribute__((__cdecl__)) remquof(float, float, int *);
  extern long double __attribute__((__cdecl__)) remquol(long double, long double, int *);


  extern double __attribute__((__cdecl__)) copysign (double, double);
  extern float __attribute__((__cdecl__)) copysignf (float, float);
  extern long double __attribute__((__cdecl__)) copysignl (long double, long double);
# 1086 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern double __attribute__((__cdecl__)) nan(const char *tagp);
  extern float __attribute__((__cdecl__)) nanf(const char *tagp);
  extern long double __attribute__((__cdecl__)) nanl(const char *tagp);
# 1097 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
  extern double __attribute__((__cdecl__)) nextafter (double, double);
  extern float __attribute__((__cdecl__)) nextafterf (float, float);
  extern long double __attribute__((__cdecl__)) nextafterl (long double, long double);


  extern double __attribute__((__cdecl__)) nexttoward (double, long double);
  extern float __attribute__((__cdecl__)) nexttowardf (float, long double);
  extern long double __attribute__((__cdecl__)) nexttowardl (long double, long double);



  extern double __attribute__((__cdecl__)) fdim (double x, double y);
  extern float __attribute__((__cdecl__)) fdimf (float x, float y);
  extern long double __attribute__((__cdecl__)) fdiml (long double x, long double y);







  extern double __attribute__((__cdecl__)) fmax (double, double);
  extern float __attribute__((__cdecl__)) fmaxf (float, float);
  extern long double __attribute__((__cdecl__)) fmaxl (long double, long double);


  extern double __attribute__((__cdecl__)) fmin (double, double);
  extern float __attribute__((__cdecl__)) fminf (float, float);
  extern long double __attribute__((__cdecl__)) fminl (long double, long double);



  extern double __attribute__((__cdecl__)) fma (double, double, double);
  extern float __attribute__((__cdecl__)) fmaf (float, float, float);
  extern long double __attribute__((__cdecl__)) fmal (long double, long double, long double);
# 1180 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _copysignf (float _Number,float _Sign);
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _chgsignf (float _X);
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _logbf(float _X);
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _nextafterf(float _X,float _Y);
   __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _finitef(float _X);
   __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _isnanf(float _X);
   __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fpclassf(float _X);



   extern long double __attribute__((__cdecl__)) _chgsignl (long double);
# 1580 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/math.h" 3
#pragma pack(pop)
# 12 "include/turtle.h" 2



# 1 "include/glad.h" 1
# 50 "include/glad.h"

# 50 "include/glad.h"
struct gladGLversionStruct {
    int major;
    int minor;
};

typedef void* (* GLADloadproc)(const char *name);
# 83 "include/glad.h"
extern struct gladGLversionStruct GLVersion;

extern int gladLoadGL(void);

extern int gladLoadGLLoader(GLADloadproc);

# 1 "include/khrplatform.h" 1
# 149 "include/khrplatform.h"
# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stdint.h" 1 3 4
# 9 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stdint.h" 3 4
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdint.h" 1 3 4
# 32 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdint.h" 3 4
# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stddef.h" 1 3 4
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stddef.h" 1 3 4
# 18 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stddef.h" 3 4
  
# 18 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stddef.h" 3
 __attribute__ ((__dllimport__)) extern int *__attribute__((__cdecl__)) _errno(void);

  errno_t __attribute__((__cdecl__)) _set_errno(int _Value);
  errno_t __attribute__((__cdecl__)) _get_errno(int *_Value);


  __attribute__ ((__dllimport__)) extern unsigned long __attribute__((__cdecl__)) __threadid(void);

  __attribute__ ((__dllimport__)) extern uintptr_t __attribute__((__cdecl__)) __threadhandle(void);
# 2 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stddef.h" 2 3 4
# 426 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 437 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stddef.h" 3 4
} max_align_t;
# 33 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdint.h" 2 3 4


typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
__extension__ typedef long long int64_t;
__extension__ typedef unsigned long long uint64_t;


typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
typedef int int_least32_t;
typedef unsigned uint_least32_t;
__extension__ typedef long long int_least64_t;
__extension__ typedef unsigned long long uint_least64_t;





typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short int_fast16_t;
typedef unsigned short uint_fast16_t;
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
__extension__ typedef long long int_fast64_t;
__extension__ typedef unsigned long long uint_fast64_t;


__extension__ typedef long long intmax_t;
__extension__ typedef unsigned long long uintmax_t;
# 10 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stdint.h" 2 3 4
# 150 "include/khrplatform.h" 2

# 150 "include/khrplatform.h"
typedef int32_t khronos_int32_t;
typedef uint32_t khronos_uint32_t;
typedef int64_t khronos_int64_t;
typedef uint64_t khronos_uint64_t;
# 242 "include/khrplatform.h"
typedef signed char khronos_int8_t;
typedef unsigned char khronos_uint8_t;
typedef signed short int khronos_int16_t;
typedef unsigned short int khronos_uint16_t;







typedef intptr_t khronos_intptr_t;
typedef uintptr_t khronos_uintptr_t;
# 264 "include/khrplatform.h"
typedef signed long long int khronos_ssize_t;
typedef unsigned long long int khronos_usize_t;
# 275 "include/khrplatform.h"
typedef float khronos_float_t;
# 288 "include/khrplatform.h"
typedef khronos_uint64_t khronos_utime_nanoseconds_t;
typedef khronos_int64_t khronos_stime_nanoseconds_t;
# 305 "include/khrplatform.h"
typedef enum {
    KHRONOS_FALSE = 0,
    KHRONOS_TRUE = 1,
    KHRONOS_BOOLEAN_ENUM_FORCE_SIZE = 0x7FFFFFFF
} khronos_boolean_enum_t;
# 90 "include/glad.h" 2
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef char GLcharARB;



typedef unsigned int GLhandleARB;

typedef khronos_uint16_t GLhalf;
typedef khronos_uint16_t GLhalfARB;
typedef khronos_int32_t GLfixed;
typedef khronos_intptr_t GLintptr;
typedef khronos_intptr_t GLintptrARB;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_ssize_t GLsizeiptrARB;
typedef khronos_int64_t GLint64;
typedef khronos_int64_t GLint64EXT;
typedef khronos_uint64_t GLuint64;
typedef khronos_uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (__attribute__((__stdcall__)) *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (__attribute__((__stdcall__)) *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (__attribute__((__stdcall__)) *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (__attribute__((__stdcall__)) *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
typedef unsigned short GLhalfNV;
typedef GLintptr GLvdpauSurfaceNV;
typedef void (__attribute__((__stdcall__)) *GLVULKANPROCNV)(void);
# 956 "include/glad.h"
extern int GLAD_GL_VERSION_1_0;
typedef void (__attribute__((__stdcall__)) * PFNGLCULLFACEPROC)(GLenum mode);
extern PFNGLCULLFACEPROC glad_glCullFace;

typedef void (__attribute__((__stdcall__)) * PFNGLFRONTFACEPROC)(GLenum mode);
extern PFNGLFRONTFACEPROC glad_glFrontFace;

typedef void (__attribute__((__stdcall__)) * PFNGLHINTPROC)(GLenum target, GLenum mode);
extern PFNGLHINTPROC glad_glHint;

typedef void (__attribute__((__stdcall__)) * PFNGLLINEWIDTHPROC)(GLfloat width);
extern PFNGLLINEWIDTHPROC glad_glLineWidth;

typedef void (__attribute__((__stdcall__)) * PFNGLPOINTSIZEPROC)(GLfloat size);
extern PFNGLPOINTSIZEPROC glad_glPointSize;

typedef void (__attribute__((__stdcall__)) * PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
extern PFNGLPOLYGONMODEPROC glad_glPolygonMode;

typedef void (__attribute__((__stdcall__)) * PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
extern PFNGLSCISSORPROC glad_glScissor;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
extern PFNGLTEXPARAMETERFPROC glad_glTexParameterf;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
extern PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
extern PFNGLTEXPARAMETERIPROC glad_glTexParameteri;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
extern PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXIMAGE1DPROC glad_glTexImage1D;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXIMAGE2DPROC glad_glTexImage2D;

typedef void (__attribute__((__stdcall__)) * PFNGLDRAWBUFFERPROC)(GLenum buf);
extern PFNGLDRAWBUFFERPROC glad_glDrawBuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARPROC)(GLbitfield mask);
extern PFNGLCLEARPROC glad_glClear;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern PFNGLCLEARCOLORPROC glad_glClearColor;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARSTENCILPROC)(GLint s);
extern PFNGLCLEARSTENCILPROC glad_glClearStencil;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARDEPTHPROC)(GLdouble depth);
extern PFNGLCLEARDEPTHPROC glad_glClearDepth;

typedef void (__attribute__((__stdcall__)) * PFNGLSTENCILMASKPROC)(GLuint mask);
extern PFNGLSTENCILMASKPROC glad_glStencilMask;

typedef void (__attribute__((__stdcall__)) * PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern PFNGLCOLORMASKPROC glad_glColorMask;

typedef void (__attribute__((__stdcall__)) * PFNGLDEPTHMASKPROC)(GLboolean flag);
extern PFNGLDEPTHMASKPROC glad_glDepthMask;

typedef void (__attribute__((__stdcall__)) * PFNGLDISABLEPROC)(GLenum cap);
extern PFNGLDISABLEPROC glad_glDisable;

typedef void (__attribute__((__stdcall__)) * PFNGLENABLEPROC)(GLenum cap);
extern PFNGLENABLEPROC glad_glEnable;

typedef void (__attribute__((__stdcall__)) * PFNGLFINISHPROC)(void);
extern PFNGLFINISHPROC glad_glFinish;

typedef void (__attribute__((__stdcall__)) * PFNGLFLUSHPROC)(void);
extern PFNGLFLUSHPROC glad_glFlush;

typedef void (__attribute__((__stdcall__)) * PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
extern PFNGLBLENDFUNCPROC glad_glBlendFunc;

typedef void (__attribute__((__stdcall__)) * PFNGLLOGICOPPROC)(GLenum opcode);
extern PFNGLLOGICOPPROC glad_glLogicOp;

typedef void (__attribute__((__stdcall__)) * PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
extern PFNGLSTENCILFUNCPROC glad_glStencilFunc;

typedef void (__attribute__((__stdcall__)) * PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
extern PFNGLSTENCILOPPROC glad_glStencilOp;

typedef void (__attribute__((__stdcall__)) * PFNGLDEPTHFUNCPROC)(GLenum func);
extern PFNGLDEPTHFUNCPROC glad_glDepthFunc;

typedef void (__attribute__((__stdcall__)) * PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
extern PFNGLPIXELSTOREFPROC glad_glPixelStoref;

typedef void (__attribute__((__stdcall__)) * PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
extern PFNGLPIXELSTOREIPROC glad_glPixelStorei;

typedef void (__attribute__((__stdcall__)) * PFNGLREADBUFFERPROC)(GLenum src);
extern PFNGLREADBUFFERPROC glad_glReadBuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
extern PFNGLREADPIXELSPROC glad_glReadPixels;

typedef void (__attribute__((__stdcall__)) * PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean *data);
extern PFNGLGETBOOLEANVPROC glad_glGetBooleanv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble *data);
extern PFNGLGETDOUBLEVPROC glad_glGetDoublev;

typedef GLenum (__attribute__((__stdcall__)) * PFNGLGETERRORPROC)(void);
extern PFNGLGETERRORPROC glad_glGetError;

typedef void (__attribute__((__stdcall__)) * PFNGLGETFLOATVPROC)(GLenum pname, GLfloat *data);
extern PFNGLGETFLOATVPROC glad_glGetFloatv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
extern PFNGLGETINTEGERVPROC glad_glGetIntegerv;

typedef const GLubyte * (__attribute__((__stdcall__)) * PFNGLGETSTRINGPROC)(GLenum name);
extern PFNGLGETSTRINGPROC glad_glGetString;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
extern PFNGLGETTEXIMAGEPROC glad_glGetTexImage;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
extern PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
extern PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat *params);
extern PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
extern PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISENABLEDPROC)(GLenum cap);
extern PFNGLISENABLEDPROC glad_glIsEnabled;

typedef void (__attribute__((__stdcall__)) * PFNGLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
extern PFNGLDEPTHRANGEPROC glad_glDepthRange;

typedef void (__attribute__((__stdcall__)) * PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
extern PFNGLVIEWPORTPROC glad_glViewport;




extern int GLAD_GL_VERSION_1_1;
typedef void (__attribute__((__stdcall__)) * PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
extern PFNGLDRAWARRAYSPROC glad_glDrawArrays;

typedef void (__attribute__((__stdcall__)) * PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
extern PFNGLDRAWELEMENTSPROC glad_glDrawElements;

typedef void (__attribute__((__stdcall__)) * PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
extern PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset;

typedef void (__attribute__((__stdcall__)) * PFNGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
extern PFNGLBINDTEXTUREPROC glad_glBindTexture;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
extern PFNGLDELETETEXTURESPROC glad_glDeleteTextures;

typedef void (__attribute__((__stdcall__)) * PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
extern PFNGLGENTEXTURESPROC glad_glGenTextures;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISTEXTUREPROC)(GLuint texture);
extern PFNGLISTEXTUREPROC glad_glIsTexture;




extern int GLAD_GL_VERSION_1_2;
typedef void (__attribute__((__stdcall__)) * PFNGLDRAWRANGEELEMENTSPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
extern PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXIMAGE3DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXIMAGE3DPROC glad_glTexImage3D;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D;




extern int GLAD_GL_VERSION_1_3;
typedef void (__attribute__((__stdcall__)) * PFNGLACTIVETEXTUREPROC)(GLenum texture);
extern PFNGLACTIVETEXTUREPROC glad_glActiveTexture;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLECOVERAGEPROC)(GLfloat value, GLboolean invert);
extern PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage;

typedef void (__attribute__((__stdcall__)) * PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOMPRESSEDTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D;

typedef void (__attribute__((__stdcall__)) * PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D;

typedef void (__attribute__((__stdcall__)) * PFNGLGETCOMPRESSEDTEXIMAGEPROC)(GLenum target, GLint level, void *img);
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage;




extern int GLAD_GL_VERSION_1_4;
typedef void (__attribute__((__stdcall__)) * PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTIDRAWARRAYSPROC)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
extern PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTIDRAWELEMENTSPROC)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
extern PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements;

typedef void (__attribute__((__stdcall__)) * PFNGLPOINTPARAMETERFPROC)(GLenum pname, GLfloat param);
extern PFNGLPOINTPARAMETERFPROC glad_glPointParameterf;

typedef void (__attribute__((__stdcall__)) * PFNGLPOINTPARAMETERFVPROC)(GLenum pname, const GLfloat *params);
extern PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv;

typedef void (__attribute__((__stdcall__)) * PFNGLPOINTPARAMETERIPROC)(GLenum pname, GLint param);
extern PFNGLPOINTPARAMETERIPROC glad_glPointParameteri;

typedef void (__attribute__((__stdcall__)) * PFNGLPOINTPARAMETERIVPROC)(GLenum pname, const GLint *params);
extern PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv;

typedef void (__attribute__((__stdcall__)) * PFNGLBLENDCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern PFNGLBLENDCOLORPROC glad_glBlendColor;

typedef void (__attribute__((__stdcall__)) * PFNGLBLENDEQUATIONPROC)(GLenum mode);
extern PFNGLBLENDEQUATIONPROC glad_glBlendEquation;




extern int GLAD_GL_VERSION_1_5;
typedef void (__attribute__((__stdcall__)) * PFNGLGENQUERIESPROC)(GLsizei n, GLuint *ids);
extern PFNGLGENQUERIESPROC glad_glGenQueries;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETEQUERIESPROC)(GLsizei n, const GLuint *ids);
extern PFNGLDELETEQUERIESPROC glad_glDeleteQueries;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISQUERYPROC)(GLuint id);
extern PFNGLISQUERYPROC glad_glIsQuery;

typedef void (__attribute__((__stdcall__)) * PFNGLBEGINQUERYPROC)(GLenum target, GLuint id);
extern PFNGLBEGINQUERYPROC glad_glBeginQuery;

typedef void (__attribute__((__stdcall__)) * PFNGLENDQUERYPROC)(GLenum target);
extern PFNGLENDQUERYPROC glad_glEndQuery;

typedef void (__attribute__((__stdcall__)) * PFNGLGETQUERYIVPROC)(GLenum target, GLenum pname, GLint *params);
extern PFNGLGETQUERYIVPROC glad_glGetQueryiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETQUERYOBJECTIVPROC)(GLuint id, GLenum pname, GLint *params);
extern PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETQUERYOBJECTUIVPROC)(GLuint id, GLenum pname, GLuint *params);
extern PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
extern PFNGLBINDBUFFERPROC glad_glBindBuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
extern PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;

typedef void (__attribute__((__stdcall__)) * PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
extern PFNGLGENBUFFERSPROC glad_glGenBuffers;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISBUFFERPROC)(GLuint buffer);
extern PFNGLISBUFFERPROC glad_glIsBuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern PFNGLBUFFERDATAPROC glad_glBufferData;

typedef void (__attribute__((__stdcall__)) * PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
extern PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;

typedef void (__attribute__((__stdcall__)) * PFNGLGETBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, void *data);
extern PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData;

typedef void * (__attribute__((__stdcall__)) * PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
extern PFNGLMAPBUFFERPROC glad_glMapBuffer;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLUNMAPBUFFERPROC)(GLenum target);
extern PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
extern PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETBUFFERPOINTERVPROC)(GLenum target, GLenum pname, void **params);
extern PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv;




extern int GLAD_GL_VERSION_2_0;
typedef void (__attribute__((__stdcall__)) * PFNGLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeAlpha);
extern PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate;

typedef void (__attribute__((__stdcall__)) * PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum *bufs);
extern PFNGLDRAWBUFFERSPROC glad_glDrawBuffers;

typedef void (__attribute__((__stdcall__)) * PFNGLSTENCILOPSEPARATEPROC)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate;

typedef void (__attribute__((__stdcall__)) * PFNGLSTENCILFUNCSEPARATEPROC)(GLenum face, GLenum func, GLint ref, GLuint mask);
extern PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate;

typedef void (__attribute__((__stdcall__)) * PFNGLSTENCILMASKSEPARATEPROC)(GLenum face, GLuint mask);
extern PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate;

typedef void (__attribute__((__stdcall__)) * PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
extern PFNGLATTACHSHADERPROC glad_glAttachShader;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const GLchar *name);
extern PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation;

typedef void (__attribute__((__stdcall__)) * PFNGLCOMPILESHADERPROC)(GLuint shader);
extern PFNGLCOMPILESHADERPROC glad_glCompileShader;

typedef GLuint (__attribute__((__stdcall__)) * PFNGLCREATEPROGRAMPROC)(void);
extern PFNGLCREATEPROGRAMPROC glad_glCreateProgram;

typedef GLuint (__attribute__((__stdcall__)) * PFNGLCREATESHADERPROC)(GLenum type);
extern PFNGLCREATESHADERPROC glad_glCreateShader;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETEPROGRAMPROC)(GLuint program);
extern PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETESHADERPROC)(GLuint shader);
extern PFNGLDELETESHADERPROC glad_glDeleteShader;

typedef void (__attribute__((__stdcall__)) * PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
extern PFNGLDETACHSHADERPROC glad_glDetachShader;

typedef void (__attribute__((__stdcall__)) * PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;

typedef void (__attribute__((__stdcall__)) * PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;

typedef void (__attribute__((__stdcall__)) * PFNGLGETACTIVEATTRIBPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib;

typedef void (__attribute__((__stdcall__)) * PFNGLGETACTIVEUNIFORMPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform;

typedef void (__attribute__((__stdcall__)) * PFNGLGETATTACHEDSHADERSPROC)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
extern PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders;

typedef GLint (__attribute__((__stdcall__)) * PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar *name);
extern PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation;

typedef void (__attribute__((__stdcall__)) * PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
extern PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
extern PFNGLGETSHADERIVPROC glad_glGetShaderiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSHADERSOURCEPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
extern PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource;

typedef GLint (__attribute__((__stdcall__)) * PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
extern PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;

typedef void (__attribute__((__stdcall__)) * PFNGLGETUNIFORMFVPROC)(GLuint program, GLint location, GLfloat *params);
extern PFNGLGETUNIFORMFVPROC glad_glGetUniformfv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETUNIFORMIVPROC)(GLuint program, GLint location, GLint *params);
extern PFNGLGETUNIFORMIVPROC glad_glGetUniformiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETVERTEXATTRIBDVPROC)(GLuint index, GLenum pname, GLdouble *params);
extern PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETVERTEXATTRIBFVPROC)(GLuint index, GLenum pname, GLfloat *params);
extern PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETVERTEXATTRIBIVPROC)(GLuint index, GLenum pname, GLint *params);
extern PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETVERTEXATTRIBPOINTERVPROC)(GLuint index, GLenum pname, void **pointer);
extern PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISPROGRAMPROC)(GLuint program);
extern PFNGLISPROGRAMPROC glad_glIsProgram;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISSHADERPROC)(GLuint shader);
extern PFNGLISSHADERPROC glad_glIsShader;

typedef void (__attribute__((__stdcall__)) * PFNGLLINKPROGRAMPROC)(GLuint program);
extern PFNGLLINKPROGRAMPROC glad_glLinkProgram;

typedef void (__attribute__((__stdcall__)) * PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
extern PFNGLSHADERSOURCEPROC glad_glShaderSource;

typedef void (__attribute__((__stdcall__)) * PFNGLUSEPROGRAMPROC)(GLuint program);
extern PFNGLUSEPROGRAMPROC glad_glUseProgram;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
extern PFNGLUNIFORM1FPROC glad_glUniform1f;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
extern PFNGLUNIFORM2FPROC glad_glUniform2f;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern PFNGLUNIFORM3FPROC glad_glUniform3f;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern PFNGLUNIFORM4FPROC glad_glUniform4f;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
extern PFNGLUNIFORM1IPROC glad_glUniform1i;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM2IPROC)(GLint location, GLint v0, GLint v1);
extern PFNGLUNIFORM2IPROC glad_glUniform2i;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM3IPROC)(GLint location, GLint v0, GLint v1, GLint v2);
extern PFNGLUNIFORM3IPROC glad_glUniform3i;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM4IPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern PFNGLUNIFORM4IPROC glad_glUniform4i;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat *value);
extern PFNGLUNIFORM1FVPROC glad_glUniform1fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat *value);
extern PFNGLUNIFORM2FVPROC glad_glUniform2fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat *value);
extern PFNGLUNIFORM3FVPROC glad_glUniform3fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat *value);
extern PFNGLUNIFORM4FVPROC glad_glUniform4fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint *value);
extern PFNGLUNIFORM1IVPROC glad_glUniform1iv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint *value);
extern PFNGLUNIFORM2IVPROC glad_glUniform2iv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint *value);
extern PFNGLUNIFORM3IVPROC glad_glUniform3iv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint *value);
extern PFNGLUNIFORM4IVPROC glad_glUniform4iv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;

typedef void (__attribute__((__stdcall__)) * PFNGLVALIDATEPROGRAMPROC)(GLuint program);
extern PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB1DPROC)(GLuint index, GLdouble x);
extern PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB1DVPROC)(GLuint index, const GLdouble *v);
extern PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB1FPROC)(GLuint index, GLfloat x);
extern PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB1FVPROC)(GLuint index, const GLfloat *v);
extern PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB1SPROC)(GLuint index, GLshort x);
extern PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB1SVPROC)(GLuint index, const GLshort *v);
extern PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB2DPROC)(GLuint index, GLdouble x, GLdouble y);
extern PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB2DVPROC)(GLuint index, const GLdouble *v);
extern PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB2FPROC)(GLuint index, GLfloat x, GLfloat y);
extern PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB2FVPROC)(GLuint index, const GLfloat *v);
extern PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB2SPROC)(GLuint index, GLshort x, GLshort y);
extern PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB2SVPROC)(GLuint index, const GLshort *v);
extern PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB3DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB3DVPROC)(GLuint index, const GLdouble *v);
extern PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB3FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB3FVPROC)(GLuint index, const GLfloat *v);
extern PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB3SPROC)(GLuint index, GLshort x, GLshort y, GLshort z);
extern PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB3SVPROC)(GLuint index, const GLshort *v);
extern PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4NBVPROC)(GLuint index, const GLbyte *v);
extern PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4NIVPROC)(GLuint index, const GLint *v);
extern PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4NSVPROC)(GLuint index, const GLshort *v);
extern PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4NUBPROC)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4NUBVPROC)(GLuint index, const GLubyte *v);
extern PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4NUIVPROC)(GLuint index, const GLuint *v);
extern PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4NUSVPROC)(GLuint index, const GLushort *v);
extern PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4BVPROC)(GLuint index, const GLbyte *v);
extern PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4DVPROC)(GLuint index, const GLdouble *v);
extern PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4FVPROC)(GLuint index, const GLfloat *v);
extern PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4IVPROC)(GLuint index, const GLint *v);
extern PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4SPROC)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4SVPROC)(GLuint index, const GLshort *v);
extern PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4UBVPROC)(GLuint index, const GLubyte *v);
extern PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4UIVPROC)(GLuint index, const GLuint *v);
extern PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIB4USVPROC)(GLuint index, const GLushort *v);
extern PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;




extern int GLAD_GL_VERSION_2_1;
typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX2X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX3X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX2X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX4X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX3X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMMATRIX4X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv;




extern int GLAD_GL_VERSION_3_0;
typedef void (__attribute__((__stdcall__)) * PFNGLCOLORMASKIPROC)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern PFNGLCOLORMASKIPROC glad_glColorMaski;

typedef void (__attribute__((__stdcall__)) * PFNGLGETBOOLEANI_VPROC)(GLenum target, GLuint index, GLboolean *data);
extern PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v;

typedef void (__attribute__((__stdcall__)) * PFNGLGETINTEGERI_VPROC)(GLenum target, GLuint index, GLint *data);
extern PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v;

typedef void (__attribute__((__stdcall__)) * PFNGLENABLEIPROC)(GLenum target, GLuint index);
extern PFNGLENABLEIPROC glad_glEnablei;

typedef void (__attribute__((__stdcall__)) * PFNGLDISABLEIPROC)(GLenum target, GLuint index);
extern PFNGLDISABLEIPROC glad_glDisablei;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISENABLEDIPROC)(GLenum target, GLuint index);
extern PFNGLISENABLEDIPROC glad_glIsEnabledi;

typedef void (__attribute__((__stdcall__)) * PFNGLBEGINTRANSFORMFEEDBACKPROC)(GLenum primitiveMode);
extern PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback;

typedef void (__attribute__((__stdcall__)) * PFNGLENDTRANSFORMFEEDBACKPROC)(void);
extern PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDBUFFERRANGEPROC)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
extern PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase;

typedef void (__attribute__((__stdcall__)) * PFNGLTRANSFORMFEEDBACKVARYINGSPROC)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying;

typedef void (__attribute__((__stdcall__)) * PFNGLCLAMPCOLORPROC)(GLenum target, GLenum clamp);
extern PFNGLCLAMPCOLORPROC glad_glClampColor;

typedef void (__attribute__((__stdcall__)) * PFNGLBEGINCONDITIONALRENDERPROC)(GLuint id, GLenum mode);
extern PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender;

typedef void (__attribute__((__stdcall__)) * PFNGLENDCONDITIONALRENDERPROC)(void);
extern PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
extern PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer;

typedef void (__attribute__((__stdcall__)) * PFNGLGETVERTEXATTRIBIIVPROC)(GLuint index, GLenum pname, GLint *params);
extern PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETVERTEXATTRIBIUIVPROC)(GLuint index, GLenum pname, GLuint *params);
extern PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI1IPROC)(GLuint index, GLint x);
extern PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI2IPROC)(GLuint index, GLint x, GLint y);
extern PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI3IPROC)(GLuint index, GLint x, GLint y, GLint z);
extern PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4IPROC)(GLuint index, GLint x, GLint y, GLint z, GLint w);
extern PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI1UIPROC)(GLuint index, GLuint x);
extern PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI2UIPROC)(GLuint index, GLuint x, GLuint y);
extern PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI3UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z);
extern PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI1IVPROC)(GLuint index, const GLint *v);
extern PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI2IVPROC)(GLuint index, const GLint *v);
extern PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI3IVPROC)(GLuint index, const GLint *v);
extern PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4IVPROC)(GLuint index, const GLint *v);
extern PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI1UIVPROC)(GLuint index, const GLuint *v);
extern PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI2UIVPROC)(GLuint index, const GLuint *v);
extern PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI3UIVPROC)(GLuint index, const GLuint *v);
extern PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4UIVPROC)(GLuint index, const GLuint *v);
extern PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4BVPROC)(GLuint index, const GLbyte *v);
extern PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4SVPROC)(GLuint index, const GLshort *v);
extern PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4UBVPROC)(GLuint index, const GLubyte *v);
extern PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBI4USVPROC)(GLuint index, const GLushort *v);
extern PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETUNIFORMUIVPROC)(GLuint program, GLint location, GLuint *params);
extern PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDFRAGDATALOCATIONPROC)(GLuint program, GLuint color, const GLchar *name);
extern PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation;

typedef GLint (__attribute__((__stdcall__)) * PFNGLGETFRAGDATALOCATIONPROC)(GLuint program, const GLchar *name);
extern PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM1UIPROC)(GLint location, GLuint v0);
extern PFNGLUNIFORM1UIPROC glad_glUniform1ui;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM2UIPROC)(GLint location, GLuint v0, GLuint v1);
extern PFNGLUNIFORM2UIPROC glad_glUniform2ui;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM3UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2);
extern PFNGLUNIFORM3UIPROC glad_glUniform3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM4UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern PFNGLUNIFORM4UIPROC glad_glUniform4ui;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM1UIVPROC)(GLint location, GLsizei count, const GLuint *value);
extern PFNGLUNIFORM1UIVPROC glad_glUniform1uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM2UIVPROC)(GLint location, GLsizei count, const GLuint *value);
extern PFNGLUNIFORM2UIVPROC glad_glUniform2uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM3UIVPROC)(GLint location, GLsizei count, const GLuint *value);
extern PFNGLUNIFORM3UIVPROC glad_glUniform3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORM4UIVPROC)(GLint location, GLsizei count, const GLuint *value);
extern PFNGLUNIFORM4UIVPROC glad_glUniform4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, const GLint *params);
extern PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, const GLuint *params);
extern PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, GLint *params);
extern PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, GLuint *params);
extern PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARBUFFERIVPROC)(GLenum buffer, GLint drawbuffer, const GLint *value);
extern PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARBUFFERUIVPROC)(GLenum buffer, GLint drawbuffer, const GLuint *value);
extern PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARBUFFERFVPROC)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
extern PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv;

typedef void (__attribute__((__stdcall__)) * PFNGLCLEARBUFFERFIPROC)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
extern PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi;

typedef const GLubyte * (__attribute__((__stdcall__)) * PFNGLGETSTRINGIPROC)(GLenum name, GLuint index);
extern PFNGLGETSTRINGIPROC glad_glGetStringi;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISRENDERBUFFERPROC)(GLuint renderbuffer);
extern PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
extern PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint *renderbuffers);
extern PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers;

typedef void (__attribute__((__stdcall__)) * PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint *renderbuffers);
extern PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers;

typedef void (__attribute__((__stdcall__)) * PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage;

typedef void (__attribute__((__stdcall__)) * PFNGLGETRENDERBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISFRAMEBUFFERPROC)(GLuint framebuffer);
extern PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
extern PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint *framebuffers);
extern PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;

typedef void (__attribute__((__stdcall__)) * PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint *framebuffers);
extern PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;

typedef GLenum (__attribute__((__stdcall__)) * PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;

typedef void (__attribute__((__stdcall__)) * PFNGLFRAMEBUFFERTEXTURE1DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D;

typedef void (__attribute__((__stdcall__)) * PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;

typedef void (__attribute__((__stdcall__)) * PFNGLFRAMEBUFFERTEXTURE3DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D;

typedef void (__attribute__((__stdcall__)) * PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv;

typedef void (__attribute__((__stdcall__)) * PFNGLGENERATEMIPMAPPROC)(GLenum target);
extern PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;

typedef void (__attribute__((__stdcall__)) * PFNGLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample;

typedef void (__attribute__((__stdcall__)) * PFNGLFRAMEBUFFERTEXTURELAYERPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer;

typedef void * (__attribute__((__stdcall__)) * PFNGLMAPBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange;

typedef void (__attribute__((__stdcall__)) * PFNGLFLUSHMAPPEDBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length);
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDVERTEXARRAYPROC)(GLuint array);
extern PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
extern PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;

typedef void (__attribute__((__stdcall__)) * PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
extern PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISVERTEXARRAYPROC)(GLuint array);
extern PFNGLISVERTEXARRAYPROC glad_glIsVertexArray;




extern int GLAD_GL_VERSION_3_1;
typedef void (__attribute__((__stdcall__)) * PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
extern PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced;

typedef void (__attribute__((__stdcall__)) * PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
extern PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXBUFFERPROC)(GLenum target, GLenum internalformat, GLuint buffer);
extern PFNGLTEXBUFFERPROC glad_glTexBuffer;

typedef void (__attribute__((__stdcall__)) * PFNGLPRIMITIVERESTARTINDEXPROC)(GLuint index);
extern PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex;

typedef void (__attribute__((__stdcall__)) * PFNGLCOPYBUFFERSUBDATAPROC)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
extern PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData;

typedef void (__attribute__((__stdcall__)) * PFNGLGETUNIFORMINDICESPROC)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
extern PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices;

typedef void (__attribute__((__stdcall__)) * PFNGLGETACTIVEUNIFORMSIVPROC)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
extern PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETACTIVEUNIFORMNAMEPROC)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
extern PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName;

typedef GLuint (__attribute__((__stdcall__)) * PFNGLGETUNIFORMBLOCKINDEXPROC)(GLuint program, const GLchar *uniformBlockName);
extern PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex;

typedef void (__attribute__((__stdcall__)) * PFNGLGETACTIVEUNIFORMBLOCKIVPROC)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName;

typedef void (__attribute__((__stdcall__)) * PFNGLUNIFORMBLOCKBINDINGPROC)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
extern PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding;




extern int GLAD_GL_VERSION_3_2;
typedef void (__attribute__((__stdcall__)) * PFNGLDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
extern PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex;

typedef void (__attribute__((__stdcall__)) * PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex;

typedef void (__attribute__((__stdcall__)) * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex;

typedef void (__attribute__((__stdcall__)) * PFNGLPROVOKINGVERTEXPROC)(GLenum mode);
extern PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex;

typedef GLsync (__attribute__((__stdcall__)) * PFNGLFENCESYNCPROC)(GLenum condition, GLbitfield flags);
extern PFNGLFENCESYNCPROC glad_glFenceSync;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISSYNCPROC)(GLsync sync);
extern PFNGLISSYNCPROC glad_glIsSync;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETESYNCPROC)(GLsync sync);
extern PFNGLDELETESYNCPROC glad_glDeleteSync;

typedef GLenum (__attribute__((__stdcall__)) * PFNGLCLIENTWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync;

typedef void (__attribute__((__stdcall__)) * PFNGLWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern PFNGLWAITSYNCPROC glad_glWaitSync;

typedef void (__attribute__((__stdcall__)) * PFNGLGETINTEGER64VPROC)(GLenum pname, GLint64 *data);
extern PFNGLGETINTEGER64VPROC glad_glGetInteger64v;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSYNCIVPROC)(GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values);
extern PFNGLGETSYNCIVPROC glad_glGetSynciv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETINTEGER64I_VPROC)(GLenum target, GLuint index, GLint64 *data);
extern PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v;

typedef void (__attribute__((__stdcall__)) * PFNGLGETBUFFERPARAMETERI64VPROC)(GLenum target, GLenum pname, GLint64 *params);
extern PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v;

typedef void (__attribute__((__stdcall__)) * PFNGLFRAMEBUFFERTEXTUREPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXIMAGE2DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXIMAGE3DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample;

typedef void (__attribute__((__stdcall__)) * PFNGLGETMULTISAMPLEFVPROC)(GLenum pname, GLuint index, GLfloat *val);
extern PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLEMASKIPROC)(GLuint maskNumber, GLbitfield mask);
extern PFNGLSAMPLEMASKIPROC glad_glSampleMaski;




extern int GLAD_GL_VERSION_3_3;
typedef void (__attribute__((__stdcall__)) * PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glad_glBindFragDataLocationIndexed;

typedef GLint (__attribute__((__stdcall__)) * PFNGLGETFRAGDATAINDEXPROC)(GLuint program, const GLchar *name);
extern PFNGLGETFRAGDATAINDEXPROC glad_glGetFragDataIndex;

typedef void (__attribute__((__stdcall__)) * PFNGLGENSAMPLERSPROC)(GLsizei count, GLuint *samplers);
extern PFNGLGENSAMPLERSPROC glad_glGenSamplers;

typedef void (__attribute__((__stdcall__)) * PFNGLDELETESAMPLERSPROC)(GLsizei count, const GLuint *samplers);
extern PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers;

typedef GLboolean (__attribute__((__stdcall__)) * PFNGLISSAMPLERPROC)(GLuint sampler);
extern PFNGLISSAMPLERPROC glad_glIsSampler;

typedef void (__attribute__((__stdcall__)) * PFNGLBINDSAMPLERPROC)(GLuint unit, GLuint sampler);
extern PFNGLBINDSAMPLERPROC glad_glBindSampler;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLERPARAMETERIPROC)(GLuint sampler, GLenum pname, GLint param);
extern PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, const GLint *param);
extern PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLERPARAMETERFPROC)(GLuint sampler, GLenum pname, GLfloat param);
extern PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, const GLfloat *param);
extern PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, const GLint *param);
extern PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv;

typedef void (__attribute__((__stdcall__)) * PFNGLSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, const GLuint *param);
extern PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, GLint *params);
extern PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, GLint *params);
extern PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, GLfloat *params);
extern PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv;

typedef void (__attribute__((__stdcall__)) * PFNGLGETSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, GLuint *params);
extern PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv;

typedef void (__attribute__((__stdcall__)) * PFNGLQUERYCOUNTERPROC)(GLuint id, GLenum target);
extern PFNGLQUERYCOUNTERPROC glad_glQueryCounter;

typedef void (__attribute__((__stdcall__)) * PFNGLGETQUERYOBJECTI64VPROC)(GLuint id, GLenum pname, GLint64 *params);
extern PFNGLGETQUERYOBJECTI64VPROC glad_glGetQueryObjecti64v;

typedef void (__attribute__((__stdcall__)) * PFNGLGETQUERYOBJECTUI64VPROC)(GLuint id, GLenum pname, GLuint64 *params);
extern PFNGLGETQUERYOBJECTUI64VPROC glad_glGetQueryObjectui64v;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
extern PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP1UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFNGLVERTEXATTRIBP1UIPROC glad_glVertexAttribP1ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP1UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern PFNGLVERTEXATTRIBP1UIVPROC glad_glVertexAttribP1uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP2UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFNGLVERTEXATTRIBP2UIPROC glad_glVertexAttribP2ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP2UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern PFNGLVERTEXATTRIBP2UIVPROC glad_glVertexAttribP2uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP3UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFNGLVERTEXATTRIBP3UIPROC glad_glVertexAttribP3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP3UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern PFNGLVERTEXATTRIBP3UIVPROC glad_glVertexAttribP3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP4UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern PFNGLVERTEXATTRIBP4UIPROC glad_glVertexAttribP4ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXATTRIBP4UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern PFNGLVERTEXATTRIBP4UIVPROC glad_glVertexAttribP4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXP2UIPROC)(GLenum type, GLuint value);
extern PFNGLVERTEXP2UIPROC glad_glVertexP2ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXP2UIVPROC)(GLenum type, const GLuint *value);
extern PFNGLVERTEXP2UIVPROC glad_glVertexP2uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXP3UIPROC)(GLenum type, GLuint value);
extern PFNGLVERTEXP3UIPROC glad_glVertexP3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXP3UIVPROC)(GLenum type, const GLuint *value);
extern PFNGLVERTEXP3UIVPROC glad_glVertexP3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXP4UIPROC)(GLenum type, GLuint value);
extern PFNGLVERTEXP4UIPROC glad_glVertexP4ui;

typedef void (__attribute__((__stdcall__)) * PFNGLVERTEXP4UIVPROC)(GLenum type, const GLuint *value);
extern PFNGLVERTEXP4UIVPROC glad_glVertexP4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP1UIPROC)(GLenum type, GLuint coords);
extern PFNGLTEXCOORDP1UIPROC glad_glTexCoordP1ui;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP1UIVPROC)(GLenum type, const GLuint *coords);
extern PFNGLTEXCOORDP1UIVPROC glad_glTexCoordP1uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP2UIPROC)(GLenum type, GLuint coords);
extern PFNGLTEXCOORDP2UIPROC glad_glTexCoordP2ui;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP2UIVPROC)(GLenum type, const GLuint *coords);
extern PFNGLTEXCOORDP2UIVPROC glad_glTexCoordP2uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP3UIPROC)(GLenum type, GLuint coords);
extern PFNGLTEXCOORDP3UIPROC glad_glTexCoordP3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP3UIVPROC)(GLenum type, const GLuint *coords);
extern PFNGLTEXCOORDP3UIVPROC glad_glTexCoordP3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP4UIPROC)(GLenum type, GLuint coords);
extern PFNGLTEXCOORDP4UIPROC glad_glTexCoordP4ui;

typedef void (__attribute__((__stdcall__)) * PFNGLTEXCOORDP4UIVPROC)(GLenum type, const GLuint *coords);
extern PFNGLTEXCOORDP4UIVPROC glad_glTexCoordP4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP1UIPROC)(GLenum texture, GLenum type, GLuint coords);
extern PFNGLMULTITEXCOORDP1UIPROC glad_glMultiTexCoordP1ui;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP1UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
extern PFNGLMULTITEXCOORDP1UIVPROC glad_glMultiTexCoordP1uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP2UIPROC)(GLenum texture, GLenum type, GLuint coords);
extern PFNGLMULTITEXCOORDP2UIPROC glad_glMultiTexCoordP2ui;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP2UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
extern PFNGLMULTITEXCOORDP2UIVPROC glad_glMultiTexCoordP2uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP3UIPROC)(GLenum texture, GLenum type, GLuint coords);
extern PFNGLMULTITEXCOORDP3UIPROC glad_glMultiTexCoordP3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP3UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
extern PFNGLMULTITEXCOORDP3UIVPROC glad_glMultiTexCoordP3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP4UIPROC)(GLenum texture, GLenum type, GLuint coords);
extern PFNGLMULTITEXCOORDP4UIPROC glad_glMultiTexCoordP4ui;

typedef void (__attribute__((__stdcall__)) * PFNGLMULTITEXCOORDP4UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
extern PFNGLMULTITEXCOORDP4UIVPROC glad_glMultiTexCoordP4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLNORMALP3UIPROC)(GLenum type, GLuint coords);
extern PFNGLNORMALP3UIPROC glad_glNormalP3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLNORMALP3UIVPROC)(GLenum type, const GLuint *coords);
extern PFNGLNORMALP3UIVPROC glad_glNormalP3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLCOLORP3UIPROC)(GLenum type, GLuint color);
extern PFNGLCOLORP3UIPROC glad_glColorP3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLCOLORP3UIVPROC)(GLenum type, const GLuint *color);
extern PFNGLCOLORP3UIVPROC glad_glColorP3uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLCOLORP4UIPROC)(GLenum type, GLuint color);
extern PFNGLCOLORP4UIPROC glad_glColorP4ui;

typedef void (__attribute__((__stdcall__)) * PFNGLCOLORP4UIVPROC)(GLenum type, const GLuint *color);
extern PFNGLCOLORP4UIVPROC glad_glColorP4uiv;

typedef void (__attribute__((__stdcall__)) * PFNGLSECONDARYCOLORP3UIPROC)(GLenum type, GLuint color);
extern PFNGLSECONDARYCOLORP3UIPROC glad_glSecondaryColorP3ui;

typedef void (__attribute__((__stdcall__)) * PFNGLSECONDARYCOLORP3UIVPROC)(GLenum type, const GLuint *color);
extern PFNGLSECONDARYCOLORP3UIVPROC glad_glSecondaryColorP3uiv;
# 16 "include/turtle.h" 2
# 1 "include/glfw3.h" 1
# 103 "include/glfw3.h"
# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stddef.h" 1 3 4
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stddef.h" 1 3 4
# 2 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stddef.h" 2 3 4
# 104 "include/glfw3.h" 2
# 1147 "include/glfw3.h"
typedef void (*GLFWglproc)(void);
# 1161 "include/glfw3.h"
typedef void (*GLFWvkproc)(void);
# 1173 "include/glfw3.h"
typedef struct GLFWmonitor GLFWmonitor;
# 1185 "include/glfw3.h"
typedef struct GLFWwindow GLFWwindow;
# 1197 "include/glfw3.h"
typedef struct GLFWcursor GLFWcursor;
# 1221 "include/glfw3.h"
typedef void (* GLFWerrorfun)(int error_code, const char* description);
# 1244 "include/glfw3.h"
typedef void (* GLFWwindowposfun)(GLFWwindow* window, int xpos, int ypos);
# 1266 "include/glfw3.h"
typedef void (* GLFWwindowsizefun)(GLFWwindow* window, int width, int height);
# 1286 "include/glfw3.h"
typedef void (* GLFWwindowclosefun)(GLFWwindow* window);
# 1306 "include/glfw3.h"
typedef void (* GLFWwindowrefreshfun)(GLFWwindow* window);
# 1327 "include/glfw3.h"
typedef void (* GLFWwindowfocusfun)(GLFWwindow* window, int focused);
# 1348 "include/glfw3.h"
typedef void (* GLFWwindowiconifyfun)(GLFWwindow* window, int iconified);
# 1369 "include/glfw3.h"
typedef void (* GLFWwindowmaximizefun)(GLFWwindow* window, int maximized);
# 1390 "include/glfw3.h"
typedef void (* GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);
# 1411 "include/glfw3.h"
typedef void (* GLFWwindowcontentscalefun)(GLFWwindow* window, float xscale, float yscale);
# 1437 "include/glfw3.h"
typedef void (* GLFWmousebuttonfun)(GLFWwindow* window, int button, int action, int mods);
# 1460 "include/glfw3.h"
typedef void (* GLFWcursorposfun)(GLFWwindow* window, double xpos, double ypos);
# 1481 "include/glfw3.h"
typedef void (* GLFWcursorenterfun)(GLFWwindow* window, int entered);
# 1502 "include/glfw3.h"
typedef void (* GLFWscrollfun)(GLFWwindow* window, double xoffset, double yoffset);
# 1528 "include/glfw3.h"
typedef void (* GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
# 1549 "include/glfw3.h"
typedef void (* GLFWcharfun)(GLFWwindow* window, unsigned int codepoint);
# 1576 "include/glfw3.h"
typedef void (* GLFWcharmodsfun)(GLFWwindow* window, unsigned int codepoint, int mods);
# 1600 "include/glfw3.h"
typedef void (* GLFWdropfun)(GLFWwindow* window, int path_count, const char* paths[]);
# 1621 "include/glfw3.h"
typedef void (* GLFWmonitorfun)(GLFWmonitor* monitor, int event);
# 1642 "include/glfw3.h"
typedef void (* GLFWjoystickfun)(int jid, int event);
# 1657 "include/glfw3.h"
typedef struct GLFWvidmode
{


    int width;


    int height;


    int redBits;


    int greenBits;


    int blueBits;


    int refreshRate;
} GLFWvidmode;
# 1691 "include/glfw3.h"
typedef struct GLFWgammaramp
{


    unsigned short* red;


    unsigned short* green;


    unsigned short* blue;


    unsigned int size;
} GLFWgammaramp;
# 1720 "include/glfw3.h"
typedef struct GLFWimage
{


    int width;


    int height;


    unsigned char* pixels;
} GLFWimage;
# 1744 "include/glfw3.h"
typedef struct GLFWgamepadstate
{



    unsigned char buttons[15];



    float axes[6];
} GLFWgamepadstate;
# 1797 "include/glfw3.h"
 int glfwInit(void);
# 1831 "include/glfw3.h"
 void glfwTerminate(void);
# 1863 "include/glfw3.h"
 void glfwInitHint(int hint, int value);
# 1890 "include/glfw3.h"
 void glfwGetVersion(int* major, int* minor, int* rev);
# 1921 "include/glfw3.h"
 const char* glfwGetVersionString(void);
# 1952 "include/glfw3.h"
 int glfwGetError(const char** description);
# 1998 "include/glfw3.h"
 GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun callback);
# 2027 "include/glfw3.h"
 GLFWmonitor** glfwGetMonitors(int* count);
# 2051 "include/glfw3.h"
 GLFWmonitor* glfwGetPrimaryMonitor(void);
# 2076 "include/glfw3.h"
 void glfwGetMonitorPos(GLFWmonitor* monitor, int* xpos, int* ypos);
# 2107 "include/glfw3.h"
 void glfwGetMonitorWorkarea(GLFWmonitor* monitor, int* xpos, int* ypos, int* width, int* height);
# 2141 "include/glfw3.h"
 void glfwGetMonitorPhysicalSize(GLFWmonitor* monitor, int* widthMM, int* heightMM);
# 2173 "include/glfw3.h"
 void glfwGetMonitorContentScale(GLFWmonitor* monitor, float* xscale, float* yscale);
# 2199 "include/glfw3.h"
 const char* glfwGetMonitorName(GLFWmonitor* monitor);
# 2225 "include/glfw3.h"
 void glfwSetMonitorUserPointer(GLFWmonitor* monitor, void* pointer);
# 2249 "include/glfw3.h"
 void* glfwGetMonitorUserPointer(GLFWmonitor* monitor);
# 2279 "include/glfw3.h"
 GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun callback);
# 2313 "include/glfw3.h"
 const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* count);
# 2341 "include/glfw3.h"
 const GLFWvidmode* glfwGetVideoMode(GLFWmonitor* monitor);
# 2374 "include/glfw3.h"
 void glfwSetGamma(GLFWmonitor* monitor, float gamma);
# 2404 "include/glfw3.h"
 const GLFWgammaramp* glfwGetGammaRamp(GLFWmonitor* monitor);
# 2445 "include/glfw3.h"
 void glfwSetGammaRamp(GLFWmonitor* monitor, const GLFWgammaramp* ramp);
# 2464 "include/glfw3.h"
 void glfwDefaultWindowHints(void);
# 2499 "include/glfw3.h"
 void glfwWindowHint(int hint, int value);
# 2537 "include/glfw3.h"
 void glfwWindowHintString(int hint, const char* value);
# 2691 "include/glfw3.h"
 GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
# 2720 "include/glfw3.h"
 void glfwDestroyWindow(GLFWwindow* window);
# 2740 "include/glfw3.h"
 int glfwWindowShouldClose(GLFWwindow* window);
# 2762 "include/glfw3.h"
 void glfwSetWindowShouldClose(GLFWwindow* window, int value);
# 2787 "include/glfw3.h"
 void glfwSetWindowTitle(GLFWwindow* window, const char* title);
# 2834 "include/glfw3.h"
 void glfwSetWindowIcon(GLFWwindow* window, int count, const GLFWimage* images);
# 2866 "include/glfw3.h"
 void glfwGetWindowPos(GLFWwindow* window, int* xpos, int* ypos);
# 2901 "include/glfw3.h"
 void glfwSetWindowPos(GLFWwindow* window, int xpos, int ypos);
# 2931 "include/glfw3.h"
 void glfwGetWindowSize(GLFWwindow* window, int* width, int* height);
# 2974 "include/glfw3.h"
 void glfwSetWindowSizeLimits(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight);
# 3017 "include/glfw3.h"
 void glfwSetWindowAspectRatio(GLFWwindow* window, int numer, int denom);
# 3058 "include/glfw3.h"
 void glfwSetWindowSize(GLFWwindow* window, int width, int height);
# 3087 "include/glfw3.h"
 void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height);
# 3124 "include/glfw3.h"
 void glfwGetWindowFrameSize(GLFWwindow* window, int* left, int* top, int* right, int* bottom);
# 3157 "include/glfw3.h"
 void glfwGetWindowContentScale(GLFWwindow* window, float* xscale, float* yscale);
# 3184 "include/glfw3.h"
 float glfwGetWindowOpacity(GLFWwindow* window);
# 3213 "include/glfw3.h"
 void glfwSetWindowOpacity(GLFWwindow* window, float opacity);
# 3241 "include/glfw3.h"
 void glfwIconifyWindow(GLFWwindow* window);
# 3268 "include/glfw3.h"
 void glfwRestoreWindow(GLFWwindow* window);
# 3293 "include/glfw3.h"
 void glfwMaximizeWindow(GLFWwindow* window);
# 3325 "include/glfw3.h"
 void glfwShowWindow(GLFWwindow* window);
# 3347 "include/glfw3.h"
 void glfwHideWindow(GLFWwindow* window);
# 3386 "include/glfw3.h"
 void glfwFocusWindow(GLFWwindow* window);
# 3413 "include/glfw3.h"
 void glfwRequestWindowAttention(GLFWwindow* window);
# 3435 "include/glfw3.h"
 GLFWmonitor* glfwGetWindowMonitor(GLFWwindow* window);
# 3494 "include/glfw3.h"
 void glfwSetWindowMonitor(GLFWwindow* window, GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);
# 3531 "include/glfw3.h"
 int glfwGetWindowAttrib(GLFWwindow* window, int attrib);
# 3568 "include/glfw3.h"
 void glfwSetWindowAttrib(GLFWwindow* window, int attrib, int value);
# 3591 "include/glfw3.h"
 void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);
# 3612 "include/glfw3.h"
 void* glfwGetWindowUserPointer(GLFWwindow* window);
# 3647 "include/glfw3.h"
 GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun callback);
# 3679 "include/glfw3.h"
 GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun callback);
# 3719 "include/glfw3.h"
 GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun callback);
# 3755 "include/glfw3.h"
 GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun callback);
# 3790 "include/glfw3.h"
 GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun callback);
# 3823 "include/glfw3.h"
 GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun callback);
# 3853 "include/glfw3.h"
 GLFWwindowmaximizefun glfwSetWindowMaximizeCallback(GLFWwindow* window, GLFWwindowmaximizefun callback);
# 3883 "include/glfw3.h"
 GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun callback);
# 3914 "include/glfw3.h"
 GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* window, GLFWwindowcontentscalefun callback);
# 3952 "include/glfw3.h"
 void glfwPollEvents(void);
# 3997 "include/glfw3.h"
 void glfwWaitEvents(void);
# 4046 "include/glfw3.h"
 void glfwWaitEventsTimeout(double timeout);
# 4066 "include/glfw3.h"
 void glfwPostEmptyEvent(void);
# 4091 "include/glfw3.h"
 int glfwGetInputMode(GLFWwindow* window, int mode);
# 4153 "include/glfw3.h"
 void glfwSetInputMode(GLFWwindow* window, int mode, int value);
# 4182 "include/glfw3.h"
 int glfwRawMouseMotionSupported(void);
# 4250 "include/glfw3.h"
 const char* glfwGetKeyName(int key, int scancode);
# 4274 "include/glfw3.h"
 int glfwGetKeyScancode(int key);
# 4313 "include/glfw3.h"
 int glfwGetKey(GLFWwindow* window, int key);
# 4342 "include/glfw3.h"
 int glfwGetMouseButton(GLFWwindow* window, int button);
# 4380 "include/glfw3.h"
 void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);
# 4420 "include/glfw3.h"
 void glfwSetCursorPos(GLFWwindow* window, double xpos, double ypos);
# 4458 "include/glfw3.h"
 GLFWcursor* glfwCreateCursor(const GLFWimage* image, int xhot, int yhot);
# 4481 "include/glfw3.h"
 GLFWcursor* glfwCreateStandardCursor(int shape);
# 4508 "include/glfw3.h"
 void glfwDestroyCursor(GLFWcursor* cursor);
# 4535 "include/glfw3.h"
 void glfwSetCursor(GLFWwindow* window, GLFWcursor* cursor);
# 4585 "include/glfw3.h"
 GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);
# 4628 "include/glfw3.h"
 GLFWcharfun glfwSetCharCallback(GLFWwindow* window, GLFWcharfun callback);
# 4670 "include/glfw3.h"
 GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun callback);
# 4707 "include/glfw3.h"
 GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback);
# 4739 "include/glfw3.h"
 GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);
# 4770 "include/glfw3.h"
 GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun callback);
# 4804 "include/glfw3.h"
 GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);
# 4841 "include/glfw3.h"
 GLFWdropfun glfwSetDropCallback(GLFWwindow* window, GLFWdropfun callback);
# 4865 "include/glfw3.h"
 int glfwJoystickPresent(int jid);
# 4898 "include/glfw3.h"
 const float* glfwGetJoystickAxes(int jid, int* count);
# 4939 "include/glfw3.h"
 const unsigned char* glfwGetJoystickButtons(int jid, int* count);
# 4996 "include/glfw3.h"
 const unsigned char* glfwGetJoystickHats(int jid, int* count);
# 5027 "include/glfw3.h"
 const char* glfwGetJoystickName(int jid);
# 5068 "include/glfw3.h"
 const char* glfwGetJoystickGUID(int jid);
# 5094 "include/glfw3.h"
 void glfwSetJoystickUserPointer(int jid, void* pointer);
# 5118 "include/glfw3.h"
 void* glfwGetJoystickUserPointer(int jid);
# 5146 "include/glfw3.h"
 int glfwJoystickIsGamepad(int jid);
# 5182 "include/glfw3.h"
 GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun callback);
# 5216 "include/glfw3.h"
 int glfwUpdateGamepadMappings(const char* string);
# 5248 "include/glfw3.h"
 const char* glfwGetGamepadName(int jid);
# 5286 "include/glfw3.h"
 int glfwGetGamepadState(int jid, GLFWgamepadstate* state);
# 5311 "include/glfw3.h"
 void glfwSetClipboardString(GLFWwindow* window, const char* string);
# 5341 "include/glfw3.h"
 const char* glfwGetClipboardString(GLFWwindow* window);
# 5371 "include/glfw3.h"
 double glfwGetTime(void);
# 5401 "include/glfw3.h"
 void glfwSetTime(double time);
# 5423 "include/glfw3.h"
 uint64_t glfwGetTimerValue(void);
# 5443 "include/glfw3.h"
 uint64_t glfwGetTimerFrequency(void);
# 5481 "include/glfw3.h"
 void glfwMakeContextCurrent(GLFWwindow* window);
# 5502 "include/glfw3.h"
 GLFWwindow* glfwGetCurrentContext(void);
# 5536 "include/glfw3.h"
 void glfwSwapBuffers(GLFWwindow* window);
# 5582 "include/glfw3.h"
 void glfwSwapInterval(int interval);
# 5620 "include/glfw3.h"
 int glfwExtensionSupported(const char* extension);
# 5662 "include/glfw3.h"
 GLFWglproc glfwGetProcAddress(const char* procname);
# 5688 "include/glfw3.h"
 int glfwVulkanSupported(void);
# 5732 "include/glfw3.h"
 const char** glfwGetRequiredInstanceExtensions(uint32_t* count);
# 17 "include/turtle.h" 2
# 1 "include/list.h" 1



# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 1 3
# 9 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt_stdio_config.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 2 3

#pragma pack(push,_CRT_PACKING)

       

       

       

       
# 33 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  
# 33 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
 struct _iobuf {



    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;

  };
  typedef struct _iobuf FILE;
# 99 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_off_t.h" 1 3




  typedef long _off_t;

  typedef long off32_t;





  __extension__ typedef long long _off64_t;

  __extension__ typedef long long off64_t;
# 26 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/_mingw_off_t.h" 3
typedef off32_t off_t;
# 100 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 2 3

__attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __acrt_iob_func(unsigned index);


  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __iob_func(void);
# 123 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __extension__ typedef long long fpos_t;
# 167 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
extern
  __attribute__((__format__(__gnu_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__gnu_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsscanf (const char * __restrict__ _Str,const char * __restrict__ Format,va_list argp);
extern
  __attribute__((__format__(__gnu_scanf__, 1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_scanf(const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__gnu_scanf__, 1,0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vscanf(const char * __restrict__ Format, va_list argp);
extern
  __attribute__((__format__(__gnu_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__gnu_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfscanf (FILE * __restrict__ fp, const char * __restrict__ Format,va_list argp);

extern
  __attribute__((__format__(__gnu_printf__,3,0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnprintf(char * __restrict__ _DstBuf,size_t _MaxCount,const char * __restrict__ _Format,
                               va_list _ArgList);
extern
  __attribute__((__format__(__gnu_printf__,3,4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);
extern
  __attribute__((__format__(__gnu_printf__,1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_printf(const char * __restrict__ , ... ) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,1,0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vprintf (const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fprintf (FILE * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfprintf (FILE * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sprintf (char * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsprintf (char * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,3))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_asprintf(char ** __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,0))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_vasprintf(char ** __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));

extern
  __attribute__((__format__(__ms_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__ms_scanf__, 1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_scanf(const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__ms_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);

extern
  __attribute__((__format__(__ms_printf__, 1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_printf(const char * __restrict__ , ... ) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 1,0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vprintf (const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fprintf (FILE * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfprintf (FILE * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_sprintf (char * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vsprintf (char * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
# 299 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
int sscanf(const char *__source, const char *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vsscanf( __source, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_scanf__, 1,2))) __attribute__ ((__nonnull__ (1)))
int scanf(const char *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfscanf( (__acrt_iob_func(0)), __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
int fscanf(FILE *__stream, const char *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfscanf( __stream, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"


static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
int vsscanf (const char *__source, const char *__format, __builtin_va_list __local_argv)
{
  return __mingw_vsscanf( __source, __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_scanf__, 1,0))) __attribute__ ((__nonnull__ (1)))
int vscanf(const char *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfscanf( (__acrt_iob_func(0)), __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
int vfscanf (FILE *__stream, const char *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfscanf( __stream, __format, __local_argv );
}


#pragma GCC diagnostic pop





static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,2,3))) __attribute__ ((__nonnull__ (2)))
int fprintf (FILE *__stream, const char *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfprintf( __stream, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,1,2))) __attribute__ ((__nonnull__ (1)))
int printf (const char *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfprintf( (__acrt_iob_func(1)), __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}
# 407 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,2,3))) __attribute__ ((__nonnull__ (2)))
int sprintf (char *__stream, const char *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vsprintf( __stream, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}



static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,2,0))) __attribute__ ((__nonnull__ (2)))
int vfprintf (FILE *__stream, const char *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfprintf( __stream, __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,1,0))) __attribute__ ((__nonnull__ (1)))
int vprintf (const char *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfprintf( (__acrt_iob_func(1)), __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,2,0))) __attribute__ ((__nonnull__ (2)))
int vsprintf (char *__stream, const char *__format, __builtin_va_list __local_argv)
{
# 446 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  return __mingw_vsprintf( __stream, __format, __local_argv );
}
# 464 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,3,4))) __attribute__ ((__nonnull__ (3)))
int snprintf (char *__stream, size_t __n, const char *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vsnprintf( __stream, __n, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}



static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
__attribute__((__format__(__gnu_printf__,3,0))) __attribute__ ((__nonnull__ (3)))
int vsnprintf (char *__stream, size_t __n, const char *__format, __builtin_va_list __local_argv)
{



  return __mingw_vsnprintf( __stream, __n, __format, __local_argv );
}
# 618 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _filbuf(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flsbuf(int _Ch,FILE *_File);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fsopen(const char *_Filename,const char *_Mode,int _ShFlag);

  void __attribute__((__cdecl__)) clearerr(FILE *_File);
  int __attribute__((__cdecl__)) fclose(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fcloseall(void);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fdopen(int _FileHandle,const char *_Mode);

  int __attribute__((__cdecl__)) feof(FILE *_File);
  int __attribute__((__cdecl__)) ferror(FILE *_File);
  int __attribute__((__cdecl__)) fflush(FILE *_File);
  int __attribute__((__cdecl__)) fgetc(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fgetchar(void);
  int __attribute__((__cdecl__)) fgetpos(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  int __attribute__((__cdecl__)) fgetpos64(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  char *__attribute__((__cdecl__)) fgets(char * __restrict__ _Buf,int _MaxCount,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fileno(FILE *_File);



  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _tempnam(const char *_DirName,const char *_FilePrefix);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flushall(void);
  FILE *__attribute__((__cdecl__)) fopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode) ;
  FILE *fopen64(const char * __restrict__ filename,const char * __restrict__ mode);
  int __attribute__((__cdecl__)) fputc(int _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fputchar(int _Ch);
  int __attribute__((__cdecl__)) fputs(const char * __restrict__ _Str,FILE * __restrict__ _File);
  size_t __attribute__((__cdecl__)) fread(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  FILE *__attribute__((__cdecl__)) freopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode,FILE * __restrict__ _File) ;
  int __attribute__((__cdecl__)) fsetpos(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fsetpos64(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fseek(FILE *_File,long _Offset,int _Origin);
  long __attribute__((__cdecl__)) ftell(FILE *_File);



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseeki64(FILE *_File,long long _Offset,int _Origin);
  __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _ftelli64(FILE *_File);
# 677 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  int fseeko64(FILE* stream, _off64_t offset, int whence);
  int fseeko(FILE* stream, _off_t offset, int whence);

  _off_t ftello(FILE * stream);
  _off64_t ftello64(FILE * stream);
# 698 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  size_t __attribute__((__cdecl__)) fwrite(const void * __restrict__ _Str,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) getc(FILE *_File);
  int __attribute__((__cdecl__)) getchar(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _getmaxstdio(void);
  char *__attribute__((__cdecl__)) gets(char *_Buffer)
    __attribute__((__warning__("Using gets() is always unsafe - use fgets() instead")));
  int __attribute__((__cdecl__)) _getw(FILE *_File);


  void __attribute__((__cdecl__)) perror(const char *_ErrMsg);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _pclose(FILE *_File);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _popen(const char *_Command,const char *_Mode);





  int __attribute__((__cdecl__)) putc(int _Ch,FILE *_File);
  int __attribute__((__cdecl__)) putchar(int _Ch);
  int __attribute__((__cdecl__)) puts(const char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putw(int _Word,FILE *_File);


  int __attribute__((__cdecl__)) remove(const char *_Filename);
  int __attribute__((__cdecl__)) rename(const char *_OldFilename,const char *_NewFilename);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _unlink(const char *_Filename);

  int __attribute__((__cdecl__)) unlink(const char *_Filename) ;


  void __attribute__((__cdecl__)) rewind(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _rmtmp(void);
  void __attribute__((__cdecl__)) setbuf(FILE * __restrict__ _File,char * __restrict__ _Buffer) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _setmaxstdio(int _Max);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _set_output_format(unsigned int _Format);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _get_output_format(void);
  int __attribute__((__cdecl__)) setvbuf(FILE * __restrict__ _File,char * __restrict__ _Buf,int _Mode,size_t _Size);







  __attribute__ ((__pure__))
  __attribute__((__format__(__ms_printf__, 1,2))) __attribute__ ((__nonnull__ (1)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf(const char * __restrict__ _Format,...);
  __attribute__((__format__(__ms_scanf__, 3,4))) __attribute__ ((__nonnull__ (3)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf(const char * __restrict__ _Src,size_t _MaxCount,const char * __restrict__ _Format,...) ;

  __attribute__ ((__pure__))
  __attribute__((__format__(__ms_printf__, 1,0))) __attribute__ ((__nonnull__ (1)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf(const char * __restrict__ _Format,va_list _ArgList);
  FILE *__attribute__((__cdecl__)) tmpfile(void) ;
  char *__attribute__((__cdecl__)) tmpnam(char *_Buffer);
  int __attribute__((__cdecl__)) ungetc(int _Ch,FILE *_File);







  __attribute__((__format__(__ms_printf__, 3,4))) __attribute__ ((__nonnull__ (3)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,...) ;
  __attribute__((__format__(__ms_printf__, 3,0))) __attribute__ ((__nonnull__ (3)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,va_list _Args) ;
# 988 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_printf_count_output(int _Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _get_printf_count_output(void);




                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vswscanf (const wchar_t * __restrict__ _Str,const wchar_t * __restrict__ Format,va_list argp);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wscanf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwscanf(const wchar_t * __restrict__ Format, va_list argp);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwscanf (FILE * __restrict__ fp, const wchar_t * __restrict__ Format,va_list argp);

                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                      __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wprintf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnwprintf (wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list);
                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_swprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ , ...);
                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vswprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ ,va_list);

                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...);
                                                    __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_wscanf(const wchar_t * __restrict__ _Format,...);
                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);

                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_wprintf(const wchar_t * __restrict__ _Format,...);
                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                    __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_swprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ , ...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vswprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ ,va_list);
# 1056 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                     __attribute__ ((__nonnull__ (2)))
int swscanf(const wchar_t *__source, const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vswscanf( __source, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                     __attribute__ ((__nonnull__ (1)))
int wscanf(const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfwscanf( (__acrt_iob_func(0)), __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                     __attribute__ ((__nonnull__ (2)))
int fwscanf(FILE *__stream, const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfwscanf( __stream, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}


static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                     __attribute__ ((__nonnull__ (2)))
int vswscanf (const wchar_t * __restrict__ __source, const wchar_t * __restrict__ __format, __builtin_va_list __local_argv)
{
  return __mingw_vswscanf( __source, __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                     __attribute__ ((__nonnull__ (1)))
int vwscanf(const wchar_t *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfwscanf( (__acrt_iob_func(0)), __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                     __attribute__ ((__nonnull__ (2)))
int vfwscanf (FILE *__stream, const wchar_t *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfwscanf( __stream, __format, __local_argv );
}




static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (2)))
int fwprintf (FILE *__stream, const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfwprintf( __stream, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (1)))
int wprintf (const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfwprintf( (__acrt_iob_func(1)), __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (2)))
int vfwprintf (FILE *__stream, const wchar_t *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfwprintf( __stream, __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (1)))
int vwprintf (const wchar_t *__format, __builtin_va_list __local_argv)
{
  return __mingw_vfwprintf( (__acrt_iob_func(1)), __format, __local_argv );
}
# 1166 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int snwprintf (wchar_t *__stream, size_t __n, const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vsnwprintf( __stream, __n, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}



static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int vsnwprintf (wchar_t *__stream, size_t __n, const wchar_t *__format, __builtin_va_list __local_argv)
{



  return __mingw_vsnwprintf( __stream, __n, __format, __local_argv );
}
# 1310 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfsopen(const wchar_t *_Filename,const wchar_t *_Mode,int _ShFlag);


  wint_t __attribute__((__cdecl__)) fgetwc(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fgetwchar(void);
  wint_t __attribute__((__cdecl__)) fputwc(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fputwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) getwc(FILE *_File);
  wint_t __attribute__((__cdecl__)) getwchar(void);
  wint_t __attribute__((__cdecl__)) putwc(wchar_t _Ch,FILE *_File);
  wint_t __attribute__((__cdecl__)) putwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) ungetwc(wint_t _Ch,FILE *_File);
  wchar_t *__attribute__((__cdecl__)) fgetws(wchar_t * __restrict__ _Dst,int _SizeInWords,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) fputws(const wchar_t * __restrict__ _Str,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putws(const wchar_t *_Str);
# 1383 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf(const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,va_list _Args) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
# 1413 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,va_list _Args);



# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/swprintf.inl" 1 3
# 12 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/swprintf.inl" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int vswprintf (wchar_t *__stream, size_t __count, const wchar_t *__format, __builtin_va_list __local_argv)
{
  return vsnwprintf( __stream, __count, __format, __local_argv );
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
                                                      __attribute__ ((__nonnull__ (3)))
int swprintf (wchar_t *__stream, size_t __count, const wchar_t *__format, ...)
{
  int __retval;
  __builtin_va_list __local_argv;

  __builtin_va_start( __local_argv, __format );
  __retval = vswprintf( __stream, __count, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}
# 1419 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 2 3
# 1428 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtempnam(const wchar_t *_Directory,const wchar_t *_FilePrefix);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf(const wchar_t * __restrict__ _Src,size_t _MaxCount,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfdopen(int _FileHandle ,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfopen(const wchar_t * __restrict__ _Filename,const wchar_t *__restrict__ _Mode) ;
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfreopen(const wchar_t * __restrict__ _Filename,const wchar_t * __restrict__ _Mode,FILE * __restrict__ _OldFile) ;



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wperror(const wchar_t *_ErrMsg);

  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wpopen(const wchar_t *_Command,const wchar_t *_Mode);




  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wremove(const wchar_t *_Filename);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtmpnam(wchar_t *_Buffer);
# 1488 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _lock_file(FILE *_File);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _unlock_file(FILE *_File);
# 1506 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  char *__attribute__((__cdecl__)) tempnam(const char *_Directory,const char *_FilePrefix) ;
  int __attribute__((__cdecl__)) fcloseall(void) ;
  FILE *__attribute__((__cdecl__)) fdopen(int _FileHandle,const char *_Format) ;
  int __attribute__((__cdecl__)) fgetchar(void) ;
  int __attribute__((__cdecl__)) fileno(FILE *_File) ;
  int __attribute__((__cdecl__)) flushall(void) ;
  int __attribute__((__cdecl__)) fputchar(int _Ch) ;
  int __attribute__((__cdecl__)) getw(FILE *_File) ;
  int __attribute__((__cdecl__)) putw(int _Ch,FILE *_File) ;
  int __attribute__((__cdecl__)) rmtmp(void) ;
# 1532 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_wide_utf8 (const wchar_t * const wptr, char **mbptr, size_t * buflen);
# 1546 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_utf8_wide (const char *const mbptr, wchar_t ** wptr, size_t * buflen);
# 1555 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
void __attribute__((__cdecl__)) __mingw_str_free(void *ptr);






  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnl(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnle(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlp(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlpe(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnv(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnve(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvp(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvpe(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
# 1586 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnv(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnve(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvp(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvpe(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);







       
       
       
       

#pragma pack(pop)

# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 1 3
# 9 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 2 3
# 29 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) clearerr_s(FILE *_File);

  size_t __attribute__((__cdecl__)) fread_s(void *_DstBuf,size_t _DstSize,size_t _ElementSize,size_t _Count,FILE *_File);
# 515 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
  int __attribute__((__cdecl__)) fprintf_s(FILE *_File,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fscanf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) fscanf_s(FILE *_File, const char *_Format, ...);
  int __attribute__((__cdecl__)) printf_s(const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scanf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scanf_s_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) scanf_s(const char *_Format, ...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_c(char *_DstBuf,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_c(char *_DstBuf,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fscanf_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sscanf_l(const char *_Src,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sscanf_s_l(const char *_Src,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) sscanf_s(const char *_Src,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_s(const char *_Src,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_l(const char *_Src,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snscanf_s_l(const char *_Src,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  int __attribute__((__cdecl__)) vfprintf_s(FILE *_File,const char *_Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vprintf_s(const char *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) vsnprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,va_list _ArgList);

  __attribute__((dllimport)) int __attribute__((__cdecl__)) vsprintf_s(char *_DstBuf,size_t _Size,const char *_Format,va_list _ArgList);

  __attribute__((dllimport)) int __attribute__((__cdecl__)) sprintf_s(char *_DstBuf,size_t _DstSize,const char *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_s(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_p(FILE *_File,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_p(const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_p(char *_Dst,size_t _MaxCount,const char *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_p(FILE *_File,const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_p(const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_p(char *_Dst,size_t _MaxCount,const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_p(const char *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vscprintf_p(const char *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_p_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_p_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_p_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_p_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_l(char *_DstBuf,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_p_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_l(char *_DstBuf,const char *_Format,_locale_t,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_p_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scprintf_p_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf_p_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _printf_s_l(const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vprintf_s_l(const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fprintf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfprintf_s_l(FILE *_File,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sprintf_s_l(char *_DstBuf,size_t _DstSize,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsprintf_s_l(char *_DstBuf,size_t _DstSize,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_s_l(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_s_l(char *_DstBuf,size_t _DstSize,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snprintf_c_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_l(char *_DstBuf,size_t _MaxCount,const char *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnprintf_c_l(char *_DstBuf,size_t _MaxCount,const char *,_locale_t _Locale,va_list _ArgList);


 
 
 
 
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) fopen_s(FILE **_File,const char *_Filename,const char *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) freopen_s(FILE** _File, const char *_Filename, const char *_Mode, FILE *_Stream);

  __attribute__ ((__dllimport__)) char* __attribute__((__cdecl__)) gets_s(char*,rsize_t);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) tmpfile_s(FILE **_File);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) tmpnam_s(char*,rsize_t);
 




  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws_s(wchar_t *_Str,size_t _SizeInWords);
 
# 830 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
  int __attribute__((__cdecl__)) fwprintf_s(FILE *_File,const wchar_t *_Format,...);
  int __attribute__((__cdecl__)) wprintf_s(const wchar_t *_Format,...);
  int __attribute__((__cdecl__)) vfwprintf_s(FILE *_File,const wchar_t *_Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vwprintf_s(const wchar_t *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) vswprintf_s(wchar_t *_Dst,size_t _SizeInWords,const wchar_t *_Format,va_list _ArgList);

  int __attribute__((__cdecl__)) swprintf_s(wchar_t *_Dst,size_t _SizeInWords,const wchar_t *_Format,...);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_s(wchar_t *_DstBuf,size_t _DstSizeInWords,size_t _MaxCount,const wchar_t *_Format,va_list _ArgList);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_s(wchar_t *_DstBuf,size_t _DstSizeInWords,size_t _MaxCount,const wchar_t *_Format,...);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_s_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_s_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_s_l(wchar_t *_DstBuf,size_t _DstSize,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwscanf_s_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) fwscanf_s(FILE *_File, const wchar_t *_Format, ...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swscanf_s_l(const wchar_t *_Src,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) swscanf_s(const wchar_t *_Src,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_s(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_s_l(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wscanf_s_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) wscanf_s(const wchar_t *_Format, ...);


 
 
 
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfreopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode,FILE *_OldFile);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wtmpnam_s(wchar_t *_DstBuf,size_t _SizeInWords);
 


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_p(FILE *_File,const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_p(const wchar_t *_Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_p(FILE *_File,const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_p(const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_p(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vswprintf_p(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_p(const wchar_t *_Format,...);
  __attribute__((dllimport)) int __attribute__((__cdecl__)) _vscwprintf_p(const wchar_t *_Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wprintf_p_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwprintf_p_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vfwprintf_p_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_c_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf_p_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_c_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf_p_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf_p_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf_l(wchar_t *_DstBuf,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) __swprintf_l(wchar_t *_Dest,const wchar_t *_Format,_locale_t _Plocinfo,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) __vswprintf_l(wchar_t *_Dest,const wchar_t *_Format,_locale_t _Plocinfo,va_list _Args);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf_l(const wchar_t *_Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fwscanf_l(FILE *_File,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swscanf_l(const wchar_t *_Src,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf_l(const wchar_t *_Src,size_t _MaxCount,const wchar_t *_Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wscanf_l(const wchar_t *_Format,_locale_t _Locale,...);
# 1605 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdio.h" 2 3
# 5 "include/list.h" 2
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt_wstdlib.h" 1 3
# 15 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/corecrt_wstdlib.h" 3
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _itow_s (int _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ltow_s (long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ultow_s (unsigned long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wgetenv_s(size_t *_ReturnSize,wchar_t *_DstBuf,size_t _DstSizeInWords,const wchar_t *_VarName);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wdupenv_s(wchar_t **_Buffer,size_t *_BufferSizeInWords,const wchar_t *_VarName);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _i64tow_s(long long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ui64tow_s(unsigned long long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wmakepath_s(wchar_t *_PathResult,size_t _SizeInWords,const wchar_t *_Drive,const wchar_t *_Dir,const wchar_t *_Filename,const wchar_t *_Ext);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wputenv_s(const wchar_t *_Name,const wchar_t *_Value);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsearchenv_s(const wchar_t *_Filename,const wchar_t *_EnvVar,wchar_t *_ResultPath,size_t _SizeInWords);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsplitpath_s(const wchar_t *_FullPath,wchar_t *_Drive,size_t _DriveSizeInWords,wchar_t *_Dir,size_t _DirSizeInWords,wchar_t *_Filename,size_t _FilenameSizeInWords,wchar_t *_Ext,size_t _ExtSizeInWords);
 
# 11 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/limits.h" 1 3 4
# 34 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/limits.h" 3 4
# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/syslimits.h" 1 3 4






# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/limits.h" 1 3 4
# 210 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/limits.h" 3 4
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/limits.h" 1 3 4
# 211 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/limits.h" 2 3 4
# 8 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/syslimits.h" 2 3 4
# 35 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/limits.h" 2 3 4
# 12 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 26 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
#pragma pack(push,_CRT_PACKING)
# 50 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  typedef int (__attribute__((__cdecl__)) *_onexit_t)(void);
# 60 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  typedef struct _div_t {
    int quot;
    int rem;
  } div_t;

  typedef struct _ldiv_t {
    long quot;
    long rem;
  } ldiv_t;





#pragma pack(4)
  typedef struct {
    unsigned char ld[10];
  } _LDOUBLE;
#pragma pack()



  typedef struct {
    double x;
  } _CRT_DOUBLE;

  typedef struct {
    float f;
  } _CRT_FLOAT;

       


  typedef struct {
    long double x;
  } _LONGDOUBLE;

       

#pragma pack(4)
  typedef struct {
    unsigned char ld12[12];
  } _LDBL12;
#pragma pack()
# 116 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  extern int * __imp___mb_cur_max;




__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) ___mb_cur_max_func(void);
# 143 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  typedef void (__attribute__((__cdecl__)) *_purecall_handler)(void);

  __attribute__ ((__dllimport__)) _purecall_handler __attribute__((__cdecl__)) _set_purecall_handler(_purecall_handler _Handler);
  __attribute__ ((__dllimport__)) _purecall_handler __attribute__((__cdecl__)) _get_purecall_handler(void);

  typedef void (__attribute__((__cdecl__)) *_invalid_parameter_handler)(const wchar_t *,const wchar_t *,const wchar_t *,unsigned int,uintptr_t);
  __attribute__ ((__dllimport__)) _invalid_parameter_handler __attribute__((__cdecl__)) _set_invalid_parameter_handler(_invalid_parameter_handler _Handler);
  __attribute__ ((__dllimport__)) _invalid_parameter_handler __attribute__((__cdecl__)) _get_invalid_parameter_handler(void);
# 159 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) unsigned long *__attribute__((__cdecl__)) __doserrno(void);

  errno_t __attribute__((__cdecl__)) _set_doserrno(unsigned long _Value);
  errno_t __attribute__((__cdecl__)) _get_doserrno(unsigned long *_Value);
# 173 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  extern __attribute__((dllimport)) char *_sys_errlist[1];
  extern __attribute__((dllimport)) int _sys_nerr;





  __attribute__ ((__dllimport__)) char ***__attribute__((__cdecl__)) __p___argv(void);
  __attribute__ ((__dllimport__)) int *__attribute__((__cdecl__)) __p__fmode(void);
# 191 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  errno_t __attribute__((__cdecl__)) _get_pgmptr(char **_Value);
  errno_t __attribute__((__cdecl__)) _get_wpgmptr(wchar_t **_Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _set_fmode(int _Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_fmode(int *_PMode);
# 282 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  extern int * __imp___argc;



  extern char *** __imp___argv;



  extern wchar_t *** __imp___wargv;
# 322 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  extern char *** __imp__environ;




  extern wchar_t *** __imp__wenviron;






  extern char ** __imp__pgmptr;




  extern wchar_t ** __imp__wpgmptr;




  extern unsigned int * __imp__osplatform;




  extern unsigned int * __imp__osver;




  extern unsigned int * __imp__winver;




  extern unsigned int * __imp__winmajor;




  extern unsigned int * __imp__winminor;





  errno_t __attribute__((__cdecl__)) _get_osplatform(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_osver(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winver(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winmajor(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winminor(unsigned int *_Value);
# 388 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) exit(int _Code) __attribute__ ((__noreturn__));
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _exit(int _Code) __attribute__ ((__noreturn__));






  void __attribute__((__cdecl__)) _Exit(int) __attribute__ ((__noreturn__));






       

  void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) abort(void);
       



  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _set_abort_behavior(unsigned int _Flags,unsigned int _Mask);







  __extension__ long long __attribute__((__cdecl__)) _abs64(long long);

  extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) long long __attribute__((__cdecl__)) _abs64(long long x) {
    return __builtin_llabs(x);
  }


  int __attribute__((__cdecl__)) atexit(void (__attribute__((__cdecl__)) *)(void));
# 434 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  int __attribute__((__cdecl__)) atoi(const char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoi_l(const char *_Str,_locale_t _Locale);
  long __attribute__((__cdecl__)) atol(const char *_Str);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _atol_l(const char *_Str,_locale_t _Locale);


  void *__attribute__((__cdecl__)) bsearch(const void *_Key,const void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(const void *,const void *));
  void __attribute__((__cdecl__)) qsort(void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(const void *,const void *));

  unsigned short __attribute__((__cdecl__)) _byteswap_ushort(unsigned short _Short);
  unsigned long __attribute__((__cdecl__)) _byteswap_ulong (unsigned long _Long);
  __extension__ unsigned long long __attribute__((__cdecl__)) _byteswap_uint64(unsigned long long _Int64);
  div_t __attribute__((__cdecl__)) div(int _Numerator,int _Denominator);
  char *__attribute__((__cdecl__)) getenv(const char *_VarName) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _itoa(int _Value,char *_Dest,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _i64toa(long long _Val,char *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ui64toa(unsigned long long _Val,char *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _atoi64(const char *_String);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _atoi64_l(const char *_String,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _strtoi64(const char *_String,char **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _strtoi64_l(const char *_String,char **_EndPtr,int _Radix,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _strtoui64(const char *_String,char **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _strtoui64_l(const char *_String,char **_EndPtr,int _Radix,_locale_t _Locale);
  ldiv_t __attribute__((__cdecl__)) ldiv(long _Numerator,long _Denominator);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ltoa(long _Value,char *_Dest,int _Radix) ;
  int __attribute__((__cdecl__)) mblen(const char *_Ch,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _mblen_l(const char *_Ch,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrlen(const char *_Str);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrlen_l(const char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrnlen(const char *_Str,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrnlen_l(const char *_Str,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) mbtowc(wchar_t * __restrict__ _DstCh,const char * __restrict__ _SrcCh,size_t _SrcSizeInBytes);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _mbtowc_l(wchar_t * __restrict__ _DstCh,const char * __restrict__ _SrcCh,size_t _SrcSizeInBytes,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) mbstowcs(wchar_t * __restrict__ _Dest,const char * __restrict__ _Source,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstowcs_l(wchar_t * __restrict__ _Dest,const char * __restrict__ _Source,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) mkstemp(char *template_name);
  int __attribute__((__cdecl__)) rand(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_error_mode(int _Mode);
  void __attribute__((__cdecl__)) srand(unsigned int _Seed);
# 483 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
double __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtod(const char * __restrict__ _Str,char ** __restrict__ _EndPtr)
{
  double __attribute__((__cdecl__)) __mingw_strtod (const char * __restrict__, char ** __restrict__);
  return __mingw_strtod( _Str, _EndPtr);
}

static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
float __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtof(const char * __restrict__ _Str,char ** __restrict__ _EndPtr)
{
  float __attribute__((__cdecl__)) __mingw_strtof (const char * __restrict__, char ** __restrict__);
  return __mingw_strtof( _Str, _EndPtr);
}






  long double __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtold(const char * __restrict__ , char ** __restrict__ );


  extern double __attribute__((__cdecl__)) __attribute__ ((__nothrow__))
  __strtod (const char * __restrict__ , char ** __restrict__);







  float __attribute__((__cdecl__)) __mingw_strtof (const char * __restrict__, char ** __restrict__);
  double __attribute__((__cdecl__)) __mingw_strtod (const char * __restrict__, char ** __restrict__);
  long double __attribute__((__cdecl__)) __mingw_strtold(const char * __restrict__, char ** __restrict__);

  __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _strtof_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,_locale_t _Locale);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _strtod_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,_locale_t _Locale);
  long __attribute__((__cdecl__)) strtol(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _strtol_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  unsigned long __attribute__((__cdecl__)) strtoul(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _strtoul_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);


  int __attribute__((__cdecl__)) system(const char *_Command);

  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ultoa(unsigned long _Value,char *_Dest,int _Radix) ;
  int __attribute__((__cdecl__)) wctomb(char *_MbCh,wchar_t _WCh) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wctomb_l(char *_MbCh,wchar_t _WCh,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcstombs(char * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcstombs_l(char * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _MaxCount,_locale_t _Locale) ;



  void *__attribute__((__cdecl__)) calloc(size_t _NumOfElements,size_t _SizeOfElements);
  void __attribute__((__cdecl__)) free(void *_Memory);
  void *__attribute__((__cdecl__)) malloc(size_t _Size);
  void *__attribute__((__cdecl__)) realloc(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _aligned_free(void *_Memory);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_malloc(size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_malloc(size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_realloc(void *_Memory,size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_realloc(void *_Memory,size_t _Size,size_t _Alignment,size_t _Offset);
# 556 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _itow(int _Value,wchar_t *_Dest,int _Radix) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ltow(long _Value,wchar_t *_Dest,int _Radix) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ultow(unsigned long _Value,wchar_t *_Dest,int _Radix) ;

  double __attribute__((__cdecl__)) __mingw_wcstod(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr);
  float __attribute__((__cdecl__)) __mingw_wcstof(const wchar_t * __restrict__ nptr, wchar_t ** __restrict__ endptr);
  long double __attribute__((__cdecl__)) __mingw_wcstold(const wchar_t * __restrict__, wchar_t ** __restrict__);


  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  double __attribute__((__cdecl__)) wcstod(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr){
    return __mingw_wcstod(_Str,_EndPtr);
  }
  static __attribute__ ((__unused__)) __inline__ __attribute__((__cdecl__))
  float __attribute__((__cdecl__)) wcstof(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr){
    return __mingw_wcstof(_Str,_EndPtr);
  }






  long double __attribute__((__cdecl__)) wcstold(const wchar_t * __restrict__, wchar_t ** __restrict__);

  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wcstod_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,_locale_t _Locale);
  __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _wcstof_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,_locale_t _Locale);
  long __attribute__((__cdecl__)) wcstol(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wcstol_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  unsigned long __attribute__((__cdecl__)) wcstoul(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _wcstoul_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wgetenv(const wchar_t *_VarName) ;


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wsystem(const wchar_t *_Command);

  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wtof(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wtof_l(const wchar_t *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wtoi(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wtoi_l(const wchar_t *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wtol(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wtol_l(const wchar_t *_Str,_locale_t _Locale);

  __extension__ __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _i64tow(long long _Val,wchar_t *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ui64tow(unsigned long long _Val,wchar_t *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wtoi64(const wchar_t *_Str);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wtoi64_l(const wchar_t *_Str,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wcstoi64(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wcstoi64_l(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _wcstoui64(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _wcstoui64_l(const wchar_t *_Str ,wchar_t **_EndPtr,int _Radix,_locale_t _Locale);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putenv(const char *_EnvString);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wputenv(const wchar_t *_EnvString);



  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _fullpath(char *_FullPath,const char *_Path,size_t _SizeInBytes);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ecvt(double _Val,int _NumOfDigits,int *_PtDec,int *_PtSign) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _fcvt(double _Val,int _NumOfDec,int *_PtDec,int *_PtSign) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _gcvt(double _Val,int _NumOfDigits,char *_DstBuf) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atodbl(_CRT_DOUBLE *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoldbl(_LDOUBLE *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoflt(_CRT_FLOAT *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atodbl_l(_CRT_DOUBLE *_Result,char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoldbl_l(_LDOUBLE *_Result,char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoflt_l(_CRT_FLOAT *_Result,char *_Str,_locale_t _Locale);
# 639 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
unsigned long __attribute__((__cdecl__)) _lrotl(unsigned long,int);
unsigned long __attribute__((__cdecl__)) _lrotr(unsigned long,int);





  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _makepath(char *_Path,const char *_Drive,const char *_Dir,const char *_Filename,const char *_Ext);
  _onexit_t __attribute__((__cdecl__)) _onexit(_onexit_t _Func);





       
       


  __extension__ unsigned long long __attribute__((__cdecl__)) _rotl64(unsigned long long _Val,int _Shift);
  __extension__ unsigned long long __attribute__((__cdecl__)) _rotr64(unsigned long long Value,int Shift);
       
       
       
       


  unsigned int __attribute__((__cdecl__)) _rotr(unsigned int _Val,int _Shift);
  unsigned int __attribute__((__cdecl__)) _rotl(unsigned int _Val,int _Shift);
       
       
  __extension__ unsigned long long __attribute__((__cdecl__)) _rotr64(unsigned long long _Val,int _Shift);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _searchenv(const char *_Filename,const char *_EnvVar,char *_ResultPath) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _splitpath(const char *_FullPath,char *_Drive,char *_Dir,char *_Filename,char *_Ext) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _swab(char *_Buf1,char *_Buf2,int _SizeInBytes);



  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wfullpath(wchar_t *_FullPath,const wchar_t *_Path,size_t _SizeInWords);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wmakepath(wchar_t *_ResultPath,const wchar_t *_Drive,const wchar_t *_Dir,const wchar_t *_Filename,const wchar_t *_Ext);




  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wsearchenv(const wchar_t *_Filename,const wchar_t *_EnvVar,wchar_t *_ResultPath) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wsplitpath(const wchar_t *_FullPath,wchar_t *_Drive,wchar_t *_Dir,wchar_t *_Filename,wchar_t *_Ext) ;


  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _beep(unsigned _Frequency,unsigned _Duration) __attribute__ ((__deprecated__));

  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _seterrormode(int _Mode) __attribute__ ((__deprecated__));
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _sleep(unsigned long _Duration) __attribute__ ((__deprecated__));
# 710 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
  char *__attribute__((__cdecl__)) ecvt(double _Val,int _NumOfDigits,int *_PtDec,int *_PtSign) ;
  char *__attribute__((__cdecl__)) fcvt(double _Val,int _NumOfDec,int *_PtDec,int *_PtSign) ;
  char *__attribute__((__cdecl__)) gcvt(double _Val,int _NumOfDigits,char *_DstBuf) ;
  char *__attribute__((__cdecl__)) itoa(int _Val,char *_DstBuf,int _Radix) ;
  char *__attribute__((__cdecl__)) ltoa(long _Val,char *_DstBuf,int _Radix) ;
  int __attribute__((__cdecl__)) putenv(const char *_EnvString) ;



  void __attribute__((__cdecl__)) swab(char *_Buf1,char *_Buf2,int _SizeInBytes) ;


  char *__attribute__((__cdecl__)) ultoa(unsigned long _Val,char *_Dstbuf,int _Radix) ;
  _onexit_t __attribute__((__cdecl__)) onexit(_onexit_t _Func);





  typedef struct { __extension__ long long quot, rem; } lldiv_t;

  __extension__ lldiv_t __attribute__((__cdecl__)) lldiv(long long, long long);

  __extension__ long long __attribute__((__cdecl__)) llabs(long long);




  __extension__ long long __attribute__((__cdecl__)) strtoll(const char * __restrict__, char ** __restrict, int);
  __extension__ unsigned long long __attribute__((__cdecl__)) strtoull(const char * __restrict__, char ** __restrict__, int);


  __extension__ long long __attribute__((__cdecl__)) atoll (const char *);


  __extension__ long long __attribute__((__cdecl__)) wtoll (const wchar_t *);
  __extension__ char *__attribute__((__cdecl__)) lltoa (long long, char *, int);
  __extension__ char *__attribute__((__cdecl__)) ulltoa (unsigned long long , char *, int);
  __extension__ wchar_t *__attribute__((__cdecl__)) lltow (long long, wchar_t *, int);
  __extension__ wchar_t *__attribute__((__cdecl__)) ulltow (unsigned long long, wchar_t *, int);
# 767 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 3
#pragma pack(pop)

# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdlib_s.h" 1 3
# 9 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdlib_s.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/stdlib_s.h" 2 3





  __attribute__ ((__dllimport__)) void * __attribute__((__cdecl__)) bsearch_s(const void *_Key,const void *_Base,rsize_t _NumOfElements,rsize_t _SizeOfElements,int (__attribute__((__cdecl__)) * _PtFuncCompare)(void *, const void *, const void *), void *_Context);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _dupenv_s(char **_PBuffer,size_t *_PBufferSizeInBytes,const char *_VarName);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) getenv_s(size_t *_ReturnSize,char *_DstBuf,rsize_t _DstSize,const char *_VarName);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _itoa_s(int _Value,char *_DstBuf,size_t _Size,int _Radix);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _i64toa_s(long long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ui64toa_s(unsigned long long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ltoa_s(long _Val,char *_DstBuf,size_t _Size,int _Radix);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) mbstowcs_s(size_t *_PtNumOfCharConverted,wchar_t *_DstBuf,size_t _SizeInWords,const char *_SrcBuf,size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _mbstowcs_s_l(size_t *_PtNumOfCharConverted,wchar_t *_DstBuf,size_t _SizeInWords,const char *_SrcBuf,size_t _MaxCount,_locale_t _Locale);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ultoa_s(unsigned long _Val,char *_DstBuf,size_t _Size,int _Radix);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wctomb_s(int *_SizeConverted,char *_MbCh,rsize_t _SizeInBytes,wchar_t _WCh);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wctomb_s_l(int *_SizeConverted,char *_MbCh,size_t _SizeInBytes,wchar_t _WCh,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcstombs_s(size_t *_PtNumOfCharConverted,char *_Dst,size_t _DstSizeInBytes,const wchar_t *_Src,size_t _MaxCountInBytes);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcstombs_s_l(size_t *_PtNumOfCharConverted,char *_Dst,size_t _DstSizeInBytes,const wchar_t *_Src,size_t _MaxCountInBytes,_locale_t _Locale);
 


  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ecvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDights,int *_PtDec,int *_PtSign);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _fcvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDec,int *_PtDec,int *_PtSign);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _gcvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDigits);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _makepath_s(char *_PathResult,size_t _Size,const char *_Drive,const char *_Dir,const char *_Filename,const char *_Ext);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _putenv_s(const char *_Name,const char *_Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _searchenv_s(const char *_Filename,const char *_EnvVar,char *_ResultPath,size_t _SizeInBytes);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _splitpath_s(const char *_FullPath,char *_Drive,size_t _DriveSize,char *_Dir,size_t _DirSize,char *_Filename,size_t _FilenameSize,char *_Ext,size_t _ExtSize);
 



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) qsort_s(void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(void *,const void *,const void *),void *_Context);
# 770 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 1 3
# 11 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 3
#pragma pack(push,_CRT_PACKING)
# 46 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 3
  typedef struct _heapinfo {
    int *_pentry;
    size_t _size;
    int _useflag;
  } _HEAPINFO;


  extern unsigned int _amblksiz;
# 77 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 3
void * __mingw_aligned_malloc (size_t _Size, size_t _Alignment);
void __mingw_aligned_free (void *_Memory);
void * __mingw_aligned_offset_realloc (void *_Memory, size_t _Size, size_t _Alignment, size_t _Offset);
void * __mingw_aligned_realloc (void *_Memory, size_t _Size, size_t _Offset);



# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/mm_malloc.h" 1 3 4
# 29 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/mm_malloc.h" 3 4
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/errno.h" 1 3 4
# 30 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/mm_malloc.h" 2 3 4


static __inline__ void *
_mm_malloc (size_t __size, size_t __align)
{
  void * __malloc_ptr;
  void * __aligned_ptr;


  if (__align & (__align - 1))
    {

      (*_errno()) = 22;

      return ((void *) 0);
    }

  if (__size == 0)
    return ((void *) 0);





    if (__align < 2 * sizeof (void *))
      __align = 2 * sizeof (void *);

  __malloc_ptr = malloc (__size + __align);
  if (!__malloc_ptr)
    return ((void *) 0);


  __aligned_ptr = (void *) (((size_t) __malloc_ptr + __align)
       & ~((size_t) (__align) - 1));


  ((void **) __aligned_ptr)[-1] = __malloc_ptr;

  return __aligned_ptr;
}

static __inline__ void
_mm_free (void *__aligned_ptr)
{
  if (__aligned_ptr)
    free (((void **) __aligned_ptr)[-1]);
}
# 85 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 2 3





  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _resetstkoflw (void);

  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _set_malloc_crt_max_wait(unsigned long _NewValue);

  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _expand(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _msize(void *_Memory);






  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _get_sbh_threshold(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_sbh_threshold(size_t _NewValue);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _set_amblksiz(size_t _Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_amblksiz(size_t *_Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapadd(void *_Memory,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapchk(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapmin(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapset(unsigned int _Fill);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapwalk(_HEAPINFO *_EntryInfo);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _heapused(size_t *_Used,size_t *_Commit);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _get_heap_handle(void);
# 125 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 3
  static __inline void *_MarkAllocaS(void *_Ptr,unsigned int _Marker) {
    if(_Ptr) {
      *((unsigned int*)_Ptr) = _Marker;
      _Ptr = (char*)_Ptr + 16;
    }
    return _Ptr;
  }
# 144 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 3
  static __inline void __attribute__((__cdecl__)) _freea(void *_Memory) {
    unsigned int _Marker;
    if(_Memory) {
      _Memory = (char*)_Memory - 16;
      _Marker = *(unsigned int *)_Memory;
      if(_Marker==0xDDDD) {
 free(_Memory);
      }





    }
  }
# 190 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/malloc.h" 3
#pragma pack(pop)
# 771 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 6 "include/list.h" 2
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/string.h" 1 3
# 45 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/string.h" 3
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _memccpy(void *_Dst,const void *_Src,int _Val,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memchr(const void *_Buf ,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp_l(const void *_Buf1,const void *_Buf2,size_t _Size,_locale_t _Locale);
  int __attribute__((__cdecl__)) memcmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  void * __attribute__((__cdecl__)) memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _Size) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memcpy_s (void *_dest,size_t _numberOfElements,const void *_src,size_t _count);
  void * __attribute__((__cdecl__)) mempcpy (void *_Dst, const void *_Src, size_t _Size);
  void * __attribute__((__cdecl__)) memset(void *_Dst,int _Val,size_t _Size);

  void * __attribute__((__cdecl__)) memccpy(void *_Dst,const void *_Src,int _Val,size_t _Size) ;
  int __attribute__((__cdecl__)) memicmp(const void *_Buf1,const void *_Buf2,size_t _Size) ;


  char * __attribute__((__cdecl__)) _strset(char *_Str,int _Val) ;
  char * __attribute__((__cdecl__)) _strset_l(char *_Str,int _Val,_locale_t _Locale) ;
  char * __attribute__((__cdecl__)) strcpy(char * __restrict__ _Dest,const char * __restrict__ _Source);
  char * __attribute__((__cdecl__)) strcat(char * __restrict__ _Dest,const char * __restrict__ _Source);
  int __attribute__((__cdecl__)) strcmp(const char *_Str1,const char *_Str2);
  size_t __attribute__((__cdecl__)) strlen(const char *_Str);
  size_t __attribute__((__cdecl__)) strnlen(const char *_Str,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memmove(void *_Dst,const void *_Src,size_t _Size) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strdup(const char *_Src);
  char *__attribute__((__cdecl__)) strchr(const char *_Str,int _Val);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcmpi(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  int __attribute__((__cdecl__)) strcoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) strcspn(const char *_Str,const char *_Control);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strerror(const char *_ErrMsg) ;
  char *__attribute__((__cdecl__)) strerror(int) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strlwr(char *_String) ;
  char *strlwr_l(char *_String,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strncat(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) strncmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  char *strncpy(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset(char *_Str,int _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset_l(char *str,int c,size_t count,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strpbrk(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strrchr(const char *_Str,int _Ch);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strrev(char *_Str);
  size_t __attribute__((__cdecl__)) strspn(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strstr(const char *_Str,const char *_SubStr);
  char *__attribute__((__cdecl__)) strtok(char * __restrict__ _Str,const char * __restrict__ _Delim) ;
       

  char *strtok_r(char * __restrict__ _Str, const char * __restrict__ _Delim, char ** __restrict__ __last);
       
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strupr(char *_String) ;
  __attribute__ ((__dllimport__)) char *_strupr_l(char *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) strxfrm(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _strxfrm_l(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);


  char *__attribute__((__cdecl__)) strdup(const char *_Src) ;
  int __attribute__((__cdecl__)) strcmpi(const char *_Str1,const char *_Str2) ;
  int __attribute__((__cdecl__)) stricmp(const char *_Str1,const char *_Str2) ;
  char *__attribute__((__cdecl__)) strlwr(char *_Str) ;
  int __attribute__((__cdecl__)) strnicmp(const char *_Str1,const char *_Str,size_t _MaxCount) ;
  int __attribute__((__cdecl__)) strncasecmp (const char *, const char *, size_t);
  int __attribute__((__cdecl__)) strcasecmp (const char *, const char *);







  char *__attribute__((__cdecl__)) strnset(char *_Str,int _Val,size_t _MaxCount) ;
  char *__attribute__((__cdecl__)) strrev(char *_Str) ;
  char *__attribute__((__cdecl__)) strset(char *_Str,int _Val) ;
  char *__attribute__((__cdecl__)) strupr(char *_Str) ;





  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsdup(const wchar_t *_Str);
  wchar_t *__attribute__((__cdecl__)) wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  wchar_t *__attribute__((__cdecl__)) wcschr(const wchar_t *_Str,wchar_t _Ch);
  int __attribute__((__cdecl__)) wcscmp(const wchar_t *_Str1,const wchar_t *_Str2);
  wchar_t *__attribute__((__cdecl__)) wcscpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  size_t __attribute__((__cdecl__)) wcscspn(const wchar_t *_Str,const wchar_t *_Control);
  size_t __attribute__((__cdecl__)) wcslen(const wchar_t *_Str);
  size_t __attribute__((__cdecl__)) wcsnlen(const wchar_t *_Src,size_t _MaxCount);
  wchar_t *wcsncat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) wcsncmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  wchar_t *wcsncpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  wchar_t *__attribute__((__cdecl__)) _wcsncpy_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count,_locale_t _Locale) ;
  wchar_t *__attribute__((__cdecl__)) wcspbrk(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsrchr(const wchar_t *_Str,wchar_t _Ch);
  size_t __attribute__((__cdecl__)) wcsspn(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsstr(const wchar_t *_Str,const wchar_t *_SubStr);



  wchar_t *__attribute__((__cdecl__)) wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim) ;

  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcserror(int _ErrNum) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) __wcserror(const wchar_t *_Str) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsrev(wchar_t *_Str);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsset(wchar_t *_Str,wchar_t _Val) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcslwr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcslwr_l(wchar_t *_String,_locale_t _Locale) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsupr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcsupr_l(wchar_t *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcsxfrm(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcsxfrm_l(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) wcscoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcscoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);


  wchar_t *__attribute__((__cdecl__)) wcsdup(const wchar_t *_Str) ;

  int __attribute__((__cdecl__)) wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2) ;
  int __attribute__((__cdecl__)) wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsrev(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsset(wchar_t *_Str,wchar_t _Val) ;
  wchar_t *__attribute__((__cdecl__)) wcslwr(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsupr(wchar_t *_Str) ;
  int __attribute__((__cdecl__)) wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2) ;







# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/string_s.h" 1 3
# 9 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/string_s.h" 3
# 1 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/string.h" 1 3
# 10 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/string_s.h" 2 3
# 24 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/sec_api/string_s.h" 3
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strset_s(char *_Dst,size_t _DstSize,int _Value);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strerror_s(char *_Buf,size_t _SizeInBytes,const char *_ErrMsg);
 
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) strerror_s(char *_Buf,size_t _SizeInBytes,int _ErrNum);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s(char *_Str,size_t _Size);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s_l(char *_Str,size_t _Size,_locale_t _Locale);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strnset_s(char *_Str,size_t _Size,int _Val,size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s(char *_Str,size_t _Size);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s_l(char *_Str,size_t _Size,_locale_t _Locale);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncat_s(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncat_s_l(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount,_locale_t _Locale);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcpy_s(char *_Dst, rsize_t _SizeInBytes, const char *_Src);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncpy_s(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncpy_s_l(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount, _locale_t _Locale);
 
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) strtok_s(char *_Str,const char *_Delim,char **_Context);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strtok_s_l(char *_Str,const char *_Delim,char **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcat_s(char *_Dst, rsize_t _SizeInBytes, const char * _Src);
 

  extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) size_t __attribute__((__cdecl__)) strnlen_s(const char * _src, size_t _count) {
    return _src ? strnlen(_src, _count) : 0;
  }

  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memmove_s(void *_dest,size_t _numberOfElements,const void *_src,size_t _count);


  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) wcstok_s(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcserror_s(wchar_t *_Buf,size_t _SizeInWords,int _ErrNum);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) __wcserror_s(wchar_t *_Buffer,size_t _SizeInWords,const wchar_t *_ErrMsg);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s(wchar_t *_Dst,size_t _DstSizeInWords,wchar_t _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s(wchar_t *_Str,size_t _SizeInWords,wchar_t _Val);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s(wchar_t *_Str,size_t _SizeInWords);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s_l(wchar_t *_Str,size_t _SizeInWords,_locale_t _Locale);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s(wchar_t *_Str,size_t _Size);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s_l(wchar_t *_Str,size_t _Size,_locale_t _Locale);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscpy_s(wchar_t *_Dst, rsize_t _SizeInWords, const wchar_t *_Src);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscat_s(wchar_t * _Dst, rsize_t _SizeInWords, const wchar_t *_Src);
 

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncat_s(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncat_s_l(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount,_locale_t _Locale);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncpy_s(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncpy_s_l(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount, _locale_t _Locale);
 
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcstok_s_l(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s_l(wchar_t *_Str,size_t _SizeInChars,wchar_t _Val,_locale_t _Locale);
 
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s_l(wchar_t *_Str,size_t _SizeInChars,wchar_t _Val, size_t _Count,_locale_t _Locale);
 

  extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) size_t __attribute__((__cdecl__)) wcsnlen_s(const wchar_t * _src, size_t _count) {
    return _src ? wcsnlen(_src, _count) : 0;
  }
# 195 "C:/Program Files (x86)/w64devkit/x86_64-w64-mingw32/include/string.h" 2 3
# 7 "include/list.h" 2
# 1 "C:/Program Files (x86)/w64devkit/lib/gcc/x86_64-w64-mingw32/14.2.0/include/stdbool.h" 1 3 4
# 8 "include/list.h" 2
# 43 "include/list.h"

# 43 "include/list.h"
typedef enum {
    LIST_TYPE_BOOL = 'b',
    LIST_TYPE_CHAR = 'c',
    LIST_TYPE_INT8 = 'e',
    LIST_TYPE_UINT8 = 'b',
    LIST_TYPE_INT16 = 'j',
    LIST_TYPE_UINT16 = 'h',
    LIST_TYPE_INT32 = 'i',
    LIST_TYPE_UINT32 = 'u',
    LIST_TYPE_INT64 = 'm',
    LIST_TYPE_UINT64 = 'l',
    LIST_TYPE_FLOAT = 'f',
    LIST_TYPE_DOUBLE = 'd',
    LIST_TYPE_STRING = 's',
    LIST_TYPE_POINTER = 'p',
    LIST_TYPE_LIST = 'r',
} list_type_t;

struct list_f;
typedef struct list_f list_t;

typedef union {
    signed char ch;
    
# 66 "include/list.h" 3 4
   _Bool 
# 66 "include/list.h"
        bo;
    int8_t c;
    uint8_t b;
    int16_t h;
    uint16_t hu;
    int32_t i;
    uint32_t u;
    int64_t li;
    uint64_t l;
    float f;
    double d;
    double lf;
    char *s;
    void *p;
    list_t *r;
    list_t *list;
} unitype;

struct list_f {
    uint32_t length;
    uint32_t realLength;
    int8_t *type;
    unitype *data;
};


list_t *list_init() {
    list_t *list = malloc(sizeof(list_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> type = calloc(1, sizeof(char));
    list -> data = calloc(1, sizeof(unitype));
    return list;
}

void list_free_lite(list_t *);
void list_free(list_t *);
void list_fprint_emb(FILE *fp, list_t *);


void list_append(list_t *list, unitype data, char type) {
    if (list -> realLength <= list -> length) {
        list -> realLength *= 2;
        list -> type = realloc(list -> type, list -> realLength);
        list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
    }
    if (type == 'z') {
        list -> type[list -> length] = 's';
    } else {
        list -> type[list -> length] = type;
    }
    if (type == 's') {
        list -> data[list -> length].s = strdup(data.s);
    } else {
        list -> data[list -> length] = data;
    }
    list -> length += 1;
}


void list_insert(list_t *list, int32_t index, unitype value, char type) {
    if (list -> length == index) {
        list_append(list, value, type);
        return;
    }
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    list_append(list, (unitype) 0, type);
    int32_t i;
    for (i = list -> length - 1; i > index; i--) {
        list -> data[i] = list -> data[i - 1];
        list -> type[i] = list -> type[i - 1];
    }
    list -> data[i] = value;
}


void list_clear(list_t *list) {
    list_free_lite(list);
    list -> length = 0;
    list -> realLength = 1;
    list -> type = calloc(1, sizeof(char));
    list -> data = calloc(1, sizeof(unitype));
}


unitype list_pop(list_t *list) {
    if (list -> length > 0) {
        list -> length -= 1;
        unitype ret = list -> data[list -> length];
        if (list -> type[list -> length] == 'r') {
            list_free(list -> data[list -> length].r);
        }
        if (list -> type[list -> length] == 's' || list -> type[list -> length] == 'p') {
            free(list -> data[list -> length].p);
        }
        list -> type[list -> length] = (char) 0;
        list -> data[list -> length] = (unitype) 0;
        if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
            list -> realLength /= 2;
            list -> type = realloc(list -> type, list -> realLength);
            list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
        }
        return ret;
    } else {
        return (unitype) 0;
    }
}


unitype list_delete(list_t *list, int32_t index) {
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    unitype ret = list -> data[index];
    if (list -> type[index] == 'r') {
        list_free(list -> data[index].r);
    }
    if (list -> type[index] == 's' || list -> type[index] == 'p') {
        free(list -> data[index].p);
    }
    for (uint32_t i = index; i < list -> length - 1 ; i++) {
        list -> data[i] = list -> data[i + 1];
        list -> type[i] = list -> type[i + 1];
    }
    list -> length -= 1;
    list -> type[list -> length] = (char) 0;
    list -> data[list -> length] = (unitype) 0;
    if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
        list -> realLength /= 2;
        list -> type = realloc(list -> type, list -> realLength);
        list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
    }
    return ret;
}


void list_delete_range(list_t *list, uint32_t indexMin, uint32_t indexMax) {
    if (indexMin > indexMax) {
        uint32_t swap = indexMin;
        indexMin = indexMax;
        indexMax = swap;
    }
    int8_t zerod = 0;
    uint32_t difference = (indexMax - indexMin);
    list -> realLength = list -> length - difference;
    if (list -> realLength <= 1) {
        zerod = 1;
        list -> realLength = 1;
    }

    int8_t *newType = malloc(list -> realLength * sizeof(int8_t));
    unitype *newData = malloc(list -> realLength * sizeof(unitype));
    for (uint32_t i = 0; i < indexMin; i++) {
        newType[i] = list -> type[i];
        newData[i] = list -> data[i];
    }
    for (uint32_t i = indexMax; i < list -> length; i++) {
        newType[i - difference] = list -> type[i];
        newData[i - difference] = list -> data[i];
    }
    list -> length = list -> realLength;
    if (zerod)
        list -> length = 0;
    free(list -> type);
    free(list -> data);
    list -> type = newType;
    list -> data = newData;
}


int32_t unitype_check_equal(unitype item1, unitype item2, int8_t typeItem1, int8_t typeItem2) {
    if ((typeItem1 == 's' || typeItem2 == 's') && typeItem1 != typeItem2) {
        return 0;
    }

    switch (typeItem1) {
        case LIST_TYPE_CHAR:
            if (item1.c == item2.c) {return 1;}
            return 0;
        case LIST_TYPE_INT8:
        case LIST_TYPE_UINT8:
            if (item1.b == item2.b) {return 1;}
            return 0;
        case LIST_TYPE_INT16:
        case LIST_TYPE_UINT16:
            if (item1.hu == item2.hu) {return 1;}
            return 0;
        case LIST_TYPE_INT32:
        case LIST_TYPE_UINT32:
            if (item1.u == item2.u) {return 1;}
            return 0;
        case LIST_TYPE_INT64:
        case LIST_TYPE_UINT64:
            if (item1.l == item2.l) {return 1;}
            return 0;
        case LIST_TYPE_FLOAT:
            if (item1.f == item2.f) {return 1;}
            return 0;
        case LIST_TYPE_DOUBLE:
            if (item1.d == item2.d) {return 1;}
            return 0;
        case LIST_TYPE_STRING:
            if (strcmp(item1.s, item2.s) == 0) {return 1;}
            return 0;
        case LIST_TYPE_POINTER:
            if (item1.p == item2.p) {return 1;}
            return 0;
        case LIST_TYPE_LIST:
            if (item1.r == item2.r) {return 1;}
            return 0;
        default:
            printf("unitype_check_equal - type %d not recognized\n", typeItem1);
            return 0;
    }
}


int32_t list_find(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            return i;
        }
    }
    return -1;
}


int32_t list_index(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            return i;
        }
    }
    return -1;
}


uint32_t list_count(list_t *list, unitype item, char type) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < list -> length; i++) {
        count += unitype_check_equal(list -> data[i], item, list -> type[i], type);
    }
    return count;
}


void list_sort(list_t *list) {

    int32_t temp;
    for (uint32_t i = 2; i < list -> length + 1; i++) {
        int32_t j = i;
        while (j > 1 && list -> data[j / 2 - 1].i > list -> data[j - 1].i) {
            temp = list -> data[j / 2 - 1].i;
            list -> data[j / 2 - 1].i = list -> data[j - 1].i;
            list -> data[j - 1].i = temp;
            j /= 2;
        }
    }

    for (int32_t i = list -> length - 1; i > 0; i--) {
        temp = list -> data[0].i;
        list -> data[0].i = list -> data[i].i;
        list -> data[i].i = temp;
        int32_t j = 1;
        while ((j * 2 - 1 < i && list -> data[j - 1].i > list -> data[j * 2 - 1].i) || (j * 2 < i && list -> data[j - 1].i > list -> data[j * 2].i)) {
            if (list -> data[j * 2 - 1].i < list -> data[j * 2].i || j * 2 == i) {
                temp = list -> data[j - 1].i;
                list -> data[j - 1] = list -> data[j * 2 - 1];
                list -> data[j * 2 - 1].i = temp;
                j = j * 2;
            } else {
                temp = list -> data[j - 1].i;
                list -> data[j - 1].i = list -> data[j * 2].i;
                list -> data[j * 2].i = temp;
                j *= 2 + 1;
            }
        }
    }
}


int32_t list_remove(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            list_delete(list, i);
            return i;
        }
    }
    return -1;
}


void unitype_fprint(FILE *fp, unitype item, char type) {
    switch (type) {
        case LIST_TYPE_CHAR:
            fprintf(fp, "%c", item.c);
        break;
        case LIST_TYPE_INT8:
            fprintf(fp, "%hhi", item.b);
        break;
        case LIST_TYPE_UINT8:
            fprintf(fp, "%hhu", item.b);
        break;
        case LIST_TYPE_INT16:
            fprintf(fp, "%hi", item.h);
        break;
        case LIST_TYPE_UINT16:
            fprintf(fp, "%hu", item.hu);
        break;
        case LIST_TYPE_INT32:
            fprintf(fp, "%d", item.i);
        break;
        case LIST_TYPE_UINT32:
            fprintf(fp, "%u", item.u);
        break;
        case LIST_TYPE_INT64:
            fprintf(fp, "%lli", item.li);
        break;
        case LIST_TYPE_UINT64:
            fprintf(fp, "%llu", item.l);
        break;
        case LIST_TYPE_FLOAT:
            fprintf(fp, "%f", item.f);
        break;
        case LIST_TYPE_DOUBLE:
            fprintf(fp, "%lf", item.d);
        break;
        case LIST_TYPE_STRING:
            fprintf(fp, "%s", item.s);
        break;
        case LIST_TYPE_POINTER:
            fprintf(fp, "%p", item.p);
        break;
        case LIST_TYPE_LIST:
            list_fprint_emb(fp, item.r);
        break;
        default:
            printf("unitype_fprint - type %d not recognized\n", type);
            return;
    }
}


void list_copy(list_t *dest, list_t *src) {
    list_free_lite(dest);
    dest -> type = calloc(src -> realLength, sizeof(int32_t));
    dest -> data = calloc(src -> realLength, sizeof(unitype));
    uint32_t len = src -> length;
    dest -> length = len;
    dest -> realLength = src -> realLength;
    for (uint32_t i = 0; i < len; i++) {
        dest -> type[i] = src -> type[i];
        if (src -> type[i] == 'r') {
            dest -> data[i] = (unitype) (void *) list_init();
            list_copy(dest -> data[i].r, src -> data[i].r);
        } else {
            if (src -> type[i] == 'p') {
                memcpy(dest -> data[i].p, src -> data[i].p, sizeof(unitype));
            } else {
                if (src -> type[i] == 's') {
                    dest -> data[i].s = strdup(src -> data[i].s);
                } else {
                    dest -> data[i] = src -> data[i];
                }
            }
        }
    }
}


void list_fprint_emb(FILE *fp, list_t *list) {
    fprintf(fp, "[");
    if (list -> length == 0) {
        fprintf(fp, "]");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        unitype_fprint(fp, list -> data[i], list -> type[i]);
        if (i == list -> length - 1) {
            fprintf(fp, "]");
        } else {
            fprintf(fp, ", ");
        }
    }
}


void list_print(list_t *list) {
    list_fprint_emb(
# 454 "include/list.h" 3
                   (__acrt_iob_func(1))
# 454 "include/list.h"
                         , list);
    printf("\n");
}


void list_print_type(list_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]\n");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        printf("%c", list -> type[i]);
        if (i == list -> length - 1) {
            printf("]\n");
        } else {
            printf(", ");
        }
    }
}


void list_free_lite(list_t *list) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (list -> type[i] == 'r') {
            list_free(list -> data[i].r);
        }
        if ((list -> type[i] == 's' || list -> type[i] == 'p') && list -> data[i].p != 
# 481 "include/list.h" 3 4
                                                                                      ((void *)0)
# 481 "include/list.h"
                                                                                          ) {
            free(list -> data[i].s);
        }
    }
    free(list -> type);
    free(list -> data);
}


void list_free(list_t *list) {
    list_free_lite(list);
    free(list);
}
# 18 "include/turtle.h" 2

extern void glColor4d(double r, double g, double b, double a);
extern void glBegin(int type);
extern void glVertex2d(double x, double y);
extern void glEnd();
void turtleTexture(int textureCode, double x1, double y1, double x2, double y2, double rot, double r, double g, double b);

typedef struct {
    GLFWwindow* window;
    list_t *keyPressed;
    void (*keyCallback)(int32_t key, int32_t scancode, int32_t action);
    void (*unicodeCallback)(uint32_t codepoint);
    int8_t mousePressed[4];
    int32_t screenbounds[2];
    int32_t lastscreenbounds[2];
    int32_t initscreenbounds[2];
    int32_t bounds[4];
    double mouseX;
    double mouseY;
    double scrollY;
    double mouseScaX;
    double mouseScaY;
    double mouseAbsX;
    double mouseAbsY;
    double x;
    double y;
    list_t *penPos;
    uint64_t penHash;
    uint32_t lastLength;
    uint8_t pen;
    uint16_t penshape;
    uint8_t close;
    uint8_t popupClose;
    double circleprez;
    double pensize;
    double penr;
    double peng;
    double penb;
    double pena;
    double currentColor[4];


    double cameraX;
    double cameraY;
    double cameraZ;
    double cameraFOV;
    double cameraDirectionLeftRight;
    double cameraDirectionUpDown;
} turtle_t;

turtle_t turtle;


void turtleSetWorldCoordinates(int32_t minX, int32_t minY, int32_t maxX, int32_t maxY) {
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    turtle.initscreenbounds[0] = turtle.screenbounds[0];
    turtle.initscreenbounds[1] = turtle.screenbounds[1];
    turtle.bounds[0] = minX;
    turtle.bounds[1] = minY;
    turtle.bounds[2] = maxX;
    turtle.bounds[3] = maxY;
}


void unicodeSense(GLFWwindow *window, uint32_t codepoint) {
    if (turtle.unicodeCallback != 
# 83 "include/turtle.h" 3 4
                                 ((void *)0)
# 83 "include/turtle.h"
                                     ) {
        turtle.unicodeCallback(codepoint);
    }
}


void keySense(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    if (turtle.keyCallback != 
# 90 "include/turtle.h" 3 4
                             ((void *)0)
# 90 "include/turtle.h"
                                 ) {
        turtle.keyCallback(key, scancode, action);
    }
    if (action == 1) {
        list_append(turtle.keyPressed, (unitype) key, 'i');
    }
    if (action == 0) {
        list_remove(turtle.keyPressed, (unitype) key, 'i');
    }
}


void mouseSense(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
    if (action == 1) {
        switch(button) {
        case 0:
            list_append(turtle.keyPressed, (unitype) "m1", 's');
            turtle.mousePressed[0] = 1;
        break;
        case 1:
            list_append(turtle.keyPressed, (unitype) "m2", 's');
            turtle.mousePressed[1] = 1;
        break;
        case 2:
            list_append(turtle.keyPressed, (unitype) "m3", 's');
            turtle.mousePressed[2] = 1;
        break;
        default:
        break;
        }
    }
    if (action == 0) {
        switch(button) {
        case 0:
            list_remove(turtle.keyPressed, (unitype) "m1", 's');
            turtle.mousePressed[0] = 0;
        break;
        case 1:
            list_remove(turtle.keyPressed, (unitype) "m2", 's');
            turtle.mousePressed[1] = 0;
        break;
        case 2:
            list_remove(turtle.keyPressed, (unitype) "m3", 's');
            turtle.mousePressed[2] = 0;
        break;
        default:
        break;
        }
    }
}


void scrollSense(GLFWwindow* window, double xoffset, double yoffset) {
    turtle.scrollY = yoffset;
}


double turtleMouseWheel() {
    double temp = turtle.scrollY;
    turtle.scrollY = 0;
    return temp;
}


int8_t turtleKeyPressed(int32_t key) {
    return list_count(turtle.keyPressed, (unitype) key, 'c');
}


int8_t turtleMouseDown() {
    return turtle.mousePressed[0];
}


int8_t turtleMouseRight() {
    return turtle.mousePressed[1];
}


int8_t turtleMouseMiddle() {
    return turtle.mousePressed[2];
}


int8_t turtleMouseMid() {
    return turtle.mousePressed[2];
}


void turtleInit(GLFWwindow* window, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY) {
    gladLoadGL();
    glfwMakeContextCurrent(window);
    glad_glEnable(0x1906);
    glad_glEnable(0x0BE2);
    glad_glBlendFunc(0x0303, 0x0302);
    glad_glClearColor(1.0, 1.0, 1.0, 0.0);
    turtle.window = window;
    turtle.close = 0;
    turtle.popupClose = 0;
    turtle.keyPressed = list_init();
    turtle.lastscreenbounds[0] = 0;
    turtle.lastscreenbounds[1] = 0;
    turtle.penPos = list_init();
    turtle.penHash = 0;
    turtle.lastLength = 0;
    turtle.x = 0;
    turtle.y = 0;
    turtle.pensize = 1;
    turtle.penshape = 0;
    turtle.circleprez = 9;
    turtle.pen = 0;
    turtle.penr = 0.0;
    turtle.peng = 0.0;
    turtle.penb = 0.0;
    turtle.pena = 0.0;
    for (uint8_t i = 0; i < 3; i++) {
        turtle.currentColor[i] = 0.0;
    }
    turtle.currentColor[3] = 1.0;

    turtle.cameraX = 0;
    turtle.cameraY = 0;
    turtle.cameraZ = 0;
    turtle.cameraFOV = 90;
    turtle.cameraDirectionLeftRight = 0;
    turtle.cameraDirectionUpDown = 0;

    turtleSetWorldCoordinates(minX, minY, maxX, maxY);
    turtle.keyCallback = 
# 218 "include/turtle.h" 3 4
                        ((void *)0)
# 218 "include/turtle.h"
                            ;
    turtle.unicodeCallback = 
# 219 "include/turtle.h" 3 4
                            ((void *)0)
# 219 "include/turtle.h"
                                ;
    glfwSetCharCallback(window, unicodeSense);
    glfwSetKeyCallback(window, keySense);
    glfwSetMouseButtonCallback(window, mouseSense);
    glfwSetScrollCallback(window, scrollSense);
}


void turtleGetMouseCoords() {
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    glfwGetCursorPos(turtle.window, &turtle.mouseAbsX, &turtle.mouseAbsY);
    turtle.mouseX = turtle.mouseAbsX;
    turtle.mouseY = turtle.mouseAbsY;
    turtle.mouseScaX = turtle.mouseAbsX;
    turtle.mouseScaY = turtle.mouseAbsY;
    turtle.mouseX -= (turtle.initscreenbounds[0] / 2) - ((turtle.bounds[2] + turtle.bounds[0]) / 2);
    turtle.mouseX *= ((double) (turtle.bounds[2] - turtle.bounds[0]) / (double) turtle.initscreenbounds[0]);
    turtle.mouseY -= (turtle.initscreenbounds[1] / 2) - ((turtle.bounds[3] + turtle.bounds[1]) / 2) + (turtle.screenbounds[1] - turtle.initscreenbounds[1]);
    turtle.mouseY *= ((double) (turtle.bounds[1] - turtle.bounds[3]) / (double) turtle.initscreenbounds[1]);
    turtle.mouseScaX -= (turtle.screenbounds[0] / 2) - ((turtle.bounds[2] + turtle.bounds[0]) / 2);
    turtle.mouseScaX *= ((double) (turtle.bounds[2] - turtle.bounds[0]) / (double) turtle.screenbounds[0]);
    turtle.mouseScaY -= (turtle.screenbounds[1] / 2) - ((turtle.bounds[3] + turtle.bounds[1]) / 2);
    turtle.mouseScaY *= ((double) (turtle.bounds[1] - turtle.bounds[3]) / (double) turtle.screenbounds[1]);
}


void turtleBgColor(double r, double g, double b) {
    glad_glClearColor(r / 255, g / 255, b / 255, 0.0);
}


void turtlePenColor(double r, double g, double b) {
    turtle.penr = r / 255;
    turtle.peng = g / 255;
    turtle.penb = b / 255;
    turtle.pena = 0.0;
}


void turtlePenColorAlpha(double r, double g, double b, double a) {
    turtle.penr = r / 255;
    turtle.peng = g / 255;
    turtle.penb = b / 255;
    turtle.pena = a / 255;
}


void turtlePenSize(double size) {
    turtle.pensize = size * 0.5;
}


void turtleClear() {


    turtle.penPos -> length = 0;
}


void turtlePenDown() {
    if (turtle.pen == 0) {
        turtle.pen = 1;
        int8_t changed = 0;
        int32_t len = turtle.penPos -> length;
        if (len > 0) {
            unitype *ren = turtle.penPos -> data;
            if (ren[len - 9].d != turtle.x) {changed = 1;}
            if (ren[len - 8].d != turtle.y) {changed = 1;}
            if (ren[len - 7].d != turtle.pensize) {changed = 1;}
            if (ren[len - 6].d != turtle.penr) {changed = 1;}
            if (ren[len - 5].d != turtle.peng) {changed = 1;}
            if (ren[len - 4].d != turtle.penb) {changed = 1;}
            if (ren[len - 3].d != turtle.pena) {changed = 1;}
            if (ren[len - 2].h != turtle.penshape) {changed = 1;}
            if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
        } else {
            changed = 1;
        }
        if (changed == 1) {
            list_append(turtle.penPos, (unitype) turtle.x, 'd');
            list_append(turtle.penPos, (unitype) turtle.y, 'd');
            list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
            list_append(turtle.penPos, (unitype) turtle.penr, 'd');
            list_append(turtle.penPos, (unitype) turtle.peng, 'd');
            list_append(turtle.penPos, (unitype) turtle.penb, 'd');
            list_append(turtle.penPos, (unitype) turtle.pena, 'd');
            list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
            list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
        }
    }
}


void turtlePenUp() {
    if (turtle.pen == 1) {
        turtle.pen = 0;
        if (turtle.penPos -> length > 0 && turtle.penPos -> type[turtle.penPos -> length - 1] != 'c') {
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
        }
    }
}


void turtlePenShape(char *selection) {
    if (strcmp(selection, "circle") == 0 || strcmp(selection, "Circle") == 0) {
        turtle.penshape = 0;
    }
    if (strcmp(selection, "square") == 0 || strcmp(selection, "Square") == 0) {
        turtle.penshape = 1;
    }
    if (strcmp(selection, "triangle") == 0 || strcmp(selection, "Triangle") == 0) {
        turtle.penshape = 2;
    }
    if (strcmp(selection, "none") == 0 || strcmp(selection, "None") == 0) {
        turtle.penshape = 3;
    }
    if (strcmp(selection, "connected") == 0 || strcmp(selection, "Connected") == 0) {
        turtle.penshape = 4;
    }
    if (strcmp(selection, "text") == 0 || strcmp(selection, "Text") == 0) {
        turtle.penshape = 5;
    }
}


void turtlePenPrez(double prez) {
    turtle.circleprez = prez;
}


void turtleGoto(double x, double y) {
    if (fabs(turtle.x - x) > 0.01 || fabs(turtle.y - y) > 0.01) {
        turtle.x = x;
        turtle.y = y;
        if (turtle.pen == 1) {
            int8_t changed = 0;
            int32_t len = turtle.penPos -> length;
            if (len > 0) {
                unitype *ren = turtle.penPos -> data;
                if (ren[len - 9].d != turtle.x) {changed = 1;}
                if (ren[len - 8].d != turtle.y) {changed = 1;}
                if (ren[len - 7].d != turtle.pensize) {changed = 1;}
                if (ren[len - 6].d != turtle.penr) {changed = 1;}
                if (ren[len - 5].d != turtle.peng) {changed = 1;}
                if (ren[len - 4].d != turtle.penb) {changed = 1;}
                if (ren[len - 3].d != turtle.pena) {changed = 1;}
                if (ren[len - 2].h != turtle.penshape) {changed = 1;}
                if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
            } else {
                changed = 1;
            }
            if (changed == 1) {
                list_append(turtle.penPos, (unitype) x, 'd');
                list_append(turtle.penPos, (unitype) y, 'd');
                list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
                list_append(turtle.penPos, (unitype) turtle.penr, 'd');
                list_append(turtle.penPos, (unitype) turtle.peng, 'd');
                list_append(turtle.penPos, (unitype) turtle.penb, 'd');
                list_append(turtle.penPos, (unitype) turtle.pena, 'd');
                list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
                list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
            }
        }
    }
}


void turtleCircleRender(double x, double y, double rad, double r, double g, double b, double a, double xfact, double yfact, double prez) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(0x0006);
    for (double i = 0; i < prez; i++) {
        glVertex2d((x + rad * sin(2 * i * 
# 409 "include/turtle.h" 3
                                         3.14159265358979323846 
# 409 "include/turtle.h"
                                              / prez)) * xfact, (y + rad * cos(2 * i * 
# 409 "include/turtle.h" 3
                                                                                       3.14159265358979323846 
# 409 "include/turtle.h"
                                                                                            / prez)) * yfact);
    }
    glEnd();
}


void turtleSquareRender(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xfact, double yfact) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(0x0006);
    glVertex2d(x1 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y2 * yfact);
    glVertex2d(x1 * xfact, y2 * yfact);
    glEnd();
}


void turtleTriangleRender(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xfact, double yfact) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(0x0004);
    glVertex2d(x1 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y2 * yfact);
    glVertex2d(x3 * xfact, y3 * yfact);
    glEnd();
}


void turtleQuadRender(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xfact, double yfact) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(0x0006);
    glVertex2d(x1 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y2 * yfact);
    glVertex2d(x3 * xfact, y3 * yfact);
    glVertex2d(x4 * xfact, y4 * yfact);
    glEnd();
}


void turtleTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h');
    list_append(turtle.penPos, (unitype) y2, 'd');

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h');
    list_append(turtle.penPos, (unitype) 0, 'd');
}

void turtleTriangleColor(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 66, 'h');
    list_append(turtle.penPos, (unitype) y2, 'd');

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) r, 'd');
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h');
    list_append(turtle.penPos, (unitype) 0, 'd');
}


void turtleQuad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y2, 'd');

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y4, 'd');
}

void turtleQuadColor(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y2, 'd');

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) r, 'd');
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y4, 'd');
}


void turtleRectangle(double x1, double y1, double x2, double y2) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y1, 'd');

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y2, 'd');
}

void turtleRectangleColor(double x1, double y1, double x2, double y2, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y1, 'd');

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) r, 'd');
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h');
    list_append(turtle.penPos, (unitype) y2, 'd');
}


void turtleCircle(double x, double y, double radius) {
    list_append(turtle.penPos, (unitype) x, 'd');
    list_append(turtle.penPos, (unitype) y, 'd');
    list_append(turtle.penPos, (unitype) radius, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 64, 'h');
    list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
}


void turtle3DTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) z1, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 130, 'h');
    list_append(turtle.penPos, (unitype) x2, 'd');

    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) z2, 'd');
    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) z3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) 130, 'h');
    list_append(turtle.penPos, (unitype) 0, 'd');
}


void turtlePerspective(double x, double y, double z, double *xOut, double *yOut) {
# 687 "include/turtle.h"
}


void turtleUpdate() {


    int8_t changed = 0;
    uint32_t len = turtle.penPos -> length;
    unitype *ren = turtle.penPos -> data;
    int8_t *renType = turtle.penPos -> type;
    uint64_t oldHash = turtle.penHash;
    turtle.penHash = 0;
    for (uint32_t i = 0; i < len; i++) {
        turtle.penHash += (uint64_t) turtle.penPos -> data[i].p;
    }
    if (len != turtle.lastLength || oldHash != turtle.penHash) {
        changed = 1;
        turtle.lastLength = len;
    }
    if (changed) {
        double xfact = (turtle.bounds[2] - turtle.bounds[0]) / 2;
        double yfact = (turtle.bounds[3] - turtle.bounds[1]) / 2;
        xfact = 1 / xfact;
        yfact = 1 / yfact;
        double lastSize = -1;
        double lastPrez = -1;
        double precomputedLog = 5;
        glad_glClear(0x00004000 | 0x00000100);
        for (int32_t i = 0; i < (int32_t) len; i += 9) {
            if (renType[i] == 'd') {
                switch (ren[i + 7].h) {
                case 0:
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                break;
                case 1:
                    turtleSquareRender(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                break;
                case 2:
                    turtleTriangleRender(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                break;
                case 5:
                    if (i - 9 < 0 || renType[i - 9] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                    }
                break;
                default:
                break;
                }
                if (i + 18 < (int32_t) len && renType[i + 9] == 'd' && ren[i + 7].h < 64 && (ren[i + 7].h == 4 || ren[i + 7].h == 5 || (fabs(ren[i].d - ren[i + 9].d) > ren[i + 2].d / 2 || fabs(ren[i + 1].d - ren[i + 10].d) > ren[i + 2].d / 2))) {
                    double dir = atan((ren[i + 9].d - ren[i].d) / (ren[i + 1].d - ren[i + 10].d));
                    double sinn = sin(dir + 
# 747 "include/turtle.h" 3
                                           3.14159265358979323846 
# 747 "include/turtle.h"
                                                / 2);
                    double coss = cos(dir + 
# 748 "include/turtle.h" 3
                                           3.14159265358979323846 
# 748 "include/turtle.h"
                                                / 2);
                    turtleQuadRender(ren[i].d + ren[i + 2].d * sinn, ren[i + 1].d - ren[i + 2].d * coss, ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i].d - ren[i + 2].d * sinn, ren[i + 1].d + ren[i + 2].d * coss, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    if ((ren[i + 7].h == 4 || ren[i + 7].h == 5) && i + 18 < (int32_t) len && renType[i + 18] == 'd') {
                        double dir2 = atan((ren[i + 18].d - ren[i + 9].d) / (ren[i + 10].d - ren[i + 19].d));
                        double sinn2 = sin(dir2 + 
# 752 "include/turtle.h" 3
                                                 3.14159265358979323846 
# 752 "include/turtle.h"
                                                      / 2);
                        double coss2 = cos(dir2 + 
# 753 "include/turtle.h" 3
                                                 3.14159265358979323846 
# 753 "include/turtle.h"
                                                      / 2);
                        turtleTriangleRender(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d + ren[i + 11].d * sinn2, ren[i + 10].d - ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                        turtleTriangleRender(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d - ren[i + 11].d * sinn2, ren[i + 10].d + ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    }
                } else {
                    if (ren[i + 7].h == 4 && i > 8 && renType[i - 8] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                    }
                    if (ren[i + 7].h == 5 && i > 8) {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                    }
                }
                if (ren[i + 7].h == 64) {
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                }
                if (ren[i + 7].h == 66) {
                    turtleTriangleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 67) {
                    turtleQuadRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 11].d, ren[i + 17].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 128) {

                }
                if (ren[i + 7].h == 129) {

                }
                if (ren[i + 7].h == 130) {
                    turtlePerspective(ren[i].d, ren[i + 1].d, ren[i + 2].d, &ren[i].d, &ren[i + 1].d);
                    turtlePerspective(ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, &ren[i + 8].d, &ren[i + 9].d);
                    turtlePerspective(ren[i + 11].d, ren[i + 12].d, ren[i + 13].d, &ren[i + 11].d, &ren[i + 12].d);
                    turtleTriangleRender(ren[i].d, ren[i + 1].d, ren[i + 8].d, ren[i + 9].d, ren[i + 11].d, ren[i + 12].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 131) {

                }
            }
        }
        glfwSwapBuffers(turtle.window);
    }
    glfwPollEvents();
    if (glfwWindowShouldClose(turtle.window)) {
        turtle.close = 1;
        if (turtle.popupClose) {
            glfwTerminate();
        }

    }
}


void turtleMainLoop() {
    while (turtle.close == 0) {
        turtleUpdate();
    }
}


void turtleFree() {
    list_free(turtle.keyPressed);
    list_free(turtle.penPos);
}
