#ifndef AVIS_SECURITY_H
#define AVIS_SECURITY_H

#include "avis_types.h"

AVIS_Result avis_generate_guid(uint8_t guid[16]);

AVIS_Result avis_hash_sha256(
        const void *data,
        size_t size,
        uint8_t hash[32]);

AVIS_Result avis_verify_hash(
        const uint8_t a[32],
        const uint8_t b[32]);

#endif
