#ifndef __PTILE_H
#define __PTILE_H

typedef unsigned long long PTile;

////////////////////////////////////////////////////////////////////////////////
// Properties of PTiles

bool is_pf_positive(PTile x);
bool is_pf_negative(PTile x);
bool is_pf_inverted(PTile x);
bool is_pf_exact(PTile x);
bool is_pf_ulp(PTile x);
bool is_pf_inf(PTile x);
bool is_pf_zero(PTile x);

////////////////////////////////////////////////////////////////////////////////
// iteration tools

PTile next(PTile x);
PTile prev(PTile x);
PTile glb(PTile x);
PTile lub(PTile x);
PTile upper_ulp(PTile x);
PTile lower_ulp(PTile x);

////////////////////////////////////////////////////////////////////////////////
// synthesis and decomposition tools
PTile    pf_synth(bool negative, bool inverted, long long epoch, unsigned long long lattice);
long long pf_epoch(PTile value);
unsigned long long pf_lattice(PTile value);

////////////////////////////////////////////////////////////////////////////////
// math tools
PTile pf_additiveinverse(PTile val);
PTile pf_multiplicativeinverse(PTile val);

////////////////////////////////////////////////////////////////////////////////
// SPECIAL DEFINES

//explicit type conversions.
#define __s(PTileval) (long long)(PTileval)            //converts PTileval to a signed integer
#define __u(PTileval) (unsigned long long)(PTileval)   //converts PTileval to an unsigned integer
#define __p(intval) (PTile)(intval)                     //converts integer value to a PTile.

//inline-able constant values.
#define __zero  (0x0000000000000000U)
#define __inf   (0x8000000000000000U)
#define __one   (0x4000000000000000U)
#define __mask1 (0x7FFFFFFFFFFFFFFFU)         //masks out all but the sign bit.
#define __mask2 (0x3FFFFFFFFFFFFFFFU)         //masks out all but the sign and inverse bits

//special overflow constants which are dependent on the current environment.
#define __many  (__inf - PENV->increment)
#define __nmany (__inf + PENV->increment)
#define __few   (__zero + PENV->increment)
#define __nfew  (__zero - PENV->increment)

#endif
