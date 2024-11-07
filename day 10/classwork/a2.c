/*description:
Program to find nth number made of given four digits 1, 4,6 and 9 as the only digits..

Input Format:

The First Line Of Input Contains T no of test cases
The Second Line Of Input Contains N as input taken.

Output Format:
Print the number of digits in the nth number .

Constraints:

1<=T<=100
1<=N<=100

Examples:

Input : 6
Output : 14

Input : 21
Output : 111

Author:CSP
DOC:7/11/2024
version:0.1v
*/
#include <stdio.h>


void find_nth_number(int n) {
   
    int digits[] = {1, 4, 6, 9};
    int result[100];  

    int idx = 0;
    while (n > 0) {
        
        result[idx++] = digits[(n - 1) % 4];
        n = (n - 1) / 4;  
    }

    
    printf("The number is:");
    for (int i = idx - 1; i >= 0; i--) {
        printf("%d", result[i]);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter the value of n: ");
    scanf("%d", &n);

    find_nth_number(n);

    return 0;
}