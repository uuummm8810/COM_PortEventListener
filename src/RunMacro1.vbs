Dim excel
Dim macroWorkBookPath
Dim targetWorkBook

Dim args
Set args = WScript.Arguments

macroWorkBookPath=args(0)

Set excel = GetObject(, "Excel.Application") ' 起動中のExcelを取得
excel.ScreenUpdating = False



excel.ScreenUpdating = True

excel.Run "'" & macroWb.Name & "'!Macro1", targetWorkBook.Name ' マクロ実行

