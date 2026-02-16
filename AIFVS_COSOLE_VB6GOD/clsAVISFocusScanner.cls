VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAVISFocusScanner"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
' AIFVS-ARTIFACT
' FILE: clsAVISFocusScanner.cls
' PURPOSE: Monitor foreground focus, copy window text, and push it into a target textbox.

Option Explicit

' ===== Win32 API =====
Private Declare Function GetForegroundWindow Lib "user32" () As Long
Private Declare Function GetWindowTextA Lib "user32" _
    (ByVal hWnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
Private Declare Function GetWindowTextLengthA Lib "user32" _
    (ByVal hWnd As Long) As Long
Private Declare Function SetWindowTextA Lib "user32" _
    (ByVal hWnd As Long, ByVal lpString As String) As Long

' ===== Internal State =====
Private mTargetTextBox As Long

' Attach a textbox hwnd for output
Public Sub AttachTextBox(ByVal hTextBox As Long)
    mTargetTextBox = hTextBox
End Sub

' Called on each timer tick from CYHY module
Public Sub UpdateTick()
    Dim fg As Long
    fg = GetForegroundWindow()
    If fg = 0 Then Exit Sub

    Dim txt As String
    txt = GetWindowTextVB(fg)

    If mTargetTextBox <> 0 Then
        SetWindowTextA mTargetTextBox, txt
    End If
End Sub

' --- internal: copy window text ---
Private Function GetWindowTextVB(ByVal hWnd As Long) As String
    Dim ln As Long
    ln = GetWindowTextLengthA(hWnd)
    If ln <= 0 Then Exit Function

    Dim buf As String
    buf = String$(ln + 2, 0)
    GetWindowTextA hWnd, buf, ln + 1
    GetWindowTextVB = Trim$(buf)
End Function



