#include <windows.h>
#include <stdio.h>
#include "EventListenerDebug.h"
#include "ExcuteVBScript.h"

//test
int test(){
    //system("cscript //nologo C:\\Users\\uuumm\\Documents\\GitHub\\COM_PortEventListener\\src\\ExcuteVBA.vbs C:\\Users\\uuumm\\Documents\\GitHub\\COM_PortEventListener\\bin\\MacroBook.xlsm Macro1");
    excuteVBScript("Macro1");
    return 0;
}

int main(){
    //test();
    selectPort();

    return 0;
}

