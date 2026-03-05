#include <stdlib.h>
#include <string.h>

#include "../include/avis_security.h"

AVIS_Result avis_generate_guid(uint8_t guid[16])
{
    for(int i=0;i<16;i++)
        guid[i] = rand() % 255;

    return AVIS_OK;
}


AVIS_Result avis_hash_sha256(
        const void *data,
        size_t size,
        uint8_t hash[32])
{
    /* placeholder for real SHA256 implementation */

    memset(hash,0,32);
    return AVIS_OK;
}
