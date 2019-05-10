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

    //to remove the uncessary zeros if present.
    removeZeroPadding(newNum);
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

bigInt* addZeroPadding_end(bigInt *num, long long int offset){
    long long int newLen = num->len + offset;
    // char *tempNum = (char*) malloc(sizeof(char)*(newLen));

    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
    result->len = newLen;
    result->num = (char *) calloc(newLen, sizeof(char)); 

    long long int i = 0;
    // printf("Entering first while loop\n");
    // printf("num->len is %lld\n", num->len);
    while(i < num->len)
    {
        result->num[i] = num->num[i];
        i++;
    }
    // int j = 0;
    // printf("Entering second while loop\n");
    while(i < newLen){
        result->num[i] = 0;
        i++;
    }

    return result;
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

bigInt* bigIntIncrement(bigInt *num){
    if( num == NULL){
        printf("bigIntIncrement:Nan\n");
        return NULL;
    }

    bigInt* unity = (bigInt*)malloc(sizeof(bigInt)*1);
    unity->num = (char *)calloc(1, sizeof(char));
    unity->num[0] = 1;
    unity->len = 1;

    bigInt *temp = bigIntAdd(num,unity);
    
    free(num);
    
    num = temp;
    free(unity);
    return num;
}

bigInt* bigIntSub(bigInt *num1, bigInt *num2){
    if(num1 == NULL || num2 == NULL)
        return NULL;
    
    
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


int iszero(bigInt* num)
{
	for(int i=0; i<num->len; i++)
	{
		if(num->num[i] != 0)
			return 0;
	}
	return 1;
}


//Divides num1/num2
bigInt* bigIntDiv(bigInt* num1, bigInt* num2)
{

	bigInt*  result = (bigInt*)malloc(sizeof(bigInt)*1);
	result->num = (char *) calloc(num1->len, sizeof(char));
	result->len = 0;

    /*
     *bigInt* unity = (bigInt*)malloc(sizeof(bigInt)*1);
     *unity->num = (char *)calloc(1, sizeof(char));
     *unity->num[0] = 1;
     *unity->len = 1;
     */

	if(num1->num == NULL || num2->num == NULL){
        printf("bigIntDiv: NaN\n");
		return (bigInt*)NULL;
	}
	
    if((iszero(num1) == 1) || (bigIntCompare(num1, num2) == -1)){
		// printf("Division is 0\n");
		result->num[0] = 0;
		result->len = 1;
		return result;
	}

    if(iszero(num2) == 1){
        printf("Division by zero is INF\n");
        return (bigInt*)NULL;
    }
    
    /*
     *char *bignum, *smallnum, p_flag = -1;
     *long long int len = 0;
     *if(num1->len > num2->len){
     *    addZeroPadding(num2,num1->len - num2->len);
     *    p_flag = 2;
     *    bignum = num1->num;
     *    smallnum = num2->num;
     *    len = num1->len;
     *}
     *else if(num1->len < num2->len){
     *    addZeroPadding(num1, num2->len - num1->len);
     *    p_flag = 1;
     *    bignum = num2->num;
     *    smallnum = num1->num;
     *    len = num2->len;
     *}
     *else{
     *    bignum = num1->num;
     *    smallnum = num2->num;
     *    len = num1->len;
     *}
     */
    
    
    bigInt *copy = (bigInt*) malloc(sizeof(bigInt)*1);
    copy->num = (char*) malloc(sizeof(char)*num1->len);
    copy->len = num1->len; 
    
    //creating a copy of bignum;
    for(long long int i = 0; i < num1->len; i++)
        copy->num[i] = num1->num[i];
    
    bigInt *temp = copy;

    //run while loop while num1 > num2
    int count = 0;
    bigIntPrint(copy);
    while(bigIntCompare(copy, num2) != -1){
        printf("...wait..");
		temp = bigIntSub(copy, num2);
        
        free(copy);
        copy = temp;
        
        result = bigIntIncrement(result);
        count+=1;
		//result = bigIntAdd(result, unity);
	}
    printf("\n");
    removeZeroPadding(result);	
    return result;
}


bigInt* bigMulsmall(bigInt* num1, int num2 )
{
	bigInt* result = (bigInt*)malloc(sizeof(bigInt*));
    result->len = num1->len + 1;
    result->num = (char*)calloc((num1->len)+1, sizeof(char));

	const int size = num1->len;

	int i, prod = 0;
	for ( i = size - 1; i >= 0; i-- )
	{
		prod = num1->num[i] * num2 + prod / 10;
		result->num[i + 1] = prod % 10;
	}
	result->num[++i] = prod / 10;

	removeZeroPadding(result);

	return result;
}


bigInt* bigIntMul(bigInt* num1, bigInt* num2)
{
    long int len = num1->len;
    if(num2->len > num1->len)
        len = num2->len;

    bigInt* temp = (bigInt*)malloc(sizeof(bigInt));
    temp->num = (char*)calloc(len, sizeof(char));
    temp->len = 1;
    
	if (num2->len > num1->len)
	{
		// swap num1 and num2
        temp = num1;
        num1 = num2;
        num2 = temp;
	}

	bigInt* result = (bigInt*)malloc(sizeof(bigInt));
    result->num = (char*)calloc(2*len, sizeof(char));
    result->len = 0;

	const int size = num2->len;
	int i = size - 1;
    // printf("Entering while loop\n");
	while ( i >= 0 )
	{
        temp = addZeroPadding_end(bigMulsmall(num1, num2->num[i]), size-i-1);
        // printf("Padding complete: ");
        // bigIntPrint(temp);
		result = bigIntAdd(result, temp);
        // printf("result: ");
        // bigIntPrint(result);
		i--;
	}

	removeZeroPadding(result);

	return result;
}


bigInt* bigIntPow(bigInt* num1, bigInt* num2)
{
    // printf("Power function called\n\n");
    bigInt* result = (bigInt*)malloc(sizeof(bigInt));
    bigInt* temp = (bigInt*)malloc(sizeof(bigInt));
    bigInt* num2_copy = (bigInt*)malloc(sizeof(bigInt));
    temp->num = (char *)calloc(num1->len + num2->len, sizeof(char));

    // UNITY (probably not matrix reference -_-)
    bigInt* unity = (bigInt*)malloc(sizeof(bigInt)*1);
    unity->num = (char *)calloc(1, sizeof(char));
    unity->num[0] = 1;
    unity->len = 1;

    temp->len = 0;
    if(num2->len > num1->len)
    {
        temp = num1;
        num1 = num2;
        num2 = temp;
    }
    num2_copy->len = num2->len;
    num2_copy->num = (char*)calloc(num2->len, sizeof(char));
    num2_copy->num = num2->num;
    int size = num1->len;
    result->num = (char *)calloc(size+(2*num2->len), sizeof(char));
	if (iszero(num2))
	{
		result->num[0] = 1;
        result->len = 1;
        return result;
	}

    // SOME ALGORITHM I FOUND ONLINE, PROBABLY MORE EFFICIENT BUT I DIDN'T UNDERSTAND IT (line 462 - 473)

	// if ( ( num2[num2.size() - 1] - '0' ) & 1 )
	// {
	// 	string temp = BigPow( num1, BigDiv( num2, "2" ) );
	// 	return BigMul( BigMul( num1, temp ), temp );
	// }
	// else
	// {
	// 	string temp = BigPow( num1, BigDiv( num2, "2" ) );
	// 	return BigMul( temp, temp );
	// }

    result->num = num1->num;
    result->len = num1->len;

    num2_copy = bigIntSub(num2_copy, unity);

    // printf("Num2_copy before entering the loop: ");
    // bigIntPrint(num2);

    while(!iszero(num2_copy))
    {
        result = bigIntMul(result, num1);
        num2_copy = bigIntSub(num2_copy, unity);
        // bigIntPrint(num2_copy);
    }
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
