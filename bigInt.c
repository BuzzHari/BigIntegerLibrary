#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"bigInt.h"

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

bigInt* bigIntStrToArr(const char *str){
    if(!str)
        return NULL;
    bigInt *newNum = (bigInt*) malloc(sizeof(bigInt)*1);
    
    newNum->len = strlen(str);
    newNum->num = (char *) malloc(sizeof(char) * (newNum->len));
    if(!(newNum->num)){
        perror("Malloc Error");
        exit(0);
    }
    int i = 0;
    for(i = 0; i < newNum->len; i++){
        newNum->num[i] = str[i] - '0';
    }
    return newNum;
}

void addZeroPadding(bigInt *num, long long int offset){
    long long int newLen = num->len + offset;
    char *tempNum = (char*) malloc(sizeof(char)*(newLen));
    long long int i = 0;
    while(i < offset)
        tempNum[i++] = 0;
    int j = 0;
    while(i < newLen){
        tempNum[i] = num->num[j];
        i++;
        j++;
    }

    free(num->num);
    num->num = tempNum;
    num->len = newLen;
}

void removeZeroPadding(bigInt *num){
    long long int i = 0;
    char *tempNum;
    while(num->num[i] == 0 && i < num->len)
        i++;
    
    if(i == num->len){
        tempNum = (char*) malloc(sizeof(char)*(1));
        tempNum[0] = 0;
        num->len = 1;
    }
    else{
        long long int newLen = num->len - i;
        tempNum = (char*) malloc(sizeof(char)*(newLen));
        long long int j = 0;
        while(i < num->len){
            tempNum[j] = num->num[i];
            j++;
            i++;
        }
        num->len = newLen;
    } 
    
    free(num->num);
    num->num = tempNum;
}

void bigIntPrint(bigInt *num){
    if(num == NULL){
        printf("NaN\n");
        return;
    }

    for(int i = 0; i < num->len; i++){
        printf("%d", num->num[i]);
    }
    printf("\n");
}

/*
 * Returns:
 *   1 - if num1 > num2
 *  -1 - if num1 < num2
 *   0 - if num1 = num2
 */
int bigIntCompare(bigInt *num1, bigInt *num2){
    if(num1->len > num2->len)
        return 1;
    if(num1->len < num2->len)
        return -1;
    
    for(int i = 0; i < num1->len; i++){
        if(num1->num[i] > num2->num[i])
            return 1;
        else if(num1->num[i] < num2->num[i])
            return -1;
    }

    return 0;
}

bigInt* bigIntSub(bigInt *num1, bigInt *num2){
    if(num1 == NULL || num2 == NULL)
        return NULL;
    
    //to remove the uncessary zeros if present.
    removeZeroPadding(num1);
    removeZeroPadding(num2);
    
    long long int len = MAX(num1->len, num2->len);
    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
    result->len = 0;
    result->num = (char *) calloc(len, sizeof(char)); 
    if(!(result->num)){
        perror("Calloc Error");
        exit(0);
    } 

    char *bignum, *smallnum, p_flag = -1;
    if(num1->len > num2->len){
        addZeroPadding(num2,num1->len - num2->len);
        p_flag = 2;
        bignum = num1->num;
        smallnum = num2->num;
    }
    else if(num1->len < num2->len){
        addZeroPadding(num1, num2->len - num1->len);
        p_flag = 1;
        bignum = num2->num;
        smallnum = num1->num;
    }
    else{
        if(bigIntCompare(num1, num2) == 1){
            bignum = num1->num;
            smallnum = num2->num;
        }
        else{
            bignum = num2->num;
            smallnum = num1->num;
        }
    }
    char carry = 0, temp;
    for(long long int i = len-1; i >= 0; i--){
        temp = bignum[i]; 
        if(carry){
            temp -= 1;
            carry = 0;
        }
        if(temp < smallnum[i]){
            temp+=10;
            carry = 1;
        }
        result->num[i] = temp - smallnum[i];
        result->len += 1;
    }
    
    if(p_flag == 1)
        removeZeroPadding(num1);
    else if(p_flag == 2)
        removeZeroPadding(num2);
    removeZeroPadding(result);
    return result;

}

bigInt* bigIntAdd(bigInt* num1, bigInt* num2)
{
    if(num1 == NULL || num2 == NULL)
        return NULL;
    
    //to remove the uncessary zeros if present.
    removeZeroPadding(num1);
    removeZeroPadding(num2);
    
    char *bignum, *smallnum;
    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
    result->len = 0;
    long int size = num1->len;
    
    int p_flag = -1;
    if (num2->len > num1->len){
        addZeroPadding(num1, num2->len - num1->len);
        bignum = num2->num;
        smallnum = num1->num;
        size = num2->len;
        p_flag = 1;
    }
    else if(num2->len < num1->len){
        addZeroPadding(num2,num1->len - num2->len);
        smallnum = num2->num;
        bignum = num1->num;
        p_flag = 2;
    }
    else{
        bignum = num1->num;
        smallnum = num2->num;
    }

    result->num = (char *) calloc(size+1, sizeof(char));
    
    int i, temp = 0;
    for (i = size - 1; i >= 0; i--){
        temp = bignum[i] + smallnum[i] + temp / 10;
        result->num[i + 1] = temp % 10;
    }
    result->num[++i] = temp / 10;
    result->len = size+1;
    
    
    if(p_flag == 1)
        removeZeroPadding(num1);
    else if(p_flag == 2)
        removeZeroPadding(num2);
    removeZeroPadding(result);
    return result;
}

/*
 *Karatsuba implementation
 *bigInt* bigIntFastMul(bigInt* num1, bigInt* num2){
 *
 *    if(num1 == NULL || num2 == NULL){
 *        printf("NaN\n");
 *        return NULL;
 *    }
 *    
 *    //Length of both the numbers should be the same.
 *    long long int len = num1->len;
 *    int p_flag = -1;
 *    if(num1->len > num2->len){
 *        addZeroPadding(num2, num1->len - num2->len);
 *        len = num1->len;
 *        p_flag = 2;
 *    }
 *    else if(num1->len < num2->len){
 *        addZeroPadding(num1, num2->len - num2->len);
 *        len = num2->len;
 *        p_flag = 1;
 *    }
 *    
 *    //making sure that length in always even.
 *    if(len%2 != 0){
 *        addZeroPadding(num1, 1);
 *        addZeroPadding(num2, 1);
 *        p_flag = 3;
 *    }
 *
 *    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
 *
 *    result = executeKaratsuba(num1,num2,len);
 *    
 *    if(p_flag == 1)
 *        removeZeroPadding(num1);
 *    else if(p_flag == 2)
 *        removeZeroPadding(num2);
 *    else if(p_flag == 3){
 *        removeZeroPadding(num1);
 *        removeZeroPadding(num2);
 *    }
 *    
 *    return result;
 *}
 */

/*
 *bigInt* executeKaratsuba(bigInt *num1, bigInt *num2, long long int len){
 *
 *    if(len == 1)
 *        return bigIntMul(num1, num2);
 *
 *    long long int m = len / 2;
 *    
 *    bigInt *a1 = getFirstHalf(num1, m);
 *    bigInt *a2 = getSecondHalf(num1, m);
 *
 *    bigInt *b1 = getFirstHalf(num2, m);
 *    bigInt *b2 = getSecondHalf(num2, m);
 *
 *    bigInt *p1 = executeKaratsuba(a1, b1, m);
 *    bigInt *p2 = executeKaratsuba(a2, b2, m);
 *    bigInt *sum1 = bigIntAdd(a1, a2);
 *    bigInt *sum2 = bigIntAdd(b1, b2);
 *    
 *    bigInt *p3 = executeKaratsuba(sum1, sum2,
 *
 *
 *}
 */
