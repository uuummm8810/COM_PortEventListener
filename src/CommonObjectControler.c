#include <Windows.h>
#include <stdio.h>

int comControlerVBA(char vbaPath[]){//pythonで作成したCOM操作を呼び出す
    
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;
    char const COM_CONTROLE_EXE_PATH;//EXEファイルのパスを指定(未定義)

    char cmd[256];
    sprintf(cmd,"%s arg1 %s",COM_CONTROLE_EXE_PATH, vbaPath);//後でsnprintfでオーバーランを防ぐように変更
    
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    if(!CreateProcess(//CreateProcessは失敗した場合戻り値0
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
        //失敗時の戻り値
        return 1;
    }

    // プロセスの終了を待つ
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    // ハンドルを閉じる
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    
    return 0;
}