#include <stdio.h>
#include <windows.h>

#include "CommonObjectControler.h"
#include "ExcuteVBScript.h"

int signalCharJudgment(char signal_char){

    switch (signal_char){
        case '0':
            system("cscript //nologo D:/COM_PortEventListener/src/ExcuteVBA.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm SumBelowBlank");
            return 0;
            
        case '1':
            system("cscript //nologo D:/COM_PortEventListener/src/ExcuteVBA.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm AverageWithBlankCheck");
            return 0;
        
        case '2':
            system("cscript //nologo D:/COM_PortEventListener/src/ExcuteVBA.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm LinEstAboveCell");
            return 0;
        
        case '3':
            system("cscript //nologo D:/COM_PortEventListener/src/ExcuteVBA.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm CalcStdevPAboveCell");
            return 0;
        
        case 'd':
            system("cscript //nologo D:/COM_PortEventListener/src/ExcuteVBA.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm Macro1");
            return 0;
        
        default:
            break;
    }

    //signal_charをもとにコードが実行されないのなら1を戻す
    return 1;
