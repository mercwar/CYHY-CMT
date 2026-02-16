VERSION 5.00
Begin VB.Form frmNavigator 
   Caption         =   "GODLY AIFVS Navigator"
   ClientHeight    =   2340
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7065
   LinkTopic       =   "frmNavigator"
   ScaleHeight     =   2340
   ScaleWidth      =   7065
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox Text1 
      Alignment       =   2  'Center
      Height          =   1455
      Left            =   360
      MultiLine       =   -1  'True
      TabIndex        =   0
      Top             =   240
      Width           =   6375
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "RIGHT CLICK ON THE FORM"
      Height          =   255
      Left            =   360
      TabIndex        =   1
      Top             =   1920
      Width           =   2775
   End
End
Attribute VB_Name = "frmNavigator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    Me.ScaleMode = vbPixels
    Text1.Text = CVBGOD_GREETING
End Sub

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button = vbRightButton Then
       ShowNavPopupAtMouse hWnd, X, Y
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Nav_Terminate
End Sub

