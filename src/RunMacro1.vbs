Dim excel
Dim macroWorkBookPath, macroWB
Dim targetWorkBook

' 引数のチェックをするエラーハンドリングを後で行うこと
Dim args
Set args = WScript.Arguments

macroWorkBookPath=args(0)

Set excel = GetObject(, "Excel.Application") ' 起動中のExcelを取得
Set targetWorkBook = excel.ActiveWorkbook

' エクセル画面更新をしばらく停止
excel.ScreenUpdating = False

Set macroWB = excel.Workbooks.Open(macroWorkBookPath)

 ' マクロ実行
excel.Run "'" & macroWB.Name & "'!Macro1", targetWorkBook.Name

' マクロ用Workbookを閉じてから画面更新を再開してマクロWBを画面に見せない
macroWB.Close False
excel.ScreenUpdating = True