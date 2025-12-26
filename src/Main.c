#include <windows.h>
#include <stdio.h>
#include "EventListenerDebug.h"
#include "ExcuteVBScript.h"

//test
int test(){
    excuteVBScript("Macro1");
    return 0;
}

int main(){
    //test();
    selectPort();

    return 0;
}

