Attribute VB_Name = "modConCleaner"
Option Explicit

Private Declare Function GetWindowThreadProcessId Lib "user32" _
    (ByVal hWnd As Long, lpdwProcessId As Long) As Long

Private Declare Function EnumWindows Lib "user32" _
    (ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long

Private Declare Function GetClassName Lib "user32" Alias "GetClassNameA" _
    (ByVal hWnd As Long, ByVal lpClassName As String, ByVal nMaxCount As Long) As Long

Private Declare Function PostMessage Lib "user32" Alias "PostMessageA" _
    (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

Private Const WM_CLOSE As Long = &H10&

Private m_ThisPID As Long

Private Declare Function GetCurrentProcessId Lib "kernel32" () As Long

Public Function EnumConsoleCleaner(ByVal hWnd As Long, ByVal lParam As Long) As Long
    Dim pid As Long
    Dim cls As String * 64
    Dim ln As Long

    ln = GetClassName(hWnd, cls, 63)
    If ln > 0 Then
        cls = Left$(cls, ln)
'Debug.Print cls & "," & ln
        If cls = "ConsoleWindowClass" Then
            GetWindowThreadProcessId hWnd, pid

            If pid = m_ThisPID Then
                PostMessage hWnd, WM_CLOSE, 0, 0
            End If
        End If
    End If

    EnumConsoleCleaner = 1   ' <-- THIS IS THE LOOP
End Function


Public Sub CleanOrphanConsoles()
    ' Cache our process ID
    m_ThisPID = GetCurrentProcessId()

    ' Enumerate all windows
    EnumWindows AddressOf EnumConsoleCleaner, 0
End Sub



