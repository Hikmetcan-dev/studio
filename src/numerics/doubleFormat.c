/* doubleFormat.c
 * Feb 2020: Formatting routines for displaying doubles
 *
 * N.B.: in general these routines follow a two-step process:
 *   1. Get (i.e. compute) the digits to display.  Trailing zeros are
 *      not included, so you can get fewer digits than requested.  In
 *      round-trip mode the number of digits requested is irrelevant
 *      so you can get more digits than requested.  In any case, the
 *      digits come back as a buffer of digits and an offset decPt.
 *      To format, start a decimal point on the left of the string and
 *      shift it right decPt positions.  So 1200.0 comes back as "12"
 *      with decPt=4.   .00401 comes back as "401" with decPt=-2.
 *     a. Sometimes you specify the total number of digits you want,
 *        aka the precision or the number of significant figures.  I
 *        call this nSigFigs.
 *     b. Sometimes you specify the digits right of the decimal
 *        point.  I call this nDecimals.
 *   2. Format the digits.  Clearly we cannot display fewer digits
 *      than returned.  In some cases we pad with zeros.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "dtoaLoc.h"
#include "doubleFormat.h"

/* patch up zero case
 * if 0 == *nDigits, we have a zero
 */
static void zeroPatch(char digBuf[], int *nDigits, int *decPt)
{
  if (0 == *nDigits) {
    digBuf[0] = '0';
    digBuf[1] = '\0';
    *nDigits = 1;
    *decPt = 1;
  }
} /* zeroPatch */

/* convert base-10 digits and implied decimal position into e-format string
 * we assume outBuf is long enough: a size of 32 is plenty
 * if zeroPad, we zero-pad on the right as necessary to show nDigits total
 */
static void dig2Exp(const char digits[], int nDigits, int decPt, int isNeg,
                    int nSigFigs, int zeroPad,
                    char outBuf[], int *bufLen)
{
  char *d;
  const char *s;
  int e, k;

  assert(decPt <= 308);
  assert(decPt >= -307);
  assert(nDigits >= 1);
  assert(nDigits <= 20);

  k = nSigFigs - nDigits;
  d = outBuf;
  if (isNeg)
    *d++ = '-';
  s = digits;
  *d++ = *s++;
  if (*s || (zeroPad && (k > 0)))
    *d++ = '.';
  while (*s)
    *d++ = *s++;
  if (zeroPad && (k > 0)) {
    (void) memset (d, '0', k);
    d += k;
  }
  *d++ = 'e';
  e = decPt-1;
  if (e < 0) {
    e *= -1;
    *d++ = '-';
  }
#if 1
  else
    *d++ = '+';
#endif
  *bufLen = (int)(d-outBuf);
  if (e <= 99) {
    sprintf (d, "%02d", e);
    *bufLen += 2;
  }
  else {
    sprintf (d, "%d", e);
    *bufLen += 3;
  }
  return;
} /* dig2Exp */

/* convert base-10 digits and implied decimal position into f-format string
 * we assume outBuf is long enough: a size of 32 is plenty
 * if zeroPad, we zero-pad on the right as necessary to show nDecimals digits
 * to the right of the decimal point
 */
static void dig2Fixed (const char digits[], int nDigits, int decPt, int isNeg,
                       int nDecimals, int zeroPad,
                       char outBuf[], int *bufLen)
{
  char *p;
  int k;

  assert(nDigits >= 1);
  assert(nDigits <= 21);
  assert(decPt <= 22);          /* at some point we shift to e-format */
  assert(decPt >= -18);         /* ditto */
  p = outBuf;
  *bufLen = 0;
  if (isNeg)
    *p++ = '-';
  if (decPt > nDigits) {
    (void) memcpy (p, digits, nDigits);
    (void) memset (p+nDigits, '0', decPt - nDigits);
    p += decPt;
    if (zeroPad && (nDecimals > 0)) {
      *p++ = '.';
      (void) memset (p, '0', nDecimals);
      p += nDecimals;
    }
    *p = '\0';
    *bufLen = (int)(p-outBuf);
  }
  else if (decPt == nDigits) {
    (void) strcpy (p, digits);
    p += decPt;
    if (zeroPad && (nDecimals > 0)) {
      *p++ = '.';
      (void) memset (p, '0', nDecimals);
      p += nDecimals;
      *p = '\0';
    }
    *bufLen = (int)(p-outBuf);
  }
  else if (decPt > 0) {
    k = nDigits - decPt;         /* nonzero digits past the decimal point */
    (void) memcpy (p, digits, decPt);
    p += decPt;
    *p++ = '.';
    (void) strcpy (p, digits+decPt);
    p += k;
    if (zeroPad && (k < nDecimals)) {
      k = nDecimals - k;        /* number of zeros to pad */
      (void) memset (p, '0', k);
      p += k;
      *p = '\0';
    }
    *bufLen = (int)(p-outBuf);
  }
  else if (0 == decPt) {
    *p++ = '0';
    *p++ = '.';
    (void) strcpy (p, digits);
    p += nDigits;
    if (zeroPad && (nDigits < nDecimals)) {
      k = nDecimals - nDigits;        /* number of zeros to pad */
      (void) memset (p, '0', k);
      p += k;
    }
    *p = '\0';
    *bufLen = (int)(p-outBuf);
  }
  else {                        /* decPt < 0 */
    *p++ = '0';
    *p++ = '.';
    (void) memset (p, '0', -decPt);
    p -= decPt;
    (void) memcpy (p, digits, nDigits);
    p += nDigits;
    if (zeroPad) {
      k = nDigits - decPt;
      if (k < nDecimals) {      /* need to do some padding */
        k = nDecimals - k;
        (void) memset (p, '0', k);
        p += k;
      }
    }
    *p = '\0';
    *bufLen = (int)(p-outBuf);
  }
  return;
}

#define DTOA_MODE_RT   0   /* shortest string that gives round-trip behavior */
#define DTOA_MODE_EFMT 2   /* returns nDigits significant digits */
#define DTOA_MODE_FFMT 3   /* returns nDecimals digits past the decimal point */

static double pow10[] = {
      1,        1e01,     1e02,     1e03,     1e04,
      1e05,     1e06,     1e07,     1e08,     1e09,
      1e10,     1e11,     1e12,     1e13,     1e14,
      1e15,     1e16,     1e17,     1e18,     1e19,
      1e20,     1e21};

/* x2Fixed: use f-fmt (rounded to nDecimals decimal places) until values
 * get too large and the output gets too long: then shift to e-format.
 * if squeeze is true, squeeze out trailing zeros.
 * the output is placed in outBuf, and *outLen gives the length of this buffer
 * On error, NULL is returned.
 */
char *x2fixed (double v, int nDecimals, int squeeze, char outBuf[], int *outLen)
{
  char *p, *pEnd;
  char digBuf[32];              /* buffer for the digits */
  int nDigits, decPt, isNeg;
  double vMax;

  *outLen = 0;
  if (nDecimals > 17)
    nDecimals = 17;
  if (nDecimals < 0)
    nDecimals = 0;
  assert(isfinite(v));

  /* compute vMax := 10^21 / 10^(nDecimals) = 10^(21-nDecimals) */
  /* vMax = 1e21 / pow10[nDecimals]; */
  vMax = pow10[21-nDecimals];
  if (fabs(v) > vMax) {
    p = dtoaLoc (v, DTOA_MODE_RT, 1,
                 digBuf, sizeof(digBuf), &decPt, &isNeg, &pEnd);
    if (NULL == p) {
      assert(p);
    }
    else {
      nDigits = (int) (pEnd - p);
      dig2Exp (digBuf, nDigits, decPt, isNeg, 17, !squeeze, outBuf, outLen);
    }
    return outBuf;
  }
  decPt = -10;
  p = dtoaLoc (v, DTOA_MODE_FFMT, nDecimals,
               digBuf, sizeof(digBuf), &decPt, &isNeg, &pEnd);
  if (NULL == p) {
    assert(p);
  }
  else {
    nDigits = (int)(pEnd - p);
    zeroPatch(digBuf, &nDigits, &decPt);
    dig2Fixed (digBuf, nDigits, decPt, isNeg, nDecimals, !squeeze, outBuf, outLen);
  }
  return outBuf;
} /* x2fixed */

/* x2efmt: use e-format, with nSigFigs digits of precision
 * if nSigFigs <= 0, we do shortest round-trip digits
 * if squeeze is true, squeeze out trailing zeros.
 * the output is placed in outBuf, and *outLen gives the length of this buffer
 * On error, NULL is returned.
 */
char *x2efmt (double v, int nSigFigs, int squeeze, char outBuf[], int *outLen)
{
  char *p, *pEnd;
  char digBuf[32];              /* buffer for the digits */
  int nDigits, decPt, isNeg;
  int mode = DTOA_MODE_EFMT;

  *outLen = 0;
  if (nSigFigs > 17)
    nSigFigs = 17;
  if (nSigFigs <= 0) {
    nSigFigs = 0;
    mode = DTOA_MODE_RT;
  }
  assert(isfinite(v));
  p = dtoaLoc (v, mode, nSigFigs,
               digBuf, sizeof(digBuf), &decPt, &isNeg, &pEnd);
  assert(p);
  nDigits = (int) (pEnd - p);
  zeroPatch(digBuf, &nDigits, &decPt);
  dig2Exp (digBuf, nDigits, decPt, isNeg, nSigFigs, !squeeze, outBuf, outLen);

  return outBuf;
} /* x2efmt */

/* x2gfmt: use g-format, with nSigFigs digits of precision
 * if nSigFigs <= 0, we do shortest round-trip digits
 * if squeeze is true, squeeze out trailing zeros.
 * the output is placed in outBuf, and *outLen gives the length of this buffer
 * On error, NULL is returned.
 */
char *x2gfmt (double v, int nSigFigs, int squeeze, char outBuf[], int *outLen)
{
  char *p, *pEnd;
  char digBuf[32];              /* buffer for the digits */
  int nDigits, decPt, isNeg;
  int sigFigs;                  /* max(digits returned, digits requested) */
  int mode = DTOA_MODE_EFMT;

  *outLen = 0;
  if (nSigFigs > 17)
    nSigFigs = 17;
  if (nSigFigs <= 0) {
    nSigFigs = 0;
    mode = DTOA_MODE_RT;
  }
  assert(isfinite(v));
  p = dtoaLoc (v, mode, nSigFigs,
               digBuf, sizeof(digBuf), &decPt, &isNeg, &pEnd);
  assert(p);
  nDigits = (int) (pEnd - p);
  zeroPatch(digBuf, &nDigits, &decPt);
  sigFigs = nDigits;
  if (sigFigs < nSigFigs)
    sigFigs = nSigFigs;
  // if ((decPt < -3) || ((decPt-nDigits) >= 1) )
  if ((decPt < -3) || ((decPt-sigFigs) >= 1) )
    dig2Exp (digBuf, nDigits, decPt, isNeg, nSigFigs, !squeeze, outBuf, outLen);
  else
    dig2Fixed (digBuf, nDigits, decPt, isNeg, nSigFigs-decPt, !squeeze, outBuf, outLen);

  return outBuf;
} /* x2gfmt */
