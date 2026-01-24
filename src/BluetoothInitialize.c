#include <windows.h>
#include <bluetoothapis.h>
#include <string.h>
#include <shlwapi.h>
#include <stdio.h>
#include <devguid.h>
#include <setupapi.h>
#include "EventListenerDebug.h"
#include "ExcuteEXE.h"

int initializeBluetooth();
int findBluetoothDevice(const wchar_t* targetName, BLUETOOTH_DEVICE_INFO* outInfo);
int setupSPP(BLUETOOTH_DEVICE_INFO deviceInfo);
BOOL findComPortByBthAddress(const char* targetAddr, char* outPort, size_t outSize);

int initializeBluetooth() {

    const wchar_t* bluetoothDeviceName = L"Experiment_Module_01";
    
    HANDLE hRadio = NULL;
    BLUETOOTH_FIND_RADIO_PARAMS btFindRadioParams = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
    HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio(&btFindRadioParams, &hRadio);
    
    if (hFind == NULL) {
        printf("Bluetooth is not available.\n");
        return 1;
    }


    BluetoothFindRadioClose(hFind);
    CloseHandle(hRadio);

    BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };
    if (findBluetoothDevice(bluetoothDeviceName, &deviceInfo)) {
        printf("Bluetooth device \"%S\" not found.\n", bluetoothDeviceName);
        return 1;
    }

    //fideBluetoothで取得したdeviceinfoとCOMポートのBluetoothアドレスが一致するか確認
    //デバイスが見つからなければSPPのセットアップ開始
    char targetAdress[128];
    snprintf(targetAdress, sizeof(targetAdress), "%012llX", deviceInfo.Address.ullLong);
    
    char outPort[16] = {0};
    size_t outSize = sizeof(outPort);
    
    // ポートが見つからない場合にセットアップを実行する
    if (!findComPortByBthAddress(targetAdress, outPort, outSize)) {
        printf("Start SPP setup...\n");
        setupSPP(deviceInfo);
        // セットアップ完了とOSの認識を待つためにリトライループを追加
        for (int i = 0; i < 5; i++) {
            Sleep(2000);
            if (findComPortByBthAddress(targetAdress, outPort, outSize)) break;
        }
    }

    if (outPort[0] != '\0') {
        char fullPortPath[32];
        snprintf(fullPortPath, sizeof(fullPortPath), "\\\\.\\%s", outPort);
        printf("Target device found on %s\n", fullPortPath);
        eventListenerDebug(fullPortPath);
    } else {
        printf("Failed to identify COM port for the device.\n");
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
            return 0;
        }
    } while (BluetoothFindNextDevice(hFind, &deviceInfo));

    //デバイスが見つからなかった場合1を返す
    BluetoothFindDeviceClose(hFind);
    return 1;
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
    snprintf(cmd, sizeof(cmd), "%s\\src\\BluetoothSPP_Setup.exe \"%012llX\"", projectDirectoryPath, deviceInfo.Address.ullLong);
    excuteEXE(cmd);

    return 0;
}

//targetAddrは16進数に変換しておくこと
BOOL findComPortByBthAddress(const char* targetAddr, char* outPort, size_t outSize) {
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA devInfoData;
    char buffer[1024];
    BOOL found = FALSE;

    //COMポートをすべて列挙しリスト化
    hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE) return FALSE;

    //フレンドリー名からCOMポートの番号を文字列比較によって取り出す
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); i++) {
        // ハードウェアIDを取得
        if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_HARDWAREID, NULL, (PBYTE)buffer, sizeof(buffer), NULL)) {
            _strupr(buffer); // 大文字に変換して比較を確実にする
            if (strstr(buffer, targetAddr) != NULL) {
                if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)buffer, sizeof(buffer), NULL)) {
                    char* comStart = strstr(buffer, "(COM");
                    if (comStart) {
                        comStart++;
                        comStart++;
                        comStart++;
                        comStart++; // '(COM' を飛ばしてCOMの番号 "x" から開始
                        char* comEnd = strchr(comStart, ')');
                        if (comEnd) {
                            size_t len = comEnd - comStart;
                            if (len < outSize) {
                                strncpy(outPort, comStart, len);
                                outPort[len] = '\0';
                                found = TRUE;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return found;
}