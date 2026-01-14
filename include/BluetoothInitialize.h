#include <windows.h>
#include <bluetoothapis.h>

#ifndef BluetoothInitialize
#define BluetoothInitialize

int initializeBluetooth();
int findBluetoothDevice(const wchar_t* targetName, BLUETOOTH_DEVICE_INFO* outInfo);
int setupSPP(BLUETOOTH_DEVICE_INFO deviceInfo);
#endif