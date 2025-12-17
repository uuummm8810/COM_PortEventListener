#include <stdio.h>
#include <windows.h>

int excuteVBScript(char filename[200]){
    char currentPath[MAX_PATH];
    GetModuleFileName(NULL, currentPath, MAX_PATH);
    
    char cmd[1024];//サイズを十分の大きい値に設定
    char arg1[480];

    //変数の代入でエラーのため下記は一時停止
    //↓
    //arg1=vbsPath;
    
    // \"%s\" とすることで、実行時には "パス" として展開される
    //sprintf(cmd, "cscript //nologo %s/src/\"%s\" \"%s\"", currentPath,filename,currentPath);
    //引き渡し先VBScriptの引数 は第一引数がMacroWorkBookPathである
    
    //testコード
    //環境次第で動作しないので必ず変更すること
    //
    
    system("cscript //nologo C:/Users/uuumm/Documents/GitHub/COM_PortEventListener/src/RunMacro1.vbs C:\\Users\\uuumm\\Documents\\GitHub\\COM_PortEventListener\\bin\\MacroBook.xlsm");
    return 0;
}