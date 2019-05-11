#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 100000 

int main(){

    srand(time(0));
    char *str1 = (char*) malloc(sizeof(char)*MAX+1);
    char *str2 = (char*) malloc(sizeof(char)*MAX+1);
    long int i = 0; 
    for(i = 0; i < MAX; i++){
        str1[i] = rand()%10;
    }
    srand(time(NULL));
    for(i = 0; i < MAX; i++){
        str2[i] = rand()%10;
    }
    for(i = 0; i < MAX; i++)
        printf("%d",str1[i]);
    printf("\n");
    for(i = 0; i < MAX; i++)
        printf("%d",str1[i]);
    printf("\n");
   return 0; 
}
