#ifndef __QSIM_TYPES_HH
#define __QSIM_TYPES_HH

/*
   qsimtypes.hh

   Generic data types which are classes.  Used for passing IO
   data around and useful enumerations
*/

#if !defined(__CINT__) && !defined(__CLING__)
#include <G4Types.hh>
#endif

#define __RUNSTR_LEN 255
#define __MAXFILE_LEN 1048576 // MB file size limit

#include "TTimeStamp.h"

enum SampType_t { kActiveTargetVolume, kAllTargetVolumes };

struct filedata_t {
    char filename[__RUNSTR_LEN];
    char hashsum[__RUNSTR_LEN];
    TTimeStamp timestamp;
};

#endif//__QSIM_TYPES_HH
