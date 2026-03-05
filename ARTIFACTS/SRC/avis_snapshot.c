#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/avis_snapshot.h"
#include "../include/avis_security.h"

AVIS_Result avis_snapshot_create(
        const char *file_path,
        const char *snapshot_path)
{
    FILE *src = fopen(file_path, "rb");
    if (!src)
        return AVIS_IO_ERROR;

    FILE *dst = fopen(snapshot_path, "wb");
    if (!dst)
    {
        fclose(src);
        return AVIS_IO_ERROR;
    }

    AVIS_SnapshotHeader header;

    memset(&header,0,sizeof(header));

    header.magic_id = AVIS_MAGIC_FVS;
    header.timestamp = time(NULL);

    avis_generate_guid(header.snapshot_guid);

    fwrite(&header,sizeof(header),1,dst);

    char buffer[4096];
    size_t r;

    while((r = fread(buffer,1,sizeof(buffer),src)) > 0)
        fwrite(buffer,1,r,dst);

    fclose(src);
    fclose(dst);

    return AVIS_OK;
}
