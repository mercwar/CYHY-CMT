VERSION 5.00
Begin VB.Form frmScanOutput 
   Caption         =   "CYHY Comment Program"
   ClientHeight    =   5415
   ClientLeft      =   165
   ClientTop       =   810
   ClientWidth     =   6570
   LinkTopic       =   "Form1"
   ScaleHeight     =   5415
   ScaleWidth      =   6570
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtLog 
      Height          =   4935
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   0
      Width           =   6495
   End
   Begin VB.Label lblTopStatus 
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
      Top             =   5040
      Width           =   6375
   End
   Begin VB.Menu scnMnuPos 
      Caption         =   "ONTOP"
      Begin VB.Menu mnuTopA 
         Caption         =   "Active"
      End
   End
End
Attribute VB_Name = "frmScanOutput"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public AOT As New clsAlwaysOnTop

Private Sub Form_Load()
    FS.AttachTextBox txtLog
    AOT.UpdateStatus Me.hWnd
    lblTopStatus.Caption = "TopMost: " & IIf(AOT.IsTopMost, "YES", "NO")
End Sub

Private Sub tmrUpdate_Timer()
    ' If you keep the global timer, you can disable this.
    ' Or call FS.UpdateTick here instead of the API timer.
End Sub

Private Sub mnuTopA_Click()
    mnuTopA.Checked = Not mnuTopA.Checked

    If mnuTopA.Checked Then
        AOT.MakeTopMost Me.hWnd
    Else
        AOT.RemoveTopMost Me.hWnd
    End If

    AOT.UpdateStatus Me.hWnd
    lblTopStatus.Caption = "TopMost: " & IIf(AOT.IsTopMost, "YES", "NO")
End Sub

