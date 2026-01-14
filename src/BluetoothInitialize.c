#include <windows.h>
#include <bluetoothapis.h>
#include <string.h>

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
    if (findBluetoothDevice(bluetoothDeviceName, &deviceInfo) == 0) {
        printf("Device not found.\n");
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
        10, // *1.28 seconds timeout
        NULL
    };

    BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };
    HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&searchParams, &deviceInfo);

    if (hFind == NULL) return 1;

    do {
        // 名前を比較 (ワイド文字列なので wcscmp を使用)
        if (wcscmp(deviceInfo.szName, targetName) == 0) {
            *outInfo = deviceInfo; // 見つかったら情報をコピー
            BluetoothFindDeviceClose(hFind);
            return TRUE;
        }
    } while (BluetoothFindNextDevice(hFind, &deviceInfo));

    BluetoothFindDeviceClose(hFind);
    return 0;
}