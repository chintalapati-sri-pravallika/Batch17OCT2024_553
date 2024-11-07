/*description:
Write a program to find the  n-th number made of prime digits

Input Format:
First Line Of Input Contains T Number Of Test Cases
Second Line Of Input Contains an input Number N.

Output Format:
Print the Nth number of sequence and it should be only prime.

Constraints:
1<=T<=100
1<=N<=10000

Examples :
Input  :
1
10
Output :
33
Author:CSP
DOC:7/11/2024
version:0.1v
*/
#include <stdio.h>
#define MAX 10000
int primeDigits[] = {2, 3, 5, 7};
int Num(int arr[], int n) {
   int index = 0;
   for (int i = 0; i < 4; i++) arr[index++] = primeDigits[i];
   for (int length = 1; index < n; length++) {
       for (int i = 0; i < index; i++) {
           for (int j = 0; j < 4; j++) {
               arr[index++] = arr[i] * 10 + primeDigits[j];
               if (index >= n) 
               return 0; 
           }
       }
   }
}
 
int main() {
   int t, n, arr[MAX];
   scanf("%d", &t);
   Num(arr, MAX);
   while (t--) {
       scanf("%d", &n);
       printf("%d\n", arr[n - 1]);
   }
   return 0;
}