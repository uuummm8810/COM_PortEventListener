#include "CommonObjectControler.h"

int signalCharJudgment(char signal_char){

    switch (signal_char){
    case '0':
        comControlerVBA("sample.vba");

        return 0;
    
    default:
        break;
    }

    //signal_charをもとにコードが実行されないのなら1を戻す
    return 1;
}