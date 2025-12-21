#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

int excuteVBScript(char macroName[200]){
    char cmd[1024];//サイズを十分の大きい値に設定
    char currentPath[MAX_PATH], projectDirectoryPath[MAX_PATH], macroBookPath[MAX_PATH], vbScriptPath[MAX_PATH];    

    GetModuleFileName(NULL,currentPath,MAX_PATH);//第一引数HMODULEがNULLなら現在のプロセスのPATHを取得し第二引数にコピー
    strcpy(projectDirectoryPath,currentPath);
    PathRemoveFileSpec(projectDirectoryPath);//コピー元のcurentPathから実行ファイル名を削る
    PathRemoveFileSpec(projectDirectoryPath);//コピー元のcurentPathから実行ファイルの親ディレクトリ(bin)を削る

    //下記のsprintfはprojectDirectoryPathが長すぎると正常に動作しない
    //projectファイルをあまりディレクトリの入れ子とならないようにすること
    sprintf(vbScriptPath, "%240s/src/ExcuteVBA.vbs",projectDirectoryPath);
    sprintf(macroBookPath,"%240s/bin/MacroBook.xlsm", projectDirectoryPath);
    
    //引き渡し先VBScriptの引数 は第一引数がMacroWorkBookPathである
    sprintf(cmd, "cscript //nologo \"%s\" \"%s\" \"%s\"", vbScriptPath, macroBookPath, macroName);
    system(cmd);
    
    return 0;
}