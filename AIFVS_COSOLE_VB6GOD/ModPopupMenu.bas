Attribute VB_Name = "ModPopupMenu"
Option Explicit

Private Declare Function CreatePopupMenu Lib "user32" () As Long
Private Declare Function AppendMenu Lib "user32" Alias "AppendMenuA" _
    (ByVal hMenu As Long, ByVal uFlags As Long, _
     ByVal uIDNewItem As Long, ByVal lpNewItem As String) As Long

Private Declare Function TrackPopupMenu Lib "user32" _
    (ByVal hMenu As Long, ByVal uFlags As Long, _
     ByVal X As Long, ByVal Y As Long, _
     ByVal nReserved As Long, ByVal hWnd As Long, _
     ByVal prcRect As Long) As Long

Private Const MF_STRING = &H0&
Private Const MF_SEPARATOR = &H800&
Private Const TPM_RETURNCMD = &H100&

' Menu command IDs
Public Enum POP_CMDS
    CM_COPY = 1
    CM_PASTE = 2
    CM_CUT = 3
    CM_UNDO = 4
    CM_CLEAR = 5
    CM_SELALL = 6
End Enum

Public Clip As clsClipControls

Public CShell As New AIFVS_DOSShell

Public Sub ShowClipPopupAtMouse(ByVal hWnd As Long, ByVal X As Long, ByVal Y As Long)
    Dim pt As POINTAPI

    pt.X = X
    pt.Y = Y
'Dim t As Long
    ' Convert client ? screen
    ClientToScreen hWnd, pt

    ' Show the clip popup at the correct screen location
    ShowClipPopup hWnd, pt.X, pt.Y
End Sub
Public Sub ShowClipPopup(ByVal hWnd As Long, ByVal X As Long, ByVal Y As Long)
    Dim hMenu As Long
    Dim cmd As Long

    hMenu = CreatePopupMenu()

    AppendMenu hMenu, MF_STRING, CM_COPY, "Copy"
    AppendMenu hMenu, MF_STRING, CM_PASTE, "Paste"
    AppendMenu hMenu, MF_STRING, CM_CUT, "Cut"
    AppendMenu hMenu, MF_STRING, CM_UNDO, "Undo"
    AppendMenu hMenu, MF_STRING, CM_CLEAR, "Clear"
    AppendMenu hMenu, MF_STRING, CM_SELALL, "Select All"



    cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, X, Y, 0, hWnd, 0)

    If cmd <> 0 Then HandleClipCommand cmd
End Sub



' ============================================================
' AIFVS COORDINATE CONVERTER
' MAKECOORD: Converts client (X,Y) ? screen POINTAPI
' ============================================================
Public Function MakeCoord(ByVal hWnd As Long, ByVal X As Long, ByVal Y As Long) As POINTAPI
    Dim pt As POINTAPI

    pt.X = X
    pt.Y = Y

    ' Convert client ? screen
    ClientToScreen hWnd, pt

    MakeCoord = pt
End Function
Public Sub HandleClipCommand(ByVal cmd As Long)
    Select Case cmd
        Case CM_COPY: Clip.Copy
        Case CM_PASTE: Clip.Paste
        Case CM_CUT: Clip.Cut
        Case CM_UNDO: Clip.Undo
        Case CM_CLEAR: Clip.Clear
        Case CM_SELALL: Clip.SelectAll
    End Select
End Sub
