#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"bigInt.h"

//Approx 25MB.
#define MAX_SIZE 100000000 

int main(){
    char *str1 = (char*) malloc(sizeof(char) * MAX_SIZE);
    char *str2 = (char*) malloc(sizeof(char) * MAX_SIZE);
    
    //char str1[1000] = "119876789992983827672357892475983247583245892374589732458723985793284";
    //char str2[1000] = "201328749837295873429857589342759802374589723456348756893246758324765";

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
    /*bigInt *num3 = bigIntSub(num1, num2);
    bigInt *num4 = bigIntAdd(num1, num2);
    bigInt *num5 = bigIntDiv(num1, num2);*/
    clock_t time1, time2;
    time1 = clock();
    bigInt *num6 = bigIntMul(num1, num2);
    time1 = clock() - time1;
    time2 = clock();
    bigInt *num7 = bigIntFastMul(num1, num2);
    time2 = clock() - time2;

    /*printf("Num1: ");
    bigIntPrint(num1);
    printf("Num2: ");
    bigIntPrint(num2);
    printf("Sub: ");
    bigIntPrint(num3);
    printf("Add: ");
    bigIntPrint(num4);
    printf("Divison: ");
    bigIntPrint(num5);*/
    printf("Product: ");
    bigIntPrint(num6);
    printf("ProductFast: ");
    bigIntPrint(num7);
    printf("Simple Mul time: %f\n", (float)time1/CLOCKS_PER_SEC);
    printf("Karatsuba  time: %f\n", (float)time2/CLOCKS_PER_SEC);
    //bigInt *num7 = bigIntPow(num1, num2);
    //printf("Power: ");
    //bigIntPrint(num7);

    return 0;
}
