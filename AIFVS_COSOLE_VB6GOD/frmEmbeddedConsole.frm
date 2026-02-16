VERSION 5.00
Begin VB.Form frmEmbeddedConsole 
   BackColor       =   &H00000000&
   Caption         =   "GODLY AIFVS Embedded Console"
   ClientHeight    =   5175
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7845
   KeyPreview      =   -1  'True
   LinkTopic       =   "frmEmbeddedConsole"
   ScaleHeight     =   345
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   523
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   240
      Top             =   2640
   End
   Begin VB.PictureBox picHost 
      Height          =   1455
      Left            =   1440
      ScaleHeight     =   93
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   141
      TabIndex        =   0
      Top             =   600
      Width           =   2175
   End
   Begin VB.PictureBox picOverlay 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FFFFFF&
      BorderStyle     =   0  'None
      DrawStyle       =   5  'Transparent
      ForeColor       =   &H80000008&
      Height          =   2415
      Left            =   5160
      ScaleHeight     =   161
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   177
      TabIndex        =   1
      Top             =   120
      Width           =   2655
   End
End
Attribute VB_Name = "frmEmbeddedConsole"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit
' ============================================================
' AIFVS REGISTRATION
' ARTIFACT:   AIFVS_FORM_EMBEDDED_CONSOLE
' OWNER:      DEMON
' ROLE:       EMBEDDED WIN32 CONSOLE HOST
' PURPOSE:    HOSTS CONSOLE IN picHost WITH POPUP CLIP CONTROLS
' LINKS:      modNavPopup, clsClipControls
' ============================================================

Private Sub Form_Load()
    Me.ScaleMode = vbPixels
    picHost.ScaleMode = vbPixels
    picOverlay.ScaleMode = vbPixels

    ' Position host + overlay
    picHost.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
    picOverlay.Move 0, 0, Me.ScaleWidth - 24, Me.ScaleHeight

    ' Position window
    Me.Left = Screen.Width - Me.Width - 100
    Me.Top = 400 + Me.Top

    ' --- ORDER FIX: console must be created BEFORE resize ---
    InitEmbeddedConsole picHost.hWnd

    ' Now that g_hConsole exists, resize it properly
    ResizeEmbeddedConsole g_hConsole, picHost.ScaleWidth, picHost.ScaleHeight

    ' Launch CMD after console is embedded
    Shell "cmd.exe", vbNormalFocus
End Sub


Private Sub Form_Resize()
    If Me.WindowState = vbMinimized Then Exit Sub

    Me.ScaleMode = vbPixels
    picHost.ScaleMode = vbPixels
    picOverlay.ScaleMode = vbPixels

    ' Resize host + overlay
    picHost.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
    picOverlay.Move 0, 0, Me.ScaleWidth - 24, Me.ScaleHeight

    ' Resize embedded console to match host
    If g_hConsole <> 0 Then
        ResizeEmbeddedConsole g_hConsole, picHost.ScaleWidth, picHost.ScaleHeight
    End If
End Sub


Private Sub Form_Unload(Cancel As Integer)
    ShutdownEmbeddedConsole
End Sub

Private Sub picHost_KeyPress(KeyAscii As Integer)
'SendKeys KeyAscii
DoEvents
End Sub

Private Sub picOverlay_KeyPress(KeyAscii As Integer)
'picHost.SetFocus
   ' picHost_KeyPress KeyAscii
       ' If m_TargetHwnd <> 0 Then
        Call SendMessage(g_hConsole, WM_CHAR, KeyAscii, 0&)
   ' End If



End Sub

Private Sub picOverlay_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)


  If Button = vbRightButton Then
        Set Clip = New clsClipControls
        Clip.TargetHwnd = picOverlay.hWnd

        ShowClipPopupAtMouse picOverlay.hWnd, X, Y
    End If

End Sub

Private Sub Timer1_Timer()
Timer1 = False
Me.Width = Me.Width + 1
End Sub
