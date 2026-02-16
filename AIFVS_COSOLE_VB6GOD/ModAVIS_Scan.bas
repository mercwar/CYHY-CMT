Attribute VB_Name = "ModAVIS_Scan"
Option Explicit

' ============================================================
' ModAVIS_Scan
' AVIS extraction + parsing utilities
' ============================================================
Public AVIS_BUFF As String
  ' GLOBAL BUFFER

' ------------------------------------------------------------
' Extract FIRST /* ... */ block




Public Function AVIS_ExtractCommentBlock(ByVal Raw As String) As String
    Dim s As Long, e As Long

    s = InStr(1, Raw, "/*", vbTextCompare)
    If s = 0 Then
        ' NO COMMENT BLOCK – FALL BACK TO WHOLE FILE
        If InStr(1, Raw, "avis", vbTextCompare) > 0 Then
            AVIS_ExtractCommentBlock = Raw
        End If
        Exit Function
    End If

    e = InStr(s + 2, Raw, "*/", vbTextCompare)
    If e = 0 Then
        ' OPENED BUT NOT CLOSED – FALL BACK TO WHOLE FILE
        If InStr(1, Raw, "avis", vbTextCompare) > 0 Then
            AVIS_ExtractCommentBlock = Raw
        End If
        Exit Function
    End If

    e = e + 2
    AVIS_ExtractCommentBlock = Mid$(Raw, s, e - s)
End Function


' ------------------------------------------------------------
' Extract ALL comment blocks
' ------------------------------------------------------------
Public Function AVIS_ExtractAllCommentBlocks(ByVal Raw As String) As Collection
    Dim col As New Collection
    Dim s As Long, e As Long, blk As String

    s = InStr(1, Raw, "/*", vbTextCompare)
    Do While s > 0
        e = InStr(s + 2, Raw, "*/", vbTextCompare)
        If e = 0 Then Exit Do

        e = e + 2
        blk = Mid$(Raw, s, e - s)
        col.Add blk

        s = InStr(e + 1, Raw, "/*", vbTextCompare)
    Loop

    Set AVIS_ExtractAllCommentBlocks = col
End Function

' ------------------------------------------------------------
' Determine if a block is AVIS-based
' ------------------------------------------------------------
Public Function AVIS_IsBlock(ByVal Block As String) As Boolean
    If InStr(1, Block, "AIFVS-ARTIFACT", vbTextCompare) > 0 Then
        AVIS_IsBlock = True
    ElseIf InStr(1, Block, "AIFVS", vbTextCompare) > 0 Then
        AVIS_IsBlock = True
    ElseIf InStr(1, Block, "AVIS", vbTextCompare) > 0 Then
        AVIS_IsBlock = True
    End If
End Function

' ------------------------------------------------------------
' Parse AVIS fields into a collection
' ------------------------------------------------------------
Public Function AVIS_ParseFields(ByVal Block As String) As Collection
    Dim col As New Collection
    Dim lines() As String
    Dim i As Long, p As Long
    Dim key As String, val As String

    lines = Split(Block, vbCrLf)

    For i = LBound(lines) To UBound(lines)
        p = InStr(lines(i), ":")
        If p > 0 Then
            key = Trim$(Left$(lines(i), p - 1))
            val = Trim$(Mid$(lines(i), p + 1))

            If Len(key) > 0 Then
                On Error Resume Next
                col.Add val, key
                On Error GoTo 0
            End If
        End If
    Next i

    Set AVIS_ParseFields = col
End Function

' ------------------------------------------------------------
' Create a clean AVIS template
' ------------------------------------------------------------
Public Function AVIS_CreateTemplate(Optional ByVal Name As String = "NEW_ARTIFACT") As String
    Dim t As String

    t = "/* AIFVS-ARTIFACT" & vbCrLf
    t = t & "   CY_NAME: " & Name & vbCrLf
    t = t & "   CY_TYPE: c_class" & vbCrLf
    t = t & "   CY_ROLE: <role>" & vbCrLf
    t = t & "   CY_LINK: dl/" & Name & ".c" & vbCrLf & vbCrLf
    t = t & "   DL_MAP:  ACK/RACK" & vbCrLf
    t = t & "   DL_DRV:  ACK/RACK" & vbCrLf
    t = t & "   DL_LDIR: ACK/RACK" & vbCrLf
    t = t & "   DL_WDIR: ACK/RACK" & vbCrLf
    t = t & "   DL_FILE: ACK/RACK" & vbCrLf
    t = t & "   DL_EXT:  ACK/RACK" & vbCrLf
    t = t & "   DL_FFN:  RRAC" & vbCrLf
    t = t & "*/"

    AVIS_CreateTemplate = t
End Function

