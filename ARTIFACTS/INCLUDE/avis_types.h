#ifndef AVIS_TYPES_H
#define AVIS_TYPES_H

#include <stdint.h>
#include <stddef.h>

#define AVIS_STR16   16
#define AVIS_STR32   32
#define AVIS_STR64   64
#define AVIS_STR128  128
#define AVIS_STR256  256

#define AVIS_GUID_SIZE 16
#define AVIS_HASH256   32

#define AVIS_MAGIC_FVS 0x46565331  /* "FVS1" */

typedef enum
{
    AVIS_OK = 0,
    AVIS_ERROR,
    AVIS_INVALID_ARGUMENT,
    AVIS_IO_ERROR,
    AVIS_HASH_MISMATCH,
    AVIS_NOT_FOUND

} AVIS_Result;

#endif
