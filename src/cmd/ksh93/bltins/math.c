//
// This is based on the arch/.../src/cmd/ksh93/FEATURE/math file generated by
// the Nmake build process. That process, however, takes upwards of 15% of the
// total build time. So This is a cleaned up, simplified, copy of that file.
//
// This assumes that all the functions in a typical UNIX libm library are
// available.
//
#include "config_ast.h"  // IWYU pragma: keep

#include <limits.h>
#include <math.h>

#include "sfio.h"
#include "streval.h"

typedef Sfdouble_t (*Math_f)(Sfdouble_t, ...);

// This used to use `finite()` but that function is deprecated and generates compiler warnings
// on some platforms.
static int local_finite(Sfdouble_t a1) {
    if (isinf(a1)) return 0;  //!OCLINT(constant conditional operator)
    if (isnan(a1)) return 0;  //!OCLINT(constant conditional operator)
    return 1;
}

static Sfdouble_t local_float(Sfdouble_t a1) { return a1; }

static int local_fpclassify(Sfdouble_t a1) {
    return fpclassify(a1);  //!OCLINT(constant conditional operator)
}

static Sfdouble_t local_int(Sfdouble_t a1) {
    if (a1 < LLONG_MIN || a1 > ULLONG_MAX) return 0.0;
    if (a1 < 0) return (Sfdouble_t)((Sflong_t)a1);
    return (Sfdouble_t)((Sfulong_t)a1);
}

static int local_isfinite(Sfdouble_t a1) {
    return isfinite(a1);  //!OCLINT(constant conditional operator)
}

static int local_isgreater(Sfdouble_t a1, Sfdouble_t a2) { return isgreater(a1, a2); }

static int local_isgreaterequal(Sfdouble_t a1, Sfdouble_t a2) { return isgreaterequal(a1, a2); }

static int local_isinf(Sfdouble_t a1) {
    // Some platforms return -1 for -inf and +1 for +inf while others only return +1. Normalize our
    // return value to be zero (not inf) or one (inf).
    return isinf(a1) != 0;  //!OCLINT(constant conditional operator)
}

static int local_isless(Sfdouble_t a1, Sfdouble_t a2) { return isless(a1, a2); }

static int local_islessequal(Sfdouble_t a1, Sfdouble_t a2) { return islessequal(a1, a2); }

static int local_islessgreater(Sfdouble_t a1, Sfdouble_t a2) { return islessgreater(a1, a2); }

static int local_isnan(Sfdouble_t a1) {
    // Some platforms return -1 for -nan and +1 for +nan while others only return +1. Normalize our
    // return value to be zero (not -nan) or one (nan).
    return isnan(a1) != 0;  //!OCLINT(constant conditional operator)
}

static int local_isnormal(Sfdouble_t a1) {
    return isnormal(a1);  //!OCLINT(constant conditional operator)
}

static int local_issubnormal(Sfdouble_t a1) {
    int q = fpclassify(a1);  //!OCLINT(constant conditional operator)
    return q == FP_SUBNORMAL;
}

static int local_isunordered(Sfdouble_t a1, Sfdouble_t a2) { return isunordered(a1, a2); }

static int local_iszero(Sfdouble_t a1) {
    int q = fpclassify(a1);  //!OCLINT(constant conditional operator)
    return q == FP_ZERO;
}

static Sfdouble_t local_j0(Sfdouble_t a1) { return j0(a1); }

static Sfdouble_t local_j1(Sfdouble_t a1) { return j1(a1); }

static Sfdouble_t local_jn(Sfdouble_t a1, Sfdouble_t a2) { return jn(a1, a2); }

static int local_signbit(Sfdouble_t a1) {
    return signbit(a1) != 0;  //!OCLINT(constant conditional operator)
}

static Sfdouble_t local_y0(Sfdouble_t a1) { return y0(a1); }

static Sfdouble_t local_y1(Sfdouble_t a1) { return y1(a1); }

static Sfdouble_t local_yn(Sfdouble_t a1, Sfdouble_t a2) { return yn(a1, a2); }

#if !_lib_expm1l
static Sfdouble_t local_expm1(Sfdouble_t a1) { return expm1(a1); }
#define expm1l local_expm1
#endif

#if !_lib_lgammal
static Sfdouble_t local_lgamma(Sfdouble_t a1) { return lgamma(a1); }
#define lgammal local_lgamma
#endif

#if !_lib_log1pl
static Sfdouble_t local_log1p(Sfdouble_t a1) { return log1p(a1); }
#define log1pl local_log1p
#endif

#if !_lib_log2l
static Sfdouble_t local_log2(Sfdouble_t a1) { return log2(a1); }
#define log2l local_log2
#endif

#if !_lib_remainderl
static Sfdouble_t local_remainder(Sfdouble_t a1, Sfdouble_t a2) { return remainder(a1, a2); }
#define remainderl local_remainder
#endif

#if !_lib_tgammal
static Sfdouble_t local_tgamma(Sfdouble_t a1) { return tgamma(a1); }
#define tgammal local_tgamma
#endif

#if __CYGWIN__
// The sqrtl() function on Cygwin incorrectly returns its input if negative rather than NaN.
static Sfdouble_t local_sqrtl(Sfdouble_t a1) {
    if (a1 < 0.0) return NAN;
    return sqrtl(a1);
}
#else  // __CYGWIN__
#define local_sqrtl sqrtl
#endif  // __CYGWIN__

//
// the first byte is a three-digit octal number <mask><return><argc>:
//
//        <mask>                typed arg bitmask counting from 1
//        <return>        function return type 0:double 1:integer
//        <argc>                number of args
//
// NOTE: swap <mask> and <return> to handle up to 3 typed args instead of just 2
//
const struct mathtab shtab_math[] = {{"\001acos", (Math_f)acosl},
                                     {"\001acosh", (Math_f)acoshl},
                                     {"\001asin", (Math_f)asinl},
                                     {"\001asinh", (Math_f)asinhl},
                                     {"\001atan", (Math_f)atanl},
                                     {"\002atan2", (Math_f)atan2l},
                                     {"\001atanh", (Math_f)atanhl},
                                     {"\001cbrt", (Math_f)cbrtl},
                                     {"\001ceil", (Math_f)ceill},
                                     {"\002copysign", (Math_f)copysignl},
                                     {"\001cos", (Math_f)cosl},
                                     {"\001cosh", (Math_f)coshl},
                                     {"\001erf", (Math_f)erfl},
                                     {"\001erfc", (Math_f)erfcl},
                                     {"\001exp", (Math_f)expl},
                                     {"\001exp2", (Math_f)exp2l},
                                     {"\001expm1", (Math_f)expm1l},
                                     {"\001fabs", (Math_f)fabsl},
                                     {"\001abs", (Math_f)fabsl},
                                     {"\002fdim", (Math_f)fdiml},
                                     {"\011finite", (Math_f)local_finite},
                                     {"\001float", (Math_f)local_float},
                                     {"\001floor", (Math_f)floorl},
                                     {"\003fma", (Math_f)fmal},
                                     {"\002fmax", (Math_f)fmaxl},
                                     {"\002fmin", (Math_f)fminl},
                                     {"\002fmod", (Math_f)fmodl},
                                     {"\011fpclassify", (Math_f)local_fpclassify},
                                     {"\002hypot", (Math_f)hypotl},
                                     {"\011ilogb", (Math_f)ilogbl},
                                     {"\001int", (Math_f)local_int},
                                     {"\011isfinite", (Math_f)local_isfinite},
                                     {"\012isgreater", (Math_f)local_isgreater},
                                     {"\012isgreaterequal", (Math_f)local_isgreaterequal},
                                     {"\011isinf", (Math_f)local_isinf},
                                     {"\012isless", (Math_f)local_isless},
                                     {"\012islessequal", (Math_f)local_islessequal},
                                     {"\012islessgreater", (Math_f)local_islessgreater},
                                     {"\011isnan", (Math_f)local_isnan},
                                     {"\011isnormal", (Math_f)local_isnormal},
                                     {"\011issubnormal", (Math_f)local_issubnormal},
                                     {"\012isunordered", (Math_f)local_isunordered},
                                     {"\011iszero", (Math_f)local_iszero},
                                     {"\001j0", (Math_f)local_j0},
                                     {"\001j1", (Math_f)local_j1},
                                     {"\002jn", (Math_f)local_jn},
                                     {"\042ldexp", (Math_f)ldexpl},
                                     {"\001lgamma", (Math_f)lgammal},
                                     {"\001log", (Math_f)logl},
                                     {"\001log10", (Math_f)log10l},
                                     {"\001log1p", (Math_f)log1pl},
                                     {"\001log2", (Math_f)log2l},
                                     {"\001logb", (Math_f)logbl},
                                     {"\001nearbyint", (Math_f)nearbyintl},
                                     {"\002pow", (Math_f)powl},
                                     {"\002remainder", (Math_f)remainderl},
                                     {"\001rint", (Math_f)rintl},
                                     {"\001round", (Math_f)roundl},
                                     {"\002scalbn", (Math_f)scalbnl},
                                     {"\011signbit", (Math_f)local_signbit},
                                     {"\001sin", (Math_f)sinl},
                                     {"\001sinh", (Math_f)sinhl},
                                     {"\001sqrt", (Math_f)local_sqrtl},
                                     {"\001tan", (Math_f)tanl},
                                     {"\001tanh", (Math_f)tanhl},
                                     {"\001tgamma", (Math_f)tgammal},
                                     {"\001trunc", (Math_f)truncl},
                                     {"\001y0", (Math_f)local_y0},
                                     {"\001y1", (Math_f)local_y1},
                                     {"\002yn", (Math_f)local_yn},
                                     {"", (Math_f)0}};
