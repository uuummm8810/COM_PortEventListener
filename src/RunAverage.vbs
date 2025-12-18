Dim excel

Dim macroWorkBookPath, macroWorkBook
Dim targetWorkBook


' 引数のチェックをするエラーハンドリングを後で行うこと
Dim args
Set args = WScript.Arguments

macroWorkBookPath=args(0)

Set excel = GetObject(, "Excel.Application") ' 起動中のExcelを取得
Set targetWorkBook = excel.ActiveWorkbook

' エクセル画面更新をしばらく停止
excel.ScreenUpdating = False


Set macroWorkBook = excel.Workbooks.Open(macroWorkBookPath)

 ' マクロ実行
targetWorkBook.Activate
excel.Run "'" & macroWorkBook.Name & "'!AverageWithBlankCheck"

' マクロ用Workbookを閉じてから画面更新を再開してマクロWBを画面に見せない
macroWorkBook.Close False
excel.ScreenUpdating = True