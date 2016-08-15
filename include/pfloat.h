#ifndef __PFLOAT_H
#define __PFLOAT_H

typedef unsigned long long PFloat;

////////////////////////////////////////////////////////////////////////////////
// Properties of PFloats

bool is_pf_positive(PFloat x);
bool is_pf_negative(PFloat x);
bool is_pf_inverted(PFloat x);
bool is_pf_exact(PFloat x);
bool is_pf_ulp(PFloat x);
bool is_pf_inf(PFloat x);
bool is_pf_zero(PFloat x);

////////////////////////////////////////////////////////////////////////////////
// iteration tools

PFloat next(PFloat x);
PFloat prev(PFloat x);
PFloat glb(PFloat x);
PFloat lub(PFloat x);
PFloat upper_ulp(PFloat x);
PFloat lower_ulp(PFloat x);

////////////////////////////////////////////////////////////////////////////////
// synthesis and decomposition tools
PFloat    pf_synth(bool negative, bool inverted, long long epoch, unsigned long long lattice);
long long pf_epoch(PFloat value);
unsigned long long pf_lattice(PFloat value);

////////////////////////////////////////////////////////////////////////////////
// math tools
PFloat pf_additiveinverse(PFloat val);
PFloat pf_multiplicativeinverse(PFloat val);

////////////////////////////////////////////////////////////////////////////////
// SPECIAL DEFINES

//explicit type conversions.
#define __s(pfloatval) (long long)(pfloatval)            //converts pfloatval to a signed integer
#define __u(pfloatval) (unsigned long long)(pfloatval)   //converts pfloatval to an unsigned integer
#define __p(intval) (PFloat)(intval)                     //converts integer value to a pfloat.

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
