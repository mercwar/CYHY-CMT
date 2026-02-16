VERSION 5.00
Begin VB.Form frmMercG 
   Caption         =   "Form1"
   ClientHeight    =   3165
   ClientLeft      =   165
   ClientTop       =   810
   ClientWidth     =   7530
   LinkTopic       =   "Form1"
   ScaleHeight     =   3165
   ScaleWidth      =   7530
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox lvWindows 
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   3135
      Left            =   3480
      ScaleHeight     =   3075
      ScaleWidth      =   3915
      TabIndex        =   1
      Top             =   0
      Width           =   3975
   End
   Begin VB.PictureBox tvWindows 
      Height          =   3135
      Left            =   0
      ScaleHeight     =   3075
      ScaleWidth      =   3315
      TabIndex        =   0
      Top             =   0
      Width           =   3375
   End
   Begin VB.Menu cmdOK 
      Caption         =   "Register Windows"
   End
End
Attribute VB_Name = "frmMercG"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public MercG As clsMercG
Public DesktopScan As clsWindowTreeScanner

Private Sub Form_Load()
    Dim i As Long
    Dim w As modGF.WINDOWINFO_UDT
    Dim nodeKey As String
    Dim parentKey As String

    MercG.LoadTreeFromScanner DesktopScan

    tvWindows.Nodes.Clear

    For i = 0 To MercG.GetTreeCount - 1
        If MercG.GetTreeItem(i) Then

            w = modGF.GF.WINDOWINFO_UDT

            nodeKey = "W" & CStr(w.hWnd)
            parentKey = "W" & CStr(w.Parent)

            If w.Parent = 0 Then
                tvWindows.Nodes.Add , , nodeKey, w.ClassName & " [" & w.hWnd & "]"
            Else
                On Error Resume Next
                tvWindows.Nodes.Add parentKey, tvwChild, nodeKey, w.ClassName & " [" & w.hWnd & "]"
                On Error GoTo 0
            End If

        End If
    Next i

    ' ? FIX: VB6 ListView MUST use With block here
    With lvSelected.ColumnHeaders
        .Clear
        .Add , , "hWnd", 1200
        .Add , , "Class", 2000
        .Add , , "Text", 4000
    End With
End Sub


Private Sub tvWindows_DblClick()
    Dim n As Node
    Set n = tvWindows.SelectedItem
    If n Is Nothing Then Exit Sub

    Dim h As Long
    h = CLng(Mid$(n.Key, 2))

    Dim i As Long
    For i = 0 To MercG.GetTreeCount - 1
        If MercG.GetTreeItem(i) Then

            Dim w As modGF.WINDOWINFO_UDT
            w = modGF.GF.WINDOWINFO_UDT

            If w.hWnd = h Then
                Dim it As ListItem
                Set it = lvSelected.ListItems.Add(, , CStr(w.hWnd))
                it.SubItems(1) = w.ClassName
                it.SubItems(2) = w.WindowText
                Exit For
            End If

        End If
    Next i
End Sub

Private Sub lvSelected_DblClick()
    If lvSelected.SelectedItem Is Nothing Then Exit Sub
    lvSelected.ListItems.Remove lvSelected.SelectedItem.Index
End Sub

Private Sub cmdOK_Click()
    Dim i As Long
    Dim it As ListItem

    ' Register all selected windows
    For i = 1 To lvSelected.ListItems.Count
        Set it = lvSelected.ListItems(i)
        Dim h As Long
        h = CLng(it.Text)

        Dim j As Long
        For j = 0 To MercG.GetTreeCount - 1
            If MercG.GetTreeItem(j) Then
                If modGF.GF.WINDOWINFO_UDT.hWnd = h Then
                    MercG.RegisterWindow 1
                    Exit For
                End If
            End If
        Next j
    Next i

    Unload Me
End Sub

Private Sub cmdCancel_Click()
    Unload Me
End Sub

