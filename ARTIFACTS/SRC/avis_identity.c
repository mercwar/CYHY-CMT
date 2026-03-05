#include <stdio.h>
#include <string.h>

#include "../include/avis_identity.h"

void avis_identity_init(AVIS_ArtifactIdentity *id)
{
    memset(id, 0, sizeof(*id));

    strcpy(id->disk.volume_name, "Project_Source");
    strcpy(id->disk.drive_letter, "B:\\");
    strcpy(id->disk.file_system, "NTFS");

    strcpy(id->project.project_name, "AVIS CORE");
    strcpy(id->project.build_system, "AVIS SCANNER");

    strcpy(id->file.magic_signature, "AVIS-FVS");

    id->security.hash_validation = 1;
    id->security.guid_validation = 1;
}


AVIS_Result avis_identity_print(const AVIS_ArtifactIdentity *id)
{
    if (!id)
        return AVIS_INVALID_ARGUMENT;

    printf("FILE: %s\n", id->file.file_name);
    printf("PROJECT: %s\n", id->project.project_name);
    printf("PATH: %s\n", id->disk.absolute_path);

    return AVIS_OK;
}
