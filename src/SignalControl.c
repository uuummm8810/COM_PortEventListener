#include <stdio.h>
#include <windows.h>

#include "CommonObjectControler.h"
#include "ExcuteVBScript.h"

int signalCharJudgment(char signal_char){

    switch (signal_char){
    case '0':
        system("cscript //nologo D:/COM_PortEventListener/src/RunMacro1.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm");
        return 0;
    case '1':
         system("cscript //nologo D:/COM_PortEventListener/src/RunAverage.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm");
        return 0;
    
    case '2':
        system("cscript //nologo D:/COM_PortEventListener/src/RunLinEstAboveCell.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm");
    
        return 0;

    case '3':
        system("cscript //nologo D:/COM_PortEventListener/src/RunCalcStdevPAboveCell.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm");
        return 0;

    case 'd':
        system("cscript //nologo D:/COM_PortEventListener/src/ExcuteVBA.vbs D:\\COM_PortEventListener\\bin\\MacroBook.xlsm Macro1");
        return 0;
    default:
    
        break;
    }

    //signal_charをもとにコードが実行されないのなら1を戻す
    return 1;
}

int signalMolsJudgment(int signalCount){
        switch (signalCount){
    case '0':
        comControlerVBA("sample.vba");

        return 0;
    
    default:
        break;
    }

    //signal_charをもとにコードが実行されないのなら1を戻す
    return 1;
}