#include <windows.h>
#include <bluetoothapis.h>

int initializeBluetooth() {
    
    HANDLE hRadio = NULL;
    BLUETOOTH_FIND_RADIO_PARAMS btFindRadioParams = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
    HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio(&btFindRadioParams, &hRadio);
    
    if (hFind == NULL) {
        printf("No Bluetooth radios found.\n");
        return 1;
    }


    BluetoothFindRadioClose(hFind);
    CloseHandle(hRadio);

    return 0;
}