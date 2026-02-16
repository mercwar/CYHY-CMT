VERSION 5.00
Begin VB.Form frmEmbeddedConsole 
   BackColor       =   &H00000000&
   Caption         =   "GODLY AIFVS Embedded Console"
   ClientHeight    =   5175
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7740
   KeyPreview      =   -1  'True
   LinkTopic       =   "frmEmbeddedConsole"
   ScaleHeight     =   345
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   516
   StartUpPosition =   2  'CenterScreen
   Begin VB.PictureBox picOverlay 
      Appearance      =   0  'Flat
      BackColor       =   &H00FFFFFF&
      BorderStyle     =   0  'None
      DrawStyle       =   5  'Transparent
      ForeColor       =   &H80000008&
      Height          =   1935
      Left            =   600
      ScaleHeight     =   129
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   473
      TabIndex        =   1
      Top             =   0
      Width           =   7095
   End
   Begin VB.PictureBox picHost 
      AutoRedraw      =   -1  'True
      Height          =   2415
      Left            =   120
      ScaleHeight     =   157
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   453
      TabIndex        =   0
      Top             =   2040
      Width           =   6855
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
 
    ' Initialize and embed the console into picHost
    InitEmbeddedConsole picHost.hWnd
    DoEvents
    
    Me.ScaleMode = vbPixels
    picHost.ScaleMode = vbPixels
    picOverlay.ScaleMode = vbPixels
    
    ' Resize the console to fill the host area
    picHost.Move 0, 0, picHost.ScaleWidth, picHost.ScaleWidth
    picOverlay.Move 0, 0, picHost.ScaleWidth - 20, picHost.ScaleWidth
    picOverlay.ZOrder 0
    ResizeEmbeddedConsole g_hConsole, Me.ScaleWidth, Me.ScaleHeight
    Dim ret As Long
    ret = SetWindowLong(picOverlay.hWnd, GWL_EXSTYLE, WS_EX_TRANSPARENT)

    ' Launch cmd.exe; it will attach to this process's console
    Shell "cmd.exe", vbNormalFocus
End Sub



Private Sub Form_Resize()
    On Error Resume Next

    Me.ScaleMode = vbPixels
    picHost.ScaleMode = vbPixels
    picOverlay.ScaleMode = vbPixels

    picHost.Move 0, 0, Me.ScaleWidth - 0, Me.ScaleHeight
    picOverlay.Move 0, 0, Me.ScaleWidth - 24, Me.ScaleHeight
    picOverlay.ZOrder 0
    ResizeEmbeddedConsole g_hConsole, Me.ScaleWidth, Me.ScaleHeight

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
