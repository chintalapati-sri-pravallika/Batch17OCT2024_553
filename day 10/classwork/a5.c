/*description:Write a c program to print alphabet triangle.
Input: 5
Output:

     A
    ABA
   ABCBA
  ABCDCBA
 ABCDEDCBA

Author:CSP
DOC:07/11/2024
version:0.1v
*/


#include <stdio.h>

//function defination
void fun(int n){
    for(int row=0;row<n;row++){
        for(int col=0;col<n-row-1;col++){  //loop to print spaces
            printf(" ");
        }
        for(int col=0;col<=row;col++){     //loop to print increasing characters
            printf("%c",'A'+col);
        }
        for(int col=row-1;col>=0;col--){   // loop to print decreasing characters
            printf("%c",'A'+col);
        }
        printf("\n");                      //to traverse to next line
    }
}


int main() {
    int n;
    scanf("%d",&n); //input from user
    fun(n);  //fun calling to print pattern

    return 0;
}