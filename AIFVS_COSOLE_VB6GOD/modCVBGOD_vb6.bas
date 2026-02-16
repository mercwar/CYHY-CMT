Attribute VB_Name = "modCVBGOD_vb6"
Option Explicit
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Declare Function EbMode Lib "vba6.dll" () As Long

Public Function IDE_Break() As Boolean
    IDE_Break = (EbMode() = 2)
End Function


Public Sub VbSleep(cvbgods As Long)
DoEvents
Sleep 100
End Sub
