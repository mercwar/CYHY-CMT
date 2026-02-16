VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "AIFVS_DOSShell"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
Private Declare Function GetWindow Lib "user32" (ByVal hWnd As Long, ByVal wCmd As Long) As Long
 Const GW_OWNER = 4
 Const GW_MAX = 5
 Const GW_HWNDPREV = 3
 Const GW_HWNDNEXT = 2
 Const GW_HWNDLAST = 1
 Const GW_HWNDFIRST = 0
 Const GW_CHILD = 5

Private Declare Function GetConsoleWindow Lib "kernel32" () As Long
Private Declare Function ShowWindow Lib "user32" (ByVal hWnd As Long, ByVal nCmdShow As Long) As Long

Private Const SW_HIDE As Long = 0

Dim hConsole As Long

Private Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Long
End Type

Private Type STARTUPINFO
    cb As Long
    lpReserved As Long
    lpDesktop As Long
    lpTitle As Long
    dwX As Long
    dwY As Long
    dwXSize As Long
    dwYSize As Long
    dwXCountChars As Long
    dwYCountChars As Long
    dwFillAttribute As Long
    dwFlags As Long
    wShowWindow As Integer
    cbReserved2 As Integer
    lpReserved2 As Long
    hStdInput As Long
    hStdOutput As Long
    hStdError As Long
End Type

Private Type PROCESS_INFORMATION
    hProcess As Long
    hThread As Long
    dwProcessId As Long
    dwThreadId As Long
End Type

Private Declare Function CreatePipe Lib "kernel32" _
    (phReadPipe As Long, phWritePipe As Long, _
     lpPipeAttributes As SECURITY_ATTRIBUTES, ByVal nSize As Long) As Long

Private Declare Function SetHandleInformation Lib "kernel32" _
    (ByVal hObject As Long, ByVal dwMask As Long, ByVal dwFlags As Long) As Long

Private Declare Function CreateProcess Lib "kernel32" Alias "CreateProcessA" _
    (ByVal lpApplicationName As String, ByVal lpCommandLine As String, _
     lpProcessAttributes As Any, lpThreadAttributes As Any, _
     ByVal bInheritHandles As Long, ByVal dwCreationFlags As Long, _
     lpEnvironment As Any, ByVal lpCurrentDirectory As String, _
     lpStartupInfo As STARTUPINFO, lpProcessInformation As PROCESS_INFORMATION) As Long

Private Declare Function ReadFile Lib "kernel32" _
    (ByVal hFile As Long, ByVal lpBuffer As String, _
     ByVal nNumberOfBytesToRead As Long, lpNumberOfBytesRead As Long, _
     ByVal lpOverlapped As Long) As Long

Private Declare Function WriteFile Lib "kernel32" _
    (ByVal hFile As Long, ByVal lpBuffer As Any, _
     ByVal nNumberOfBytesToWrite As Long, lpNumberOfCharsWritten As Long, _
     ByVal lpOverlapped As Long) As Long

Private Declare Function PeekNamedPipe Lib "kernel32" _
    (ByVal hNamedPipe As Long, ByVal lpBuffer As Long, ByVal nBufferSize As Long, _
     lpBytesRead As Long, lpTotalBytesAvail As Long, lpBytesLeftThisMessage As Long) As Long

Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Private Const STARTF_USESTDHANDLES = &H100
Private Const CREATE_NO_WINDOW = &H8000000
Private Const HANDLE_FLAG_INHERIT = &H1

Private m_hInWrite As Long
Private m_hOutRead As Long

Private m_pi As PROCESS_INFORMATION
Private Declare Function TerminateProcess Lib "kernel32" (ByVal hProcess As Long, ByVal uExitCode As Long) As Long



Public Sub InitShell()
    Dim sa As SECURITY_ATTRIBUTES
    Dim hOutWrite As Long
    Dim hInRead As Long
    Dim si As STARTUPINFO
    Dim ok As Long

    sa.nLength = Len(sa)
    sa.bInheritHandle = 1

    CreatePipe m_hOutRead, hOutWrite, sa, 0
    SetHandleInformation m_hOutRead, HANDLE_FLAG_INHERIT, 0

    CreatePipe hInRead, m_hInWrite, sa, 0

    si.cb = Len(si)
    si.dwFlags = STARTF_USESTDHANDLES
    si.hStdOutput = hOutWrite
    si.hStdError = hOutWrite
    si.hStdInput = hInRead

    ok = CreateProcess(vbNullString, "cmd.exe", ByVal 0&, ByVal 0&, 1, _
                       CREATE_NO_WINDOW, ByVal 0&, vbNullString, si, m_pi)

    CloseHandle hOutWrite
    CloseHandle hInRead
End Sub

Public Sub SendCommand(ByVal s As String)
    Dim written As Long
    WriteFile m_hInWrite, ByVal (s & vbCrLf), Len(s) + 2, written, 0&
End Sub

Public Function ReadOutput() As String
    Dim avail As Long, dummy As Long
    Dim buf As String, read As Long

    PeekNamedPipe m_hOutRead, 0&, 0&, dummy, avail, dummy
    If avail = 0 Then Exit Function

    buf = String$(avail, vbNullChar)
    ReadFile m_hOutRead, buf, avail, read, 0&
    ReadOutput = Left$(buf, read)
End Function
Public Sub KillShell()
    If m_pi.hProcess <> 0 Then
        TerminateProcess m_pi.hProcess, 0
        CloseHandle m_pi.hProcess
        m_pi.hProcess = 0
    End If

    If m_pi.hThread <> 0 Then
        CloseHandle m_pi.hThread
        m_pi.hThread = 0
    End If
End Sub
