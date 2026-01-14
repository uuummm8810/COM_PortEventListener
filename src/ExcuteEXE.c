#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>

void CALLBACK OnProcessFinished(void* lpParameter, BOOLEAN TimerOrWaitFired);

int excuteEXE(char* cmd){
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
    HANDLE hProcess = (HANDLE)lpParameter;

    DWORD exitCode;
    GetExitCodeProcess(hProcess, &exitCode);
    printf("%lu",exitCode);
    CloseHandle(hProcess);
    hProcess = NULL;
}

