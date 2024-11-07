/*description:rotate array every kth element
input:1 2 3 4 5 6 7 8 9
key = 3
output:3 2 1 6 5 4 9 8 7
Author: CSP
DOC:07/11/2024
version:0.1v
*/

#include <stdio.h>
void printArray(int arr[],int size){
    for(int i=0;i<size;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}


void swap(int arr[],int start,int end){
    int temp=arr[start];
    arr[start]=arr[end];
    arr[end]=temp;
}


void rotate(int arr[],int size,int key){
    for(int i=0;i<size;i=i+key){
        int start=i;
        int end=i+key-1;
        if(end>=size)
        end=size-1;
        swap(arr,start,end);
    }
}


int main() {
    int key;
    scanf("%d",&key);
    int size;
    scanf("%d",&size);
    int arr[size];
    for(int i=0;i<size;i++){
        scanf("%d",&arr[i]);
    }

    printf("array before modification\n");
    printArray(arr,size);
    
    rotate(arr,size,key);
    
    printf("array after modification\n");
    printArray(arr,size);
return 0;
}