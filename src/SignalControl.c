#include <stdio.h>
#include <windows.h>

#include "CommonObjectControler.h"
#include "ExcuteVBScript.h"

//signal_charをもとにコードが実行されないのなら1を戻す
int signalCharJudgment(char signal_char){

    switch (signal_char){
        case '0':
            excuteVBScript("SumBelowBlank");
            return 0;

        case '1':
            excuteVBScript("AverageWithBlankCheck");
            return 0;
        
        case '2':
            excuteVBScript("LinEstAboveCell");
            return 0;

        case '3':
            excuteVBScript("CalcStdevPAboveCell");
            return 0;

        case '4':               
            excuteVBScript("SumSelectedCellsAndDisplayBelowo1");            
            return 0;

	 case '5':               
            excuteVBScript("AverageSelectedCellsAndDisplayBelow");            
            return 0;

	 case '6':               
            excuteVBScript("StandardDeviationSelectedCellsAndDisplayBelow");            
            return 0;

	 case '7':               
            excuteVBScript("LeastSquaresAndDisplayBelow");            
            return 0;

        default:
            break;
    }

    
    return 1;
}