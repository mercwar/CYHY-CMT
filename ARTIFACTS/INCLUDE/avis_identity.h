#ifndef AVIS_IDENTITY_H
#define AVIS_IDENTITY_H

#include "avis_types.h"

typedef struct
{
    char volume_name[AVIS_STR64];
    char drive_letter[8];
    char file_system[AVIS_STR32];

    char absolute_path[AVIS_STR256];
    char relative_path[AVIS_STR256];
    char workspace_root[AVIS_STR128];

} AVIS_DiskIdentity;


typedef struct
{
    char project_name[AVIS_STR64];
    char project_class[AVIS_STR64];
    char repository[AVIS_STR128];
    char owner[AVIS_STR64];

    char engine_layer[AVIS_STR64];
    char build_system[AVIS_STR64];

    char application_role[AVIS_STR256];

} AVIS_ProjectIdentity;


typedef struct
{
    char file_name[AVIS_STR128];
    char file_id[AVIS_STR128];

    char version[AVIS_STR16];

    char language[AVIS_STR16];
    char module_type[AVIS_STR64];

    char artifact_class[AVIS_STR64];
    char magic_signature[AVIS_STR32];

    char hash_method[AVIS_STR16];
    uint8_t file_hash[AVIS_HASH256];

    char build_signature[AVIS_STR64];

    uint8_t unique_guid[AVIS_GUID_SIZE];
    uint8_t parent_guid[AVIS_GUID_SIZE];

} AVIS_FileIdentity;


typedef struct
{
    char platform[AVIS_STR64];
    char architecture[AVIS_STR32];

    char compiler_primary[AVIS_STR32];
    char compiler_secondary[AVIS_STR32];

    char standard[AVIS_STR16];
    char flags[AVIS_STR256];

} AVIS_BuildInfo;


typedef struct
{
    int hash_validation;
    int guid_validation;
    int path_normalization;
    int metadata_verification;

} AVIS_SecurityModel;


typedef struct
{
    AVIS_DiskIdentity disk;
    AVIS_ProjectIdentity project;
    AVIS_FileIdentity file;
    AVIS_BuildInfo build;
    AVIS_SecurityModel security;

} AVIS_ArtifactIdentity;


/* API */

void avis_identity_init(AVIS_ArtifactIdentity *id);

AVIS_Result avis_identity_print(const AVIS_ArtifactIdentity *id);

AVIS_Result avis_identity_serialize(
        const AVIS_ArtifactIdentity *id,
        const char *path);

AVIS_Result avis_identity_load(
        AVIS_ArtifactIdentity *id,
        const char *path);

#endif
