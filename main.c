#include<stdio.h>
#include<stdlib.h>

#include"bigInt.h"

//Approx 25MB.
#define MAX_SIZE 25000000

int main(){
    char *str1 = (char*) malloc(sizeof(char) * MAX_SIZE);
    char *str2 = (char*) malloc(sizeof(char) * MAX_SIZE);
    
    if(!str1){
        printf("Malloc Error\n");
        exit(0);
    }
        
    if(!str2){
        printf("Malloc Error str2\n");
        exit(0);
    }
    
    scanf("%s", str1);
    scanf("%s", str2);
    
    bigInt *num1 = bigIntStrToArr(str1);
    bigInt *num2 = bigIntStrToArr(str2);
    bigInt *num3 = bigIntSub(num1, num2);
    bigInt *num4 = bigIntAdd(num1, num2);
    bigInt *num5 = bigIntDiv(num1, num2);
    printf("Num1: ");
    bigIntPrint(num1);
    printf("Num2: ");
    bigIntPrint(num2);
    printf("Sub: ");
    bigIntPrint(num3);
    printf("Add: ");
    bigIntPrint(num4);
    printf("Division: ");
    bigIntPrint(num5);
}
