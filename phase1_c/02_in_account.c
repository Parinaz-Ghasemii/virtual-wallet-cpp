// Phase 1 - Problem 2: deposit / withdraw / exchange inside a single account

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//DEPOSIT
void Deposit(int n,float *acc,char arz[n][20],float value[n],char words[4][50]){
        //c is index of currency
        int c;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[1],arz[i]))){c=i;break;}
            }
        acc[c]+=atof(words[2]);
        return;

}

//WITHDRAWL
void Withdrawl(int n,float *acc,char arz[n][20],float value[n],char words[4][50],int *error){
        //c is index of currency
        int c;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[1],arz[i]))){c=i;break;}
            }
        float val=atof(words[2]);
        if(acc[c]<val){
            (*error)++;
        }
        else{
            acc[c]-=val;
        }
}
//EXCHANGE
void Exchange(int n,float *acc,char arz[n][20],float value[n],char words[4][50],int *error){
        //c is index of currency
        int c1,c2,j;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[1],arz[i]))){c1=i;}
            if(!(strcmp(words[2],arz[i]))){c2=i;}
            if(!(strcmp("jibCoin",arz[i]))){j=i;}
            }
        float val1=atof(words[3]);
        float val2=(val1*value[c1])/value[c2];
        //karmozd and subtract

        if(val1<=1000){
                val1+=val1/100;
            }
        else{val1+=(10/value[c1]);}

        if(val1>acc[c1]){
            (*error)++;
            }
        else{
        acc[c1]-=val1;
        acc[c2]+=val2;}
        return;


}

//MAIN FUNCTION

int main(){

    int n;
    scanf("%d",&n);
    //get currency and value
    char arz[n][20];
    float value[n];
    int i;
    for(i=0;i<n;i++){
        scanf("%s %f",arz[i],&value[i]);
    }
    //get account number
    float *account_num;
    account_num=malloc(n*sizeof(float));
    for(i=0;i<n;i++){
        scanf("%f\n",&account_num[i]);
    }
    int error=0;
    //get instructions
    int m;
    scanf("%d\n",&m);
    char instruct[50];
    char words[4][50];
    int len;
    for(i=0;i<m;i++){
        fgets(instruct, 50 , stdin);

        //Split instruction into words

        int j,ctr,k;
        j=0; ctr=0;
        for(k=0;k<=(strlen(instruct));k++){
            if(instruct[k]==' '||instruct[k]=='\0'){
                words[ctr][j]='\0';
                ctr++;
                j=0;
            }
            else{
               words[ctr][j]=instruct[k];
                j++;
            }
        }

        //find out which instruction and do
            if(!(strcmp(words[0],"DEPOSIT")))
            {
                Deposit(n,account_num,arz,value,words);
            }
        else if(!(strcmp(words[0],"WITHDRAWL")))
            {
                Withdrawl(n,account_num,arz,value,words,&error);
            }
        else if(!(strcmp(words[0],"EXCHANGE")))
            {Exchange(n,account_num,arz,value,words,&error);
            }

}
for(i=0;i<error;i++){
    printf("Insufficient Funds\n");
}
for(i=0;i<n;i++){
    printf("%.2f ",account_num[i]);
}
return 0;


}
