/*===========================================================
   AVIS LAW Markdown Parser — Full Struct Mapper
   FILE_ID: avis_law_full_parser.c
   VERSION: 1.0.0
   DEPENDENCIES: stdio.h, stdlib.h, string.h
===========================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_BLOCKS 32

/*------------------------ AVIS Structs ------------------------*/

// Identity Initialization
typedef struct {
    char disk_volume[64];
    char drive_letter[8];
    char file_system[16];
    char project_name[64];
    char project_class[64];
    char repository[64];
    char project_owner[32];
    char engine_layer[64];
    char build_system[64];
    char application_role[128];
} AVIS_Identity;

// File Identity Report
typedef struct {
    char file_name[64];
    char file_id[64];
    char version[16];
    char language[16];
    char module_type[64];
    char artifact_class[64];
    char magic_signature[32];
    char hash_method[16];
    char file_guid[64];
    char parent_guid[64];
    char build_signature[64];
} AVIS_FileIdentity;

// Disk Location
typedef struct {
    char absolute_path[256];
    char relative_path[128];
    char workspace_root[64];
    char volume_name[64];
    char drive_letter[8];
    char file_system[16];
} AVIS_Disk;

// Build Environment
typedef struct {
    char target_platform[32];
    char architecture[16];
    char primary_compiler[32];
    char secondary_compiler[32];
    char c_standard[8];
    char compiler_flags[256];
} AVIS_Build;

// Security Model
typedef struct {
    int hash_validation;
    int guid_validation;
    int path_normalization;
    int metadata_verification;
} AVIS_Security;

// Snapshot Engine
typedef struct {
    char source_file[256];
    char snapshot_file[256];
    char snapshot_guid[64];
    char timestamp[32];
    unsigned long file_size;
    char hash_method[16];
    char status[32];
} AVIS_Snapshot;

// Artifact Lineage
typedef struct {
    char lineage_map[1024];
} AVIS_Lineage;

// Project Scanner
typedef struct {
    int files_scanned;
    int artifacts_detected;
    int modules_indexed;
    int snapshots_found;
    char index_status[32];
} AVIS_Scanner;

/*------------------------ Generic LAW Block ------------------------*/
typedef struct {
    char section[64];
    char content[4096];
} AVIS_LAW_BLOCK;

/*------------------------ Helper Functions ------------------------*/
// Extract simple "Field : Value" lines
void parse_field(const char *content, const char *field, char *dest, size_t size) {
    char *pos = strstr(content, field);
    if (pos) {
        pos += strlen(field);
        while (*pos == ' ' || *pos == ':') pos++;
        char *end = strchr(pos, '\n');
        if (!end) end = pos + strlen(pos);
        size_t len = end - pos;
        if (len >= size) len = size - 1;
        strncpy(dest, pos, len);
        dest[len] = '\0';
    }
}

// Convert ENABLED/disabled to int
int parse_enabled(const char *content, const char *field) {
    char value[16] = {0};
    parse_field(content, field, value, sizeof(value));
    return (strcasecmp(value, "ENABLED") == 0) ? 1 : 0;
}

/*------------------------ Main Parser ------------------------*/
int main() {
    FILE *fp = fopen("SPEC.md", "r");
    if (!fp) { perror("Failed to open SPEC.md"); return 1; }

    AVIS_LAW_BLOCK blocks[MAX_BLOCKS];
    int block_count = 0;
    char line[MAX_LINE];
    int in_block = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "#AVIS_LAW_", 10) == 0) {
            if (block_count >= MAX_BLOCKS) break;
            in_block = 1;
            sscanf(line, "#AVIS_LAW_%63s", blocks[block_count].section);
            blocks[block_count].content[0] = '\0';
            continue;
        }
        if (in_block) {
            if (strncmp(line, "```", 3) == 0) {
                in_block = 0;
                block_count++;
                continue;
            }
            strncat(blocks[block_count].content, line, sizeof(blocks[block_count].content) - strlen(blocks[block_count].content) - 1);
        }
    }
    fclose(fp);

    /*------------------------ Map Blocks ------------------------*/
    for (int i = 0; i < block_count; i++) {

        if (strcmp(blocks[i].section, "IDENTITY") == 0) {
            AVIS_Identity id;
            parse_field(blocks[i].content, "Disk Volume", id.disk_volume, sizeof(id.disk_volume));
            parse_field(blocks[i].content, "Drive Letter", id.drive_letter, sizeof(id.drive_letter));
            parse_field(blocks[i].content, "File System", id.file_system, sizeof(id.file_system));
            parse_field(blocks[i].content, "Project Name", id.project_name, sizeof(id.project_name));
            parse_field(blocks[i].content, "Project Class", id.project_class, sizeof(id.project_class));
            parse_field(blocks[i].content, "Repository", id.repository, sizeof(id.repository));
            parse_field(blocks[i].content, "Project Owner", id.project_owner, sizeof(id.project_owner));
            parse_field(blocks[i].content, "Engine Layer", id.engine_layer, sizeof(id.engine_layer));
            parse_field(blocks[i].content, "Build System", id.build_system, sizeof(id.build_system));
            parse_field(blocks[i].content, "Application Role", id.application_role, sizeof(id.application_role));

            printf("=== AVIS IDENTITY ===\n%s\n%s\n%s\n\n", id.project_name, id.project_class, id.application_role);
        }

        else if (strcmp(blocks[i].section, "FILE_IDENTITY") == 0) {
            AVIS_FileIdentity f;
            parse_field(blocks[i].content, "File Name", f.file_name, sizeof(f.file_name));
            parse_field(blocks[i].content, "File ID", f.file_id, sizeof(f.file_id));
            parse_field(blocks[i].content, "Version", f.version, sizeof(f.version));
            parse_field(blocks[i].content, "Language", f.language, sizeof(f.language));
            parse_field(blocks[i].content, "Module Type", f.module_type, sizeof(f.module_type));
            parse_field(blocks[i].content, "Artifact Class", f.artifact_class, sizeof(f.artifact_class));
            parse_field(blocks[i].content, "Magic Signature", f.magic_signature, sizeof(f.magic_signature));
            parse_field(blocks[i].content, "Hash Method", f.hash_method, sizeof(f.hash_method));
            parse_field(blocks[i].content, "File GUID", f.file_guid, sizeof(f.file_guid));
            parse_field(blocks[i].content, "Parent GUID", f.parent_guid, sizeof(f.parent_guid));
            parse_field(blocks[i].content, "Build Signature", f.build_signature, sizeof(f.build_signature));

            printf("=== AVIS FILE_IDENTITY ===\n%s | %s\n\n", f.file_name, f.version);
        }

        else if (strcmp(blocks[i].section, "DISK") == 0) {
            AVIS_Disk d;
            parse_field(blocks[i].content, "Absolute Path", d.absolute_path, sizeof(d.absolute_path));
            parse_field(blocks[i].content, "Relative Path", d.relative_path, sizeof(d.relative_path));
            parse_field(blocks[i].content, "Workspace Root", d.workspace_root, sizeof(d.workspace_root));
            parse_field(blocks[i].content, "Volume Name", d.volume_name, sizeof(d.volume_name));
            parse_field(blocks[i].content, "Drive Letter", d.drive_letter, sizeof(d.drive_letter));
            parse_field(blocks[i].content, "File System", d.file_system, sizeof(d.file_system));
        }

        else if (strcmp(blocks[i].section, "BUILD") == 0) {
            AVIS_Build b;
            parse_field(blocks[i].content, "Target Platform", b.target_platform, sizeof(b.target_platform));
            parse_field(blocks[i].content, "Architecture", b.architecture, sizeof(b.architecture));
            parse_field(blocks[i].content, "Primary Compiler", b.primary_compiler, sizeof(b.primary_compiler));
            parse_field(blocks[i].content, "Secondary", b.secondary_compiler, sizeof(b.secondary_compiler));
            parse_field(blocks[i].content, "C Standard", b.c_standard, sizeof(b.c_standard));
            parse_field(blocks[i].content, "Compiler Flags", b.compiler_flags, sizeof(b.compiler_flags));
        }

        else if (strcmp(blocks[i].section, "SECURITY") == 0) {
            AVIS_Security s;
            s.hash_validation       = parse_enabled(blocks[i].content, "Hash Validation");
            s.guid_validation       = parse_enabled(blocks[i].content, "GUID Validation");
            s.path_normalization    = parse_enabled(blocks[i].content, "Path Normalization");
            s.metadata_verification = parse_enabled(blocks[i].content, "Metadata Verification");
        }

        else if (strcmp(blocks[i].section, "SNAPSHOT_CREATE") == 0 ||
                 strcmp(blocks[i].section, "SNAPSHOT_RESTORE") == 0) {
            AVIS_Snapshot snap;
            parse_field(blocks[i].content, "Source File", snap.source_file, sizeof(snap.source_file));
            parse_field(blocks[i].content, "Snapshot File", snap.snapshot_file, sizeof(snap.snapshot_file));
            parse_field(blocks[i].content, "Snapshot GUID", snap.snapshot_guid, sizeof(snap.snapshot_guid));
            parse_field(blocks[i].content, "Timestamp", snap.timestamp, sizeof(snap.timestamp));
            parse_field(blocks[i].content, "File Size", (char*)&snap.file_size, sizeof(snap.file_size));
            parse_field(blocks[i].content, "Hash Method", snap.hash_method, sizeof(snap.hash_method));
            parse_field(blocks[i].content, "Status", snap.status, sizeof(snap.status));
        }

        else if (strcmp(blocks[i].section, "LINEAGE") == 0) {
            AVIS_Lineage l;
            strncpy(l.lineage_map, blocks[i].content, sizeof(l.lineage_map)-1);
        }

        else if (strcmp(blocks[i].section, "SCANNER") == 0) {
            AVIS_Scanner sc;
            parse_field(blocks[i].content, "Files Scanned", (char*)&sc.files_scanned, sizeof(int));
            parse_field(blocks[i].content, "Artifacts Detected", (char*)&sc.artifacts_detected, sizeof(int));
            parse_field(blocks[i].content, "Modules Indexed", (char*)&sc.modules_indexed, sizeof(int));
            parse_field(blocks[i].content, "Snapshots Found", (char*)&sc.snapshots_found, sizeof(int));
            parse_field(blocks[i].content, "Index Status", sc.index_status, sizeof(sc.index_status));
        }
    }

    return 0;
}
