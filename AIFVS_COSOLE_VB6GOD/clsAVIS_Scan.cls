VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAVIS_Scan"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
' ============================================
' AIFVS-ARTIFACT
' clsAVIS_Scan
' Scans AVIS_BUFF for AVIS comment blocks
' ============================================

Option Explicit

Private Const TARGET As String = "avis"

' ------------------------------------------------------------
' Returns True if AVIS comment exists anywhere in AVIS_BUFF

Public Function HasAVIS() As Boolean
    HasAVIS = (InStr(1, LCase$(AVIS_BUFF), TARGET) > 0)
End Function

Public Function GetAVISBlock() As String
    Dim blk As String

    blk = AVIS_ExtractCommentBlock(AVIS_BUFF)

    ' If extractor returned whole file or a block, just pass it through
    If Len(blk) > 0 Then
        GetAVISBlock = blk
    End If
End Function


' ------------------------------------------------------------
' Returns all lines that contain the word AVIS
' ------------------------------------------------------------
Public Function GetAVISLines() As String
    Dim lines() As String
    Dim out As String
    Dim i As Long

    lines = Split(AVIS_BUFF, vbCrLf)

    For i = 0 To UBound(lines)
        If InStr(1, LCase$(lines(i)), TARGET) > 0 Then
            out = out & lines(i) & vbCrLf
        End If
    Next i

    GetAVISLines = out
End Function

' ------------------------------------------------------------
' Returns the FIRST AVIS comment block (/* ... */)
' ------------------------------------------------------------


' ------------------------------------------------------------
' Returns ALL comment blocks (AVIS or not)
' ------------------------------------------------------------
Public Function GetAllBlocks() As Collection
    Set GetAllBlocks = AVIS_ExtractAllCommentBlocks(AVIS_BUFF)
End Function

' ------------------------------------------------------------
' Parse AVIS fields into a collection
' ------------------------------------------------------------
Public Function ParseAVISBlock(ByVal Block As String) As Collection
    Set ParseAVISBlock = AVIS_ParseFields(Block)
End Function

' ------------------------------------------------------------
' Create a new AVIS template
' ------------------------------------------------------------
Public Function NewAVISTemplate(Optional ByVal Name As String = "NEW_ARTIFACT") As String
    NewAVISTemplate = AVIS_CreateTemplate(Name)
End Function

