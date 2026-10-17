// Phase 1 - Problem 1: custom one-way hash + RSA encryption of the hash

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//HASH
void Hash_func(int *arr,char str[100]){
    int len=strlen(str);
    if(len>0 && str[len-1]=='\n') str[--len]='\0';   // drop the newline left by fgets
    int temp[100];
    for(int i=0;i<len;i++){
        temp[i]=(((3*(int)str[i])+17)%26)+65;
    }
    int sum=0;
    for(int j=0;j<16;j++){   // the hash is always exactly 16 characters
        sum=0;
        for(int i=0;i<len;i++){
            if(i%16==j){
                sum+=temp[i];
            }
        }
        arr[j]=sum%26+65;
    }
    return;

}

//GCD
int gcd(int num1,int num2){
    while (num1 != num2)
    {
        if (num1 > num2)  { num1 = num1 - num2; }
        else { num2 = num2 - num1; }

    }
    return num2;
}

//mod
double mod(int a,int k,int n){
    double b=a;
    int mod1=a%n;
    double mod2=fmod(b,n);
    for(int i=1;i<k;i++){
        mod2=fmod((mod2* mod1),n);
        }
    return mod2;


}

int main(){
    char S[100];
    int p,q;
    fgets(S,100,stdin);
    scanf("%d %d",&p,&q);
    int n=p*q;
    //phi function
    int phi=(p-1)*(q-1);
    //find e
    int e;
    for(int i=2;i<phi;i++){
            if(gcd(i,phi)==1){
                e=i;
                break;
            }
    }

    //Hash function for S
    int hash_arr[16];
    Hash_func(hash_arr,S);

    //print result
    double res;
    for(int i=0;i<16;i++){
        res=mod(hash_arr[i],e,n);
        printf("%g ",res);
    }
    return 0;



}
