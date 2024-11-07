/*description:
Check whether a given number can be expressed as the sum of two prime number
input
Enter a positive integer: 34
Output
34 = 3 + 31
34 = 5 + 29
34 = 11 + 23
34 = 17 + 17
Author:CSP
DOC:7/11/2024
version:0.1v
*/

#include <stdio.h>
int prime(int num){
    int i,isprime=1;
    if(num==0 || num==1){
        isprime=0;
    }else{
        for(i=2;i<num/2;i++){
            if(num%i==0){
                isprime=0;
                break;
            }
        }
    }
    return isprime;
}

int main(){
    int num,i,flag=0,count=0;
    scanf("%d",&num);
    for(i=2;i<=num/2;i++){
        if(prime(i)==1){
            if(prime(num-i)==1){
                printf("%d = %d + %d\n",num,i,num-i);
                flag=1;
                count++;
            }
        }
    }
    if(count>0)
    printf("no of ways:%d",count);
    else
    printf("no of ways:-1");
    if(flag==0)
    printf("%d can't be expressed as sum");
    return 0;