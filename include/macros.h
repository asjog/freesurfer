/*
  @(#)macros.h  1.9
  12/12/95
*/
/*------------------------------------------------------------------------
      File Name:  macros.h

         Author:  Bruce Fischl

        Created:  Jan. 1994

    Description:  

------------------------------------------------------------------------*/
#ifndef MACROS_H
#define MACROS_H

#include "const.h"
#include "utils.h"
#include "math.h"

#ifndef UCHAR
#define UCHAR        unsigned char
#endif

#ifndef UINT
#define UINT         unsigned int
#endif

#define ISOPTION(c)  ((c) == '-')

/* these are defined in hipl_format.h */
#ifndef MIN
#define MIN(a,b)     ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)     ((a) >= (b) ? (a) : (b))
#endif

#define EVEN(n)      ((((n) / 2) * 2) == n)
#define ODD(n)       (!EVEN(n))

#define RADIANS(deg) (2.0 * PI * (double)(deg)) / (360.0)
#define DEGREES(rad) ((360.0 * (double)(rad)) / (2.0 * PI))
#define NDEGREES(rad) (DEGREES(normAngle(rad)))

#if 0
#define FZERO(f)     (fabs(f) < 0.0000000001)
#else
#define FZERO(f)     (iszero(f))
#endif

#define FEQUAL(f1,f2) (FZERO(f1-f2))

#ifndef SQR
#define SQR(a)   ((a)*(a))
#endif

#define FSIGN(f)   (FZERO(f) ? 0.0f : (f < 0.0f) ? -1.0f : 1.0f)
#include <stdlib.h>
#include <string.h>

#define STRALLOC(str)   ((char *)calloc(strlen(str)+1, sizeof(char)))
#define STRCPALLOC(str) strcpy(STRALLOC(str), str)

#endif
