#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include "SanitizeVBScript.h"

int checkUpdate();
int downloadVBS(int VBS_num);
int downloadVBS(int VBS_num);

//LPCWSTRなどのWideを考慮していないため修正の可能性あり

//サーバーから更新履歴をチェック
int checkUpdate(){
    
    char clientVersion[] = "20251218_01";
    char serverURL[]="https://server-com-port-event-listener.vercel.app";
    char serverAdress[]="server-com-port-event-listener.vercel.app";
    HANDLE hInternet = NULL;

    //サーバーとの接続確認
    //接続確立
    if(InternetCheckConnection(serverURL, FLAG_ICC_FORCE_CONNECTION, 0)){
        hInternet = InternetOpen(
            "COM_EventListener",
            INTERNET_OPEN_TYPE_PRECONFIG,
            NULL,
            NULL,
            0//非同期通信をするならここを変更
        );
    }else{
        printf("Server Offline\n");
    }
    
    HANDLE hConnect = NULL;
    if(hInternet != NULL){
        hConnect = InternetConnect(
            hInternet,
            serverAdress,
            INTERNET_DEFAULT_HTTPS_PORT,
            NULL,//公開APIなのでuserName,PasswordはNULL
            NULL,
            INTERNET_SERVICE_HTTP,
            0,//dwFlag
            0//dwContext 非同期通信でどのリクエストに対する返答かを識別
        );
    }

    char httpObjectName[1024] = "";
    snprintf(httpObjectName, sizeof(httpObjectName), "/api/check-update?version=%s", clientVersion);
    HANDLE hRequest = NULL;
    if(hConnect != NULL){
        hRequest = HttpOpenRequest(
            hConnect,
            NULL,
            httpObjectName,
            NULL,//HTTP ver.default
            NULL,
            NULL,
            INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD,
            0 //dwContext
        );
    }
    BOOL RequestComplete = FALSE;
    if(hRequest != NULL){ 
        RequestComplete = HttpSendRequest(
            hRequest,
            NULL,
            0,
            NULL,
            0
        );
    }
    if(RequestComplete){
        printf("RequestComplete\n");
    }
    //更新があった場合に個別でダウンロードを呼び出し

    //ダウンロード完了後にはHandleをクローズ
    //開いたのとは逆順で行う
    if(hRequest != NULL){
        InternetCloseHandle(hRequest);
    }
    if(hConnect != NULL){
        InternetCloseHandle(hConnect);
    }
    if(hInternet!= NULL){
        InternetCloseHandle(hInternet);
    }
    return 0;
}

//VBScriptの番号に対応したファイルをダウンロード
int downloadVBS(int VBS_num){
    
    char* downloadText = NULL;//仮
    if(sanitizeVBScript(downloadText)){
        printf("Corrupted VBScript detected!\n");
        return 1; //不審なスクリプトがあった場合には中止
    }
    return 0;
}

//テキスト状態からVBSへ変換
//無毒化のフェーズで問題があったら中止
int convertVBS(){

}

