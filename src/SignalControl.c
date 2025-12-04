#include "CommonObjectControler.h"
#include "ExcuteVBScript.h"

int signalCharJudgment(char signal_char){

    switch (signal_char){
    case '0':
        excuteVBScript("./bin.RunMacro1.vbs");

        return 0;
    case '1':
        comControlerVBA("sample.vba");

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