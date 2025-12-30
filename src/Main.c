#include <windows.h>
#include <stdio.h>
#include "EventListenerDebug.h"
#include "ExcuteVBScript.h"
#include "DownloadVBScript.h"

int test();
int main();

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

