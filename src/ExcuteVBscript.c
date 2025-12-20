#include <stdio.h>
#include <windows.h>

int excuteVBScript(char macroName[200]){
    char currentPath[MAX_PATH];
    GetModuleFileName(NULL, currentPath, MAX_PATH);
    
    char cmd[1024];//サイズを十分の大きい値に設定
    char arg1[480];

    char projectDirectory,macroBookPath;
    char vbScriptPath;
    sprintf(vbScriptPath, "%s/src/ExcuteVBA.vbs",projectDirectory);
    sprintf(macroBookPath,"%s/bin/MacroBook.xlsm", projectDirectory);
    
    sprintf(cmd, "cscript //nologo \"%s\" \"%s\" \"%s\"", vbScriptPath, macroBookPath, macroName);
    //引き渡し先VBScriptの引数 は第一引数がMacroWorkBookPathである
    
    system(cmd);
    return 0;
}