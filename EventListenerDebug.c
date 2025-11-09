#include <stdio.h>
#include <windows.h>

#define COM_PORT "\\\\.\\COM3" //ポート番号をセット
#define BAUD 9600 //通信速度[bps]

int main() {
    printf("EventListenerDebug.c loaded\n");
    
    HANDLE handlePort;
    handlePort = CreateFile(
        COM_PORT,       //Filename
        GENERIC_READ,   //dwDesiredAccess
        0,              //dwShareMode
        NULL,           //lpSecurityAttributes
        OPEN_EXISTING,  //dwCreationDisposition
        0,              //dwFlagsAndAttributes
        NULL            //hTemplateFile
    ); 
    char received_char;

    int eventLoopFlag = 1;
    if (handlePort == INVALID_HANDLE_VALUE) {
        printf("Port %s opened failed\n", COM_PORT);
        eventLoopFlag = 0;
    }else{
        printf("Port %s opened successfully\n", COM_PORT);
    }
    
    DWORD dwEvtMask;//イベント状態
    DWORD dwBytesRead;//ReadFileで読み込んだバイト数

    //通信設定
    DCB dcbSerialParams = { 0 };//DCBのメンバを全て0で初期化
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);//データ構造体の長さ
    GetCommState(handlePort, &dcbSerialParams);
    dcbSerialParams.BaudRate = BAUD;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(handlePort, &dcbSerialParams);//設定をポートに反映

    //タイムアウト設定
    COMMTIMEOUTS timeouts = { 0 };//COMMTIMEOUTSメンバを全て０で初期化
    timeouts.ReadIntervalTimeout = MAXDWORD;
    SetCommTimeouts(handlePort, &timeouts);//timeout=0
    SetCommMask(handlePort, EV_RXCHAR);//EV_RXCHARを検知したときにhandlePortを呼び出す

    // イベントリスナーのメインループ
    while(eventLoopFlag) {
        
        printf("Waiting for event...\n");
        if (WaitCommEvent(handlePort, &dwEvtMask, NULL)) {//WaitCommEventで一時休止
            
            // データを1byte読み込む
            if (ReadFile(handlePort, &received_char, 1, &dwBytesRead, NULL)) {
                printf("Signal detected!!\n");
                while(dwBytesRead) {
                    printf("Received char: %c\n", received_char);
                    ReadFile(handlePort, &received_char, 1, &dwBytesRead, NULL);
                }
            }
        }else {
            printf("Failed to WaitCommEvent\n");
            break;
        }
    }
    
    if (handlePort != INVALID_HANDLE_VALUE){
        CloseHandle(handlePort);
    }
    printf("Press ENTER key to exit...\n");
    getchar();
    return 0;
}