#include <windows.h>
#include <bluetoothapis.h>
#include <string.h>
#include <shlwapi.h>
#include <stdio.h>
#include "ExcuteEXE.h"

int initializeBluetooth();
int findBluetoothDevice(const wchar_t* targetName, BLUETOOTH_DEVICE_INFO* outInfo);
int setupSPP(BLUETOOTH_DEVICE_INFO deviceInfo);

int initializeBluetooth() {

    const wchar_t* bluetoothDeviceName = L"Experiment_Module_01";
    
    HANDLE hRadio = NULL;
    BLUETOOTH_FIND_RADIO_PARAMS btFindRadioParams = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
    HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio(&btFindRadioParams, &hRadio);
    
    if (hFind == NULL) {
        printf("No Bluetooth radios found.\n");
        return 1;
    }


    BluetoothFindRadioClose(hFind);
    CloseHandle(hRadio);

    BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };
    if (hFind != NULL && findBluetoothDevice(bluetoothDeviceName, &deviceInfo) == 0) {
        printf("Bluetooth device not found.\n");
        return 1;
    }

    return 0;
}

int findBluetoothDevice(const wchar_t* targetName, BLUETOOTH_DEVICE_INFO* outInfo) {
    BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = {
        sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
        TRUE,
        TRUE,
        TRUE,
        TRUE,
        TRUE,
        10, // 10 * 1.28 seconds timeout
        NULL
    };

    BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };
    HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&searchParams, &deviceInfo);

    if (hFind == NULL) return 1;

    do {
        //名前を比較(ワイド文字列なので wcscmp を使用))
        if (wcscmp(deviceInfo.szName, targetName) == 0) {
            *outInfo = deviceInfo; // 見つかったら情報をコピー
            BluetoothFindDeviceClose(hFind);
            return 1;
        }
    } while (BluetoothFindNextDevice(hFind, &deviceInfo));

    //デバイスが見つからなかった場合0を返す
    BluetoothFindDeviceClose(hFind);
    return 0;
}

int setupSPP(BLUETOOTH_DEVICE_INFO deviceInfo){
    //外部のSPPセットアップ関数を呼び出す
    //外部exeファイルとすることで管理者権限を物理的に分離

    char currentPath[MAX_PATH], projectDirectoryPath[MAX_PATH];

    GetModuleFileName(NULL,currentPath,MAX_PATH);//第一引数HMODULEがNULLなら現在のプロセスのPATHを取得し第二引数にコピー
    strcpy(projectDirectoryPath,currentPath);
    PathRemoveFileSpec(projectDirectoryPath);//コピー元のcurentPathから実行ファイル名を削る
    PathRemoveFileSpec(projectDirectoryPath);//コピー元のcurentPathから実行ファイルの親ディレクトリ(bin)を削る

    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "%s\\src\\BluetoothSPP_Setup.exe \"%04X%08X\"", projectDirectoryPath, &deviceInfo);
    excuteEXE(cmd);

    return 0;
}