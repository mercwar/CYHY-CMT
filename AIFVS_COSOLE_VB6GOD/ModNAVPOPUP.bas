Attribute VB_Name = "ModNAVPOPUP"

Option Explicit
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" _
    (ByVal hWnd As Long, ByVal wMsg As Long, _
     ByVal wParam As Long, ByVal lParam As Long) As Long

Public Declare Function PostMessage Lib "user32" Alias "PostMessageA" _
    (ByVal hWnd As Long, ByVal wMsg As Long, _
     ByVal wParam As Long, ByVal lParam As Long) As Long

' ============================================================
' AIFVS REGISTRATION
' ARTIFACT:   AIFVS_MOD_NAVPOPUP
' OWNER:      DEMON
' ROLE:       GLOBAL NAVIGATION POPUP
' PURPOSE:    OPEN/CLOSE/CONTROL COMMAND WINDOWS
' ============================================================

Private Declare Function CreatePopupMenu Lib "user32" () As Long
Private Declare Function AppendMenu Lib "user32" Alias "AppendMenuA" _
    (ByVal hMenu As Long, ByVal uFlags As Long, _
     ByVal uIDNewItem As Long, ByVal lpNewItem As String) As Long

Private Declare Function TrackPopupMenu Lib "user32" _
    (ByVal hMenu As Long, ByVal uFlags As Long, _
     ByVal X As Long, ByVal Y As Long, _
     ByVal nReserved As Long, ByVal hWnd As Long, _
     ByVal prcRect As Long) As Long

Public Type POINTAPI
    X As Long
    Y As Long
End Type

Public Declare Function ClientToScreen Lib "user32" (ByVal hWnd As Long, lpPoint As POINTAPI) As Long

Private Const MF_STRING = &H0&
Private Const MF_SEPARATOR = &H800&
Private Const TPM_RETURNCMD = &H100&

Public Enum NAV_CMDS
    NAV_OPEN_HIDDEN = 1
    NAV_OPEN_EMBED = 2
    NAV_BOTH = 3
    NAV_EXIT = 20
End Enum

' ============================================================
' PUBLIC ENTRY: SHOW POPUP AT MOUSE
' ============================================================

Public Sub ShowNavPopupAtMouse(ByVal hWnd As Long, ByVal X As Long, ByVal Y As Long)
    Dim pt As POINTAPI

    pt.X = X
    pt.Y = Y

    ' Convert client ? screen
    ClientToScreen hWnd, pt

    ShowNavPopup hWnd, pt.X, pt.Y
End Sub

' ============================================================
' INTERNAL: BUILD POPUP
' ============================================================
Private Sub ShowNavPopup(ByVal hWnd As Long, ByVal X As Long, ByVal Y As Long)
    Dim hMenu As Long
    Dim cmd As Long

    hMenu = CreatePopupMenu()

    AppendMenu hMenu, MF_STRING, NAV_OPEN_HIDDEN, "Open Hidden Shell"
    AppendMenu hMenu, MF_STRING, NAV_OPEN_EMBED, "Open Embedded Console"
    AppendMenu hMenu, MF_STRING, NAV_BOTH, "Open Both"
    AppendMenu hMenu, MF_SEPARATOR, 0, vbNullString
    AppendMenu hMenu, MF_STRING, NAV_EXIT, "Exit Navigator"

    cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, X, Y, 0, hWnd, 0)

    If cmd <> 0 Then HandleNavCommand cmd
End Sub

' ============================================================
' COMMAND ROUTER
' ============================================================
Private Sub HandleNavCommand(ByVal cmd As Long)
    Select Case cmd

        Case NAV_OPEN_HIDDEN
            If frmHiddenShell Is Nothing Then Load frmHiddenShell
            frmHiddenShell.Show

        Case NAV_OPEN_EMBED
            If frmEmbeddedConsole Is Nothing Then Load frmEmbeddedConsole
            frmEmbeddedConsole.Show

        Case NAV_BOTH
            If frmHiddenShell Is Nothing Then Load frmHiddenShell
            If frmEmbeddedConsole Is Nothing Then Load frmEmbeddedConsole
            frmHiddenShell.Show
            frmEmbeddedConsole.Show

        Case NAV_EXIT
            Unload frmNavigator

    End Select
End Sub

