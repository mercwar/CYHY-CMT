# 📄 AVIS_PROTOCOL_DL.md — Data Lake Compressed Template

```text
#AVIS_LAW_IDENTITY:<DISK_VOLUME>|<DRIVE_LETTER>|<FILE_SYSTEM>|<PROJECT_NAME>|<PROJECT_CLASS>|<REPOSITORY>|<PROJECT_OWNER>|<ENGINE_LAYER>|<BUILD_SYSTEM>|<APPLICATION_ROLE>
#AVIS_LAW_FILE_IDENTITY:<FILE_NAME>|<VERSION>|<LANGUAGE>|<MODULE_TYPE>|<ARTIFACT_CLASS>|<MAGIC_SIGNATURE>|<HASH_METHOD>|<FILE_GUID>|<PARENT_GUID>
#AVIS_LAW_DISK:<ABSOLUTE_PATH>|<RELATIVE_PATH>|<WORKSPACE_ROOT>|<VOLUME_NAME>|<DRIVE_LETTER>
#AVIS_LAW_SECURITY:<HASH_VALIDATION>|<GUID_VALIDATION>|<PATH_NORMALIZATION>|<METADATA_VERIFICATION>
#AVIS_LAW_SNAPSHOT_CREATE:<STATUS>|<VERIFICATION_STATUS>|<SNAPSHOT_GUID>
#AVIS_LAW_LINEAGE:<PROJECT_NAME>-><MODULE>-><SUBMODULE>-><ARTIFACT>
```

---
```
### ✅ Features of _DL version:

1. **Single-line key-value fields** separated by `|` for ultra-fast ingestion.
2. **Placeholders only**, allowing automated pipelines or robots to populate live data.
3. Preserves **`#AVIS_LAW_*`** identifiers for compliance scanning.
4. Ideal for **data lake or bulk AI ingestion**, minimal parsing overhead.
```
