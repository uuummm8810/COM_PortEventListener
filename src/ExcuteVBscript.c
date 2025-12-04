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
    sprintf(cmd, "cscript //nologo %260s/src/\"%200s\" \"%260s\"", currentPath,filename,currentPath);

    system(cmd);
    return 0;
}