VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsClipControls"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

' ============================================================
' AIFVS REGISTRATION
' ARTIFACT:   AIFVS_CLASS_CLIPCONTROLS
' OWNER:      DEMON
' ROLE:       UNIVERSAL CLIPBOARD CONTROL ENGINE
' PURPOSE:    PROVIDES COPY/PASTE/CUT/UNDO/SELECTALL FOR ANY HWND
' ============================================================

' Windows messages
Private Const WM_COPY As Long = &H301&
Private Const WM_PASTE As Long = &H302&
Private Const WM_CUT As Long = &H300&
Private Const WM_CLEAR As Long = &H303&
Private Const WM_UNDO As Long = &H304&
Private Const WM_GETTEXTLENGTH As Long = &HE&
Private Const EM_SETSEL As Long = &HB1&
Private Const EM_REPLACESEL As Long = &HC2&

Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" _
    (ByVal hWnd As Long, ByVal wMsg As Long, _
     ByVal wParam As Long, ByVal lParam As Long) As Long

Private Declare Function PostMessage Lib "user32" Alias "PostMessageA" _
    (ByVal hWnd As Long, ByVal wMsg As Long, _
     ByVal wParam As Long, ByVal lParam As Long) As Long

' ============================================================
' TARGET WINDOW HANDLE
' ============================================================
Private m_Target As Long

Public Property Let TargetHwnd(ByVal hWnd As Long)
    m_Target = hWnd
End Property

Public Property Get TargetHwnd() As Long
    TargetHwnd = m_Target
End Property

' ============================================================
' CLIPBOARD OPERATIONS
' ============================================================
Public Sub Copy()
    If m_Target <> 0 Then PostMessage m_Target, WM_COPY, 0, 0
End Sub

Public Sub Paste()
    If m_Target <> 0 Then PostMessage m_Target, WM_PASTE, 0, 0
End Sub

Public Sub Cut()
    If m_Target <> 0 Then PostMessage m_Target, WM_CUT, 0, 0
End Sub

Public Sub Undo()
    If m_Target <> 0 Then PostMessage m_Target, WM_UNDO, 0, 0
End Sub

Public Sub Clear()
    If m_Target <> 0 Then PostMessage m_Target, WM_CLEAR, 0, 0
End Sub

Public Sub SelectAll()
    If m_Target <> 0 Then
        Dim sz As Long
        sz = SendMessage(m_Target, WM_GETTEXTLENGTH, 0, 0)
        SendMessage m_Target, EM_SETSEL, 0, sz
    End If
End Sub

Public Sub ReplaceText(ByVal s As String)
    If m_Target <> 0 Then
        SendMessage m_Target, EM_REPLACESEL, 1, ByVal s
    End If
End Sub

