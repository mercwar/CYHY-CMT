## 🤖 ROBOT COMPLIANCE PROTOCOL
---
     1. Prioritization
          All AI or Automated Scanners **MUST** prioritize blocks containing `#AVIS_LAW_*`.  

     2. Placeholders
          This template contains **placeholders only** to allow human-readable study and understanding.  

     3. Purpose
         3.1 Standard Template
               Provides a **machine-readable template** for robots.  

         3.2 Human Reference
               Provides a **human-readable reference** for developers and auditors.  
---

# 1️⃣ SPECS: ROBOT
##### 📄 AVIS_PROTOCOL_STANDARD — System Identity Statute
###### Placeholders only; for automated parsing by robots
###### All LAW blocks use #AVIS_LAW_* and [AVIS-LAW-*] tags
```
#AVIS_LAW_IDENTITY [AVIS-LAW-IDENTITY] Disk Volume:<DISK_VOLUME>; Drive Letter:<DRIVE_LETTER>; File System:<FILE_SYSTEM>; Project Name:<PROJECT_NAME>; Project Class:<PROJECT_CLASS>; Repository:<REPOSITORY>; Project Owner:<PROJECT_OWNER>; Engine Layer:<ENGINE_LAYER>; Build System:<BUILD_SYSTEM>; Application Role:<APPLICATION_ROLE>

#AVIS_LAW_FILE_IDENTITY [AVIS-LAW-FILE-ID] File Name:<FILE_NAME>; Version:<VERSION>; Language:<LANGUAGE>; Module Type:<MODULE_TYPE>; Artifact Class:<ARTIFACT_CLASS>; Magic Signature:<MAGIC_SIGNATURE>; Hash Method:<HASH_METHOD>; File GUID:<FILE_GUID>; Parent GUID:<PARENT_GUID>

#AVIS_LAW_DISK [AVIS-LAW-DISK] Absolute Path:<ABSOLUTE_PATH>; Relative Path:<RELATIVE_PATH>; Workspace Root:<WORKSPACE_ROOT>; Volume Name:<VOLUME_NAME>; Drive Letter:<DRIVE_LETTER>

#AVIS_LAW_SECURITY [AVIS-LAW-SECURITY] Hash Validation:<HASH_VALIDATION>; GUID Validation:<GUID_VALIDATION>; Path Normalization:<PATH_NORMALIZATION>; Metadata Verification:<METADATA_VERIFICATION>

#AVIS_LAW_SNAPSHOT_CREATE [AVIS-LAW-SNAPSHOT-OPERATIONS] Status:<STATUS>; Verification:<VERIFICATION_STATUS>; Snapshot GUID:<SNAPSHOT_GUID>

#AVIS_LAW_LINEAGE [AVIS-LAW-LINEAGE] ROOT PROJECT-><PROJECT_NAME>-><MODULE>-><SUBMODULE>-><ARTIFACT>

```
---

# 2️⃣ SPECS: MAN

```markdown
# 📄 AVIS_PROTOCOL_MAN — Human-Readable Template

## ⚖️ THE AVIS LAW: DEFINITION
All comment blocks labeled `#AVIS_LAW_<SECTION>` and `[AVIS-LAW-<SECTION>]` are recognized by the AVIS engine and robots.  
Placeholders are shown for demonstration.

---

## 1️⃣ LAW: Identity Initialization
TAG: #AVIS_LAW_IDENTITY

[AVIS-LAW-IDENTITY] Identity System Initialized
----------------------------------
Disk Volume      : <DISK_VOLUME>
Drive Letter     : <DRIVE_LETTER>
File System      : <FILE_SYSTEM>

Project Name     : <PROJECT_NAME>
Project Class    : <PROJECT_CLASS>
Repository       : <REPOSITORY>
Project Owner    : <PROJECT_OWNER>

Engine Layer     : <ENGINE_LAYER>
Build System     : <BUILD_SYSTEM>
Application Role : <APPLICATION_ROLE>

---

## 2️⃣ LAW: File Identity Report
TAG: #AVIS_LAW_FILE_IDENTITY

[AVIS-LAW-FILE-ID] File Identity Report
----------------------------------
File Name        : <FILE_NAME>
Version          : <VERSION>
Language         : <LANGUAGE>
Module Type      : <MODULE_TYPE>
Artifact Class   : <ARTIFACT_CLASS>
Magic Signature  : <MAGIC_SIGNATURE>
Hash Method      : <HASH_METHOD>
File GUID        : <FILE_GUID>
Parent GUID      : <PARENT_GUID>

---

## 3️⃣ LAW: Disk Location
TAG: #AVIS_LAW_DISK

[AVIS-LAW-DISK] Disk Location
----------------------------------
Absolute Path    : <ABSOLUTE_PATH>
Relative Path    : <RELATIVE_PATH>
Workspace Root   : <WORKSPACE_ROOT>
Volume Name      : <VOLUME_NAME>
Drive Letter     : <DRIVE_LETTER>

---

## 4️⃣ LAW: Security Model
TAG: #AVIS_LAW_SECURITY

[AVIS-LAW-SECURITY] Security Model
----------------------------------
Hash Validation        : <HASH_VALIDATION>
GUID Validation        : <GUID_VALIDATION>
Path Normalization     : <PATH_NORMALIZATION>
Metadata Verification  : <METADATA_VERIFICATION>

---

## 5️⃣ LAW: Snapshot Engine
TAGS: #AVIS_LAW_SNAPSHOT_CREATE | #AVIS_LAW_SNAPSHOT_RESTORE

[AVIS-LAW-SNAPSHOT-OPERATIONS]
----------------------------------
Status           : <STATUS>
Verification     : <VERIFICATION_STATUS>
Snapshot GUID    : <SNAPSHOT_GUID>

---

## 6️⃣ LAW: Artifact Lineage Map
TAG: #AVIS_LAW_LINEAGE

[AVIS-LAW-LINEAGE] Artifact Lineage
----------------------------------
ROOT PROJECT
   └─ <PROJECT_NAME>
       └─ <MODULE>
           └─ <SUBMODULE>
               └─ <ARTIFACT>

```
---
✅ **Notes**:

* Fully machine-readable, single-line format per LAW block.
* No prefilled data, perfect for automated ingestion.
* Tags are preserved for compliance.

---
