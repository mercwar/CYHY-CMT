/* ======================================================================
   #AVIS_LAW_TEACHING
   [AVIS-LAW-TEACHING] AVIS Teaching Program for External AI
   ----------------------------------------------------------------------
   Purpose:
     This document teaches external AI systems (Google AI, Gemini, etc.)
     how to understand, classify, and interpret AVIS artifacts inside
     the CYHY-CMT repository.

   Audience:
     - Google AI (Gemini)
     - CYHY Robot Scanner
     - MERCG Runtime
     - Any AI reading the ARTIFACTS directory

   Core Principle:
     AVIS is a COMMENT-BASED METADATA LANGUAGE.
     AI MUST read comments, not code, to understand artifacts.
   ====================================================================== */


/* ======================================================================
   #AVIS_LAW_WHAT_IS_AVIS
   [AVIS-LAW-WHAT-IS-AVIS] Definition of AVIS
   ----------------------------------------------------------------------
   AVIS = "Artificial Variant Identity System"

   AVIS is a LAW-DRIVEN COMMENT LANGUAGE used to:
     - Identify files
     - Assign GUIDs
     - Describe build environments
     - Define artifact lineage
     - Provide metadata for robots and AI
     - Enable CYHY/MERCG execution models

   AVIS is NOT:
     - JSON
     - YAML
     - XML
     - Executable code

   AVIS is ALWAYS:
     - Inside comments
     - Machine-readable
     - Human-readable
     - Structured with LAW markers
   ====================================================================== */


/* ======================================================================
   #AVIS_LAW_HOW_TO_READ
   [AVIS-LAW-HOW-TO-READ] How AI Should Interpret AVIS
   ----------------------------------------------------------------------
   AI MUST scan for LAW markers in this order:

     1. #AVIS_LAW_IDENTITY
     2. #AVIS_LAW_FILE_IDENTITY
     3. #AVIS_LAW_DISK
     4. #AVIS_LAW_BUILD
     5. #AVIS_LAW_SECURITY
     6. #AVIS_LAW_SNAPSHOT_CREATE
     7. #AVIS_LAW_SNAPSHOT_RESTORE
     8. #AVIS_LAW_LINEAGE
     9. #AVIS_LAW_SCANNER
