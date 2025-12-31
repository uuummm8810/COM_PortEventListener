#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//ここで危険な文字列がないかチェック
//不審なものを検出した場合には1をreturn
int sanitizeVBScript(char text[]){
    //(比較用にすべて小文字で定義)
    const char* blacklist[] = {
        "createobject",
        "wscript.shell",
        "filesystemobject",
        "execute",
        "eval",
        "deletefile",
        "adodb.stream",
        NULL //配列の終わりを示す
    };

    if (text == NULL) {
        return 0;
    }

    //大文字小文字を無視するために、作業用バッファを作って小文字化する
    size_t len = strlen(text);
    char* lowerText = (char*)malloc(len + 1);
    if (lowerText == NULL) return 1; //メモリ確保失敗時は安全のためエラー扱い

    for (size_t i = 0; i < len; i++) {
        lowerText[i] = (char)tolower((unsigned char)text[i]);
    }
    lowerText[len] = '\0';

    int result = 0;
    for (int i = 0; blacklist[i] != NULL; i++) {
        if (strstr(lowerText, blacklist[i]) != NULL) {
            result = 1; //禁止ワード検出
            break;
        }
    }

    free(lowerText);
    lowerText = NULL;
    return result;
}