#include <stdio.h>

#define MAXSIZE 100

typedef struct{
    char ch[MAXSIZE + 1];
    int length;
}SString;

void InitString(SString *S){
    S->length = 0;
}

void ClearString(SString *S){
    S->length = 0;
}

int StrAssign(SString *S, char str[]){
    int i = 0;
    while(str[i] != '\0'){
        i++;
    }

    if(i > MAXSIZE){
        return 0;
    }

    S->length = i;
    for(i = 1; i <= S->length; i++){
        S->ch[i] = str[i - 1];
    }

    return 1;
}

void PrintString(SString S){
    int i;
    for(i = 1; i <= S.length; i++){
        printf("%c", S.ch[i]);
    }
    printf("\n");
}

int SubString(SString *Sub, SString S, int pos, int len){
    int i;

    if(pos < 1 || pos > S.length || len < 0 || pos + len - 1 > S.length){
        return 0;
    }

    for(i = 1; i <= len; i++){
        Sub->ch[i] = S.ch[pos + i - 1];
    }

    Sub->length = len;
    return 1;
}

int Index(SString S, SString T){
    int i, j;

    if(T.length == 0){
        return 1;
    }

    for(i = 1; i <= S.length - T.length + 1; i++){
        for(j = 1; j <= T.length; j++){
            if(S.ch[i + j - 1] != T.ch[j]){
                break;
            }
        }
        if(j > T.length){
            return i;
        }
    }

    return 0;
}

int StrDelete(SString *S, int pos, int len){
    int i;

    if(pos < 1 || pos > S->length || len < 0 || pos + len - 1 > S->length){
        return 0;
    }

    for(i = pos + len; i <= S->length; i++){
        S->ch[i - len] = S->ch[i];
    }

    S->length = S->length - len;
    return 1;
}

int main(){
    SString s, sub, t;

    InitString(&s);
    printf("初始化后串长：%d\n", s.length);

    StrAssign(&s, "abcdefg");
    printf("赋值后串为：");
    PrintString(s);

    if(SubString(&sub, s, 2, 3)){
        printf("求得子串为：");
        PrintString(sub);
    }else{
        printf("求子串失败\n");
    }

    StrAssign(&t, "cde");
    printf("子串cde第一次出现的位置是：%d\n", Index(s, t));

    if(StrDelete(&s, 3, 2)){
        printf("删除后串为：");
        PrintString(s);
    }else{
        printf("删除失败\n");
    }

    ClearString(&s);
    printf("清空后串长：%d\n", s.length);

    return 0;
}
