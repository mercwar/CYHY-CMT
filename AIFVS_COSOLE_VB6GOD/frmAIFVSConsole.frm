VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form frmHiddenShell 
   Caption         =   "GODLY AIFVS CMD Terminal"
   ClientHeight    =   4935
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   9735
   LinkTopic       =   "Form1"
   ScaleHeight     =   4935
   ScaleWidth      =   9735
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrPoll 
      Interval        =   100
      Left            =   6960
      Top             =   120
   End
   Begin RichTextLib.RichTextBox rtbConsole 
      Height          =   4170
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7095
      _ExtentX        =   12515
      _ExtentY        =   7355
      _Version        =   393217
      ReadOnly        =   -1  'True
      ScrollBars      =   3
      TextRTF         =   $"frmAIFVSConsole.frx":0000
   End
   Begin VB.TextBox txtInput 
      Height          =   330
      Left            =   120
      TabIndex        =   1
      Top             =   4380
      Width           =   7095
   End
End
Attribute VB_Name = "frmHiddenShell"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit



Private Sub Form_Load()
    rtbConsole.Text = ""
    txtInput.Text = ""

    CShell.InitShell

    tmrPoll.Interval = 100
    tmrPoll.Enabled = True

    AppendLine "AIFVS CMD Terminal"
    AppendLine "Hidden cmd.exe shell initialized."
    AppendLine ""
    AppendPrompt
End Sub

Private Sub Form_Resize()
    On Error Resume Next

    'rtbConsole.Move 120, 120, ScaleWidth - 240, ScaleHeight - 240 - txtInput.Height
   ' txtInput.Move 120, rtbConsole.Top + rtbConsole.Height + 60, ScaleWidth - 240
End Sub

Private Sub Form_Unload(Cancel As Integer)
    tmrPoll.Enabled = False
    On Error Resume Next
    CShell.SendCommand "exit"
    CShell.KillShell
End Sub

Private Sub rtbConsole_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button = vbRightButton Then
        Set Clip = New clsClipControls
        Clip.TargetHwnd = rtbConsole.hWnd

        ShowClipPopupAtMouse Me.hWnd, CLng(Me.ScaleX(X, vbTwips, vbPixels)), CLng(Me.ScaleY(Y, vbTwips, vbPixels))
    End If
End Sub

Private Sub txtInput_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then  ' ENTER
        Dim cmd As String
        cmd = Trim$(txtInput.Text)

        If Len(cmd) > 0 Then
            AppendLine "> " & cmd
            CShell.SendCommand cmd
        End If

        txtInput.Text = ""
        KeyAscii = 0
    End If
End Sub

Private Sub tmrPoll_Timer()
    Dim s As String

    s = CShell.ReadOutput
    If Len(s) > 0 Then
        AppendRaw s
    End If
End Sub

Private Sub AppendLine(ByVal s As String)
    rtbConsole.SelStart = Len(rtbConsole.Text)
    rtbConsole.SelText = s & vbCrLf
    rtbConsole.SelStart = Len(rtbConsole.Text)
End Sub

Private Sub AppendRaw(ByVal s As String)
    rtbConsole.SelStart = Len(rtbConsole.Text)
    rtbConsole.SelText = s
    rtbConsole.SelStart = Len(rtbConsole.Text)
End Sub

Private Sub AppendPrompt()
    rtbConsole.SelStart = Len(rtbConsole.Text)
    rtbConsole.SelText = "> "
    rtbConsole.SelStart = Len(rtbConsole.Text)
End Sub

Private Sub txtInput_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button = vbRightButton Then
        Set Clip = New clsClipControls
        Clip.TargetHwnd = txtInput.hWnd

        ShowClipPopupAtMouse Me.hWnd, _
            Me.ScaleX(X, vbTwips, vbPixels), _
            Me.ScaleY(Y, vbTwips, vbPixels)
    End If
End Sub

