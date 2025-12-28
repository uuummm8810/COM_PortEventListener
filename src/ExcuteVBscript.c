#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

int excuteVBScript(char macroName[200]);
void CALLBACK OnProcessFinished(void* lpParameter, BOOLEAN TimerOrWaitFired);

HANDLE hWSHProcess;

//VBScript にて、{ProjectDirectory}/bin/MacroBookのマクロ{macroName}を実行
int excuteVBScript(char macroName[200]){
    char cmd[1024];//サイズを十分の大きい値に設定
    char currentPath[MAX_PATH], projectDirectoryPath[MAX_PATH], macroBookPath[MAX_PATH], vbScriptPath[MAX_PATH];    

    GetModuleFileName(NULL,currentPath,MAX_PATH);//第一引数HMODULEがNULLなら現在のプロセスのPATHを取得し第二引数にコピー
    strcpy(projectDirectoryPath,currentPath);
    PathRemoveFileSpec(projectDirectoryPath);//コピー元のcurentPathから実行ファイル名を削る
    PathRemoveFileSpec(projectDirectoryPath);//コピー元のcurentPathから実行ファイルの親ディレクトリ(bin)を削る

    //下記のsprintfはprojectDirectoryPathが長すぎると正常に動作しない
    //projectファイルをあまりディレクトリの入れ子とならないようにすること
    snprintf(vbScriptPath, sizeof(vbScriptPath), "%s\\src\\ExcuteVBA.vbs",projectDirectoryPath);
    snprintf(macroBookPath, sizeof(macroBookPath), "%s\\bin\\MacroBook.xlsm", projectDirectoryPath);
    
    //引き渡し先VBScriptの引数 は第一引数がMacroWorkBookPathである
    snprintf(cmd, sizeof(cmd), "cscript //nologo \"%s\" \"%s\" \"%s\"", vbScriptPath, macroBookPath, macroName);

    //CreateProcessのための初期化
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    //WSHでVBScript を実行
    if(CreateProcess(
        NULL,
        cmd,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &startupInfo,
        &processInfo
    )){
        CloseHandle(processInfo.hThread);
        HANDLE waitObject=NULL;
        DWORD dw5Second = 5000;
        RegisterWaitForSingleObject(
            &waitObject,
            processInfo.hProcess,
            OnProcessFinished,
            processInfo.hProcess,
            dw5Second,
            WT_EXECUTEONLYONCE
        );
    }else{
        return 1;//CreateProcess失敗時には1を返す
    }

    return 0;
}

void CALLBACK OnProcessFinished(void* lpParameter, BOOLEAN TimerOrWaitFired) {
    hWSHProcess = (HANDLE)lpParameter;

    DWORD exitCode;
    GetExitCodeProcess(hWSHProcess, &exitCode);
    printf("%lu",exitCode);
    CloseHandle(hWSHProcess);
}

//WSHがActiveかチェックするための関数
//WSHをクローズしたのちに偶然ほかのプロセスが同じHANDLEを利用していると不具合の可能性
BOOL checkWSHProcessActive(){
    DWORD exitCode;
    GetExitCodeProcess(hWSHProcess, &exitCode);
    if(exitCode == STILL_ACTIVE){
        return TRUE;    
    }else{
        return FALSE;
    }
}
