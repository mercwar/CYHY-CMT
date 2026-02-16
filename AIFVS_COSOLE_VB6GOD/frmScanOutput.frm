VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmScanOutput 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "CYHY Comment Program"
   ClientHeight    =   8775
   ClientLeft      =   150
   ClientTop       =   795
   ClientWidth     =   12870
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8775
   ScaleWidth      =   12870
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer CYHY_TIMER 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   5760
      Top             =   4320
   End
   Begin MSComctlLib.TreeView tvEditors 
      Height          =   8415
      Left            =   0
      TabIndex        =   3
      Top             =   360
      Width           =   2895
      _ExtentX        =   5106
      _ExtentY        =   14843
      _Version        =   393217
      Style           =   7
      Appearance      =   1
   End
   Begin MSComctlLib.TabStrip TabStrip1 
      Height          =   30
      Left            =   3720
      TabIndex        =   2
      Top             =   1920
      Width           =   30
      _ExtentX        =   53
      _ExtentY        =   53
      _Version        =   393216
      BeginProperty Tabs {1EFB6598-857C-11D1-B16A-00C0F0283628} 
         NumTabs         =   1
         BeginProperty Tab1 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            ImageVarType    =   2
         EndProperty
      EndProperty
   End
   Begin VB.TextBox txtEditor 
      Height          =   8415
      Left            =   3000
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   360
      Width           =   9855
   End
   Begin VB.TextBox txtMod 
      Height          =   8415
      Left            =   3000
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   4
      Top             =   360
      Width           =   9855
   End
   Begin VB.Label lblAVIS 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00800000&
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   0
      Width           =   11655
   End
   Begin VB.Menu scnMnuPos 
      Caption         =   "ONTOP"
      Begin VB.Menu mnuTopA 
         Caption         =   "Active"
      End
   End
   Begin VB.Menu mnuMercGWindow 
      Caption         =   "Scan"
   End
   Begin VB.Menu txtmnuEdit 
      Caption         =   "Editor"
      WindowList      =   -1  'True
      Begin VB.Menu mnuCYHY 
         Caption         =   "CYHY"
      End
      Begin VB.Menu mnuMERCG 
         Caption         =   "Merc-G"
      End
   End
End
Attribute VB_Name = "frmScanOutput"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private mActiveEditorHwnd As Long

Private Sub CYHY_TIMER_Timer()
    If mTimerID = 0 Then
        CYHY_TIMER_CLOSE = False
        CYHY_TIMER_WAIT = True
        mTimerID = SetTimer(0, 0, 120, AddressOf CYHY_TimerProc)
    End If
End Sub

Private Sub Form_Load()
    AOT.UpdateStatus Me.hWnd
    txtEditor.Text = AVIS_BUFF
    lblAVIS.Caption = "TopMost: " & IIf(AOT.IsTopMost, "YES", "NO")
End Sub

Private Sub Form_Unload(Cancel As Integer)
    CYHY_TIMER.Enabled = False

    If mTimerID <> 0 Then
        CYHY_TIMER_CLOSE = True
        KillTimer 0, mTimerID
        mTimerID = 0
    End If
End Sub

Private Sub mnuCYHY_Click()
    txtMod.ZOrder 0

    AVIS_BUFF = txtEditor.Text

    If AVIS.HasAVIS Then
        txtMod.Text = AVIS.GetAVISBlock
    End If
End Sub

Private Sub mnuMERCG_Click()
    txtEditor.ZOrder 0
End Sub

Private Sub mnuTopA_Click()
    mnuTopA.Checked = Not mnuTopA.Checked

    If mnuTopA.Checked Then
        AOT.MakeTopMost Me.hWnd
    Else
        AOT.RemoveTopMost Me.hWnd
    End If

    AOT.UpdateStatus Me.hWnd
    lblAVIS.Caption = "TopMost: " & IIf(AOT.IsTopMost, "YES", "NO")
End Sub

Private Sub mnuMercGWindow_Click()
    If mTimerID <> 0 Then
        CYHY_TIMER_CLOSE = True
        KillTimer 0, mTimerID
        mTimerID = 0
    End If

    frmScanOutput.tvEditors.Nodes.Clear

    DesktopScan.ScanDesktop
    MercG.LoadTreeFromScanner DesktopScan

    Set frmMercG.MercG = MercG
    Set frmMercG.DesktopScan = DesktopScan

    frmMercG.Show vbModal
End Sub

Public Sub SelectEditorNode(ByVal Node As MSComctlLib.Node)
    Dim h As Long
    h = CLng(Node.Tag)

    mActiveEditorHwnd = h
    FS.TargetHwnd = h
End Sub

Private Sub tvEditors_NodeClick(ByVal Node As MSComctlLib.Node)
    SelectEditorNode Node
End Sub

Private Sub tvEditors_DblClick()
    If Not tvEditors.SelectedItem Is Nothing Then
        SelectEditorNode tvEditors.SelectedItem
    End If
End Sub

Public Sub UpdateActiveEditor()
    txtEditor.Text = FS.FocusedText
End Sub

Private Sub txtEditor_Change()
    AVIS_BUFF = frmScanOutput.txtEditor.Text
End Sub

