Attribute VB_Name = "modEmbeddedConsole"
Option Explicit
Public Const GWL_STYLE = (-16)
Public Const GWL_EXSTYLE = (-20)
Public Const WS_EX_TRANSPARENT = &H20&
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal HWND As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
' --- Console allocation ---
Private Declare Function AllocConsole Lib "kernel32" () As Long
Private Declare Function FreeConsole Lib "kernel32" () As Long
Private Declare Function GetConsoleWindow Lib "kernel32" () As Long

' --- Window parenting + movement ---
Private Declare Function SetParent Lib "user32" _
    (ByVal hWndChild As Long, ByVal hWndNewParent As Long) As Long

Private Declare Function MoveWindow Lib "user32" _
    (ByVal HWND As Long, ByVal X As Long, ByVal Y As Long, _
     ByVal nWidth As Long, ByVal nHeight As Long, _
     ByVal bRepaint As Long) As Long

Private Declare Function ShowWindow Lib "user32" _
    (ByVal HWND As Long, ByVal nCmdShow As Long) As Long

Private Const SW_SHOW As Long = 5
Private Const SW_HIDE As Long = 0
Private Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" _
    (ByVal HWND As Long, ByVal nIndex As Long) As Long



Public Const WM_CHAR     As Long = &H102&
Public Const WM_KEYDOWN  As Long = &H100&
Public Const WM_KEYUP    As Long = &H101&

Private Const WS_CAPTION As Long = &HC00000
Private Const WS_THICKFRAME As Long = &H40000
' --- Global console window handle ---
Public g_hConsole As Long
Public Sub RemoveConsoleFrame()
    If g_hConsole = 0 Then Exit Sub

    Dim style As Long
    style = GetWindowLong(g_hConsole, GWL_STYLE)

    style = style And Not WS_CAPTION
    style = style And Not WS_THICKFRAME

    SetWindowLong g_hConsole, GWL_STYLE, style
End Sub

' ============================================================
' Initialize and embed the console into a host control
' ============================================================
Public Sub InitEmbeddedConsole(ByVal hHost As Long)
    Dim ret As Long

    ' Allocate a console for this process
    ret = AllocConsole()

    ' Get the console window handle
    g_hConsole = GetConsoleWindow()
    If g_hConsole = 0 Then Exit Sub
    RemoveConsoleFrame
    ' Reparent console into the host (PictureBox)
    SetParent g_hConsole, hHost

    ' Show the console window (now a child control)
    ShowWindow g_hConsole, SW_SHOW
End Sub


' ============================================================
' Resize the embedded console to match the host area
' ============================================================
Public Sub ResizeEmbeddedConsole(HWND As Long, ByVal w As Long, ByVal h As Long)
    If HWND <> 0 Then
        MoveWindow HWND, 0, 0, w, h, 1
    End If
End Sub


' ============================================================
' Shutdown and free the console
' ============================================================
Public Sub ShutdownEmbeddedConsole()
    If g_hConsole <> 0 Then
        ShowWindow g_hConsole, SW_HIDE
    End If

    FreeConsole
    g_hConsole = 0
End Sub

