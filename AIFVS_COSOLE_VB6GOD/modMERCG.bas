Attribute VB_Name = "modMERCG"
' ============================================================
' AVIS.STRUCT.MERCG_REG_WINDOW
' ROLE:
'   - Atomic registry entry for MercG supported windows
'   - Stored in modMERCG.MERCG_REG()
'   - Returned through modGF.MERCG_REG_BUFFER (GF-law)
'   - No class returns; no UDT returns; no direct array exposure
' ============================================================
Public Type MERCG_REG_WINDOW
    hWnd        As Long      ' AVIS: Window handle
    ClassName   As String    ' AVIS: Win32 class
    WindowText  As String    ' AVIS: Caption/text snapshot
    REG_TYPE    As Long      ' AVIS: Registration type (user-defined)
End Type

