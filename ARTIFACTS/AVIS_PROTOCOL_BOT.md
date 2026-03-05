
# 📄 AVIS_PROTOCOL_BOT.md — Robot-optimized template

```text
#AVIS_LAW_IDENTITY [AVIS-LAW-IDENTITY] Disk Volume:<DISK_VOLUME>; Drive Letter:<DRIVE_LETTER>; File System:<FILE_SYSTEM>; Project Name:<PROJECT_NAME>; Project Class:<PROJECT_CLASS>; Repository:<REPOSITORY>; Project Owner:<PROJECT_OWNER>; Engine Layer:<ENGINE_LAYER>; Build System:<BUILD_SYSTEM>; Application Role:<APPLICATION_ROLE>

#AVIS_LAW_FILE_IDENTITY [AVIS-LAW-FILE-ID] File Name:<FILE_NAME>; Version:<VERSION>; Language:<LANGUAGE>; Module Type:<MODULE_TYPE>; Artifact Class:<ARTIFACT_CLASS>; Magic Signature:<MAGIC_SIGNATURE>; Hash Method:<HASH_METHOD>; File GUID:<FILE_GUID>; Parent GUID:<PARENT_GUID>

#AVIS_LAW_DISK [AVIS-LAW-DISK] Absolute Path:<ABSOLUTE_PATH>; Relative Path:<RELATIVE_PATH>; Workspace Root:<WORKSPACE_ROOT>; Volume Name:<VOLUME_NAME>; Drive Letter:<DRIVE_LETTER>

#AVIS_LAW_SECURITY [AVIS-LAW-SECURITY] Hash Validation:<ENABLED/DISABLED>; GUID Validation:<ENABLED/DISABLED>; Path Normalization:<ENABLED/DISABLED>; Metadata Verification:<ENABLED/DISABLED>

#AVIS_LAW_SNAPSHOT_CREATE [AVIS-LAW-SNAPSHOT-OPERATIONS] Status:<STATUS>; Verification:<VERIFICATION_STATUS>; Snapshot GUID:<SNAPSHOT_GUID>

#AVIS_LAW_LINEAGE [AVIS-LAW-LINEAGE] ROOT PROJECT-><PROJECT_NAME>-><MODULE>-><SUBMODULE>-><ARTIFACT>
```
---
```

### ✅ Features of _BOT version:

1. **One LAW per line**, machine-readable.
2. All fields as `<PLACEHOLDER>` for robots to fill automatically.
3. Easy to parse with regex or line-by-line scanners.
4. Preserves `#AVIS_LAW_*` and `[AVIS-LAW-*]` tags for automated compliance.

```
