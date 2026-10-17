// Phase 1 - Problem 3: multi-user wallet with an exchange account; commands arrive RSA-encrypted

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>   // fmod (link with -lm)

//DEPOSIT
int Deposit(int n,int co_num,float **acc ,char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],char (*errors)[50],int error_index){
        //c is index of currency
        int c;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[2],arz[i]))){c=i;break;}
            }

        if(sarafi[c]>=atof(words[3])){
            int co_index;
            for(int i=0;i<co_num;i++){
                if(!(strcmp(words[1],name[i]))){co_index=i;break;}
                }
            acc[co_index][c]+=atof(words[3]);
            sarafi[c]-=atof(words[3]);
            }
        else{
            char err[50]="Insufficient Funds: ";
            strcat(err,s_id);
            strcpy(errors[error_index],err);
            error_index++;
        }
        return error_index;
        }

//WITHDRAWL
int Withdrawl(int n,int co_num,float** acc,char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],char (*errors)[50],int error_index){
        //c is index of currency
        int c;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[2],arz[i]))){c=i;break;}
            }
        int co_index;
        for(int i=0;i<co_num;i++){
            if(!(strcmp(words[1],name[i]))){co_index=i;break;}
            }
        float val=atof(words[3]);
        if(acc[co_index][c]<val){
            char err[50]="Insufficient Funds: ";
            strcat(err,name[co_index]);
            strcpy(errors[error_index],err);
            error_index++;
        }
        else{
            acc[co_index][c]-=val;
            sarafi[c]+=val;
        }
    return error_index;
}
//EXCHANGE
int Exchange(int n,int co_num,float** acc,char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],char (*errors)[50],int error_index){
        //find costumer
        int co_index;
        for(int i=0;i<co_num;i++){
            if(!(strcmp(words[1],name[i]))){co_index=i;break;}
            }
        //c is index of currency
        int c1,c2,j;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[2],arz[i]))){c1=i;}
            if(!(strcmp(words[3],arz[i]))){c2=i;}
            if(!(strcmp("jibCoin",arz[i]))){j=i;}
            }
        float val1=atof(words[4]);
        float val2=(val1*value[c1])/value[c2];
        //karmozd and subtract

        if(val1<=1000){
                val1+=val1/100;
            }
        else{val1+=value[j]*(10/value[c1]);}

        if(val1>acc[co_index][c1]){
            char err[50]="Insufficient Funds: ";
            strcat(err,name[co_index]);
            strcpy(errors[error_index],err);
            error_index++;
            return error_index;
            }
        else{
            //check error for sarafi

            if(val2>sarafi[c2]){
                char err[50]="Insufficient Funds: ";
                strcat(err,s_id);
                strcpy(errors[error_index],err);
                error_index++;
                return error_index;
                }
            //do the exchange
            else{
            acc[co_index][c1]-=val1;
            sarafi[c1]+=val1;
            sarafi[c2]-=val2;
            acc[co_index][c2]+=val2;}
            }
        return error_index;

}
//TRANSFER

int Transfer(int n,int co_num,float** acc,char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],char (*errors)[50],int error_index){

        //find costumer
        int co1,co2;
        for(int i=0;i<co_num;i++){
            if(!(strcmp(words[1],name[i]))){co1=i;}
            if(!(strcmp(words[2],name[i]))){co2=i;}
            }
        //c is index of currency
        int c=0,j=0;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[3],arz[i]))){c=i;}
            if(!(strcmp("jibCoin",arz[i]))){j=i;}
            }
        float val=atof(words[4]);
        //karmozd and subtract
        float karmozd=0;
        if(val<=100){karmozd=val/10;}
        else{karmozd=value[j]*(10/value[c]);}

        if((val+karmozd)>acc[co1][c]){
            char err[50]="Insufficient Funds: ";
            strcat(err,name[co1]);
            strcpy(errors[error_index],err);
            error_index++;
            return error_index;
            }
        else{
            //do the exchange
            acc[co1][c]-=(val+karmozd);
            sarafi[c]+=karmozd;
            acc[co2][c]+=val;
            }
        return error_index;
}

//CHECK IF USER IS VALID
int ValidName(int co_num,char name[co_num][50],char words[10][50],char (*errors)[50],int error_index){
    int valid=0,valid2=0;
    if((!(strcmp(words[0],"DEPOSIT"))) || (!(strcmp(words[0],"WITHDRAW"))) || (!(strcmp(words[0],"EXCHANGE")))){
        for(int i=0;i<co_num;i++){
            if(!(strcmp(words[1],name[i])))
                {valid=1;break;}
    }
    if(valid==0){
        char err[50]="Invalid User: ";
        strcat(err,words[1]);
        strcpy(errors[error_index],err);
        error_index++;
        }
    }
    if((!(strcmp(words[0],"TRANSFER")))){
        for(int i=0;i<co_num;i++){
        if(!(strcmp(words[1],name[i])))
            {valid=1;}
        if(!(strcmp(words[2],name[i])))
            {valid2=1;}
    }
    if(valid==0){
        char err[50]="Invalid User: ";
        strcat(err,words[1]);
        strcpy(errors[error_index],err);
        error_index++;
        return error_index;
        }
    if(valid2==0){
        char err[50]="Invalid User: ";
        strcat(err,words[2]);
        strcpy(errors[error_index],err);
        error_index++;
        return error_index;
        }
    }
    return error_index;
    }

//CHECK IF COMMAND IS VALID
int ValidCommand(char words[10][50],int len,char (*errors)[50],int error_index){
    int valid_com=0;
    if(!(strcmp(words[0],"TRANSFER"))) valid_com=1;
    if(!(strcmp(words[0],"WITHDRAW"))) valid_com=1;
    if(!(strcmp(words[0],"EXCHANGE"))) valid_com=1;
    if(!(strcmp(words[0],"DEPOSIT"))) valid_com=1;

    if(valid_com==0){
        char err[50]="Invalid Command";
        strcpy(errors[error_index],err);
        error_index++;
        }
    return error_index;
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

float find_d(int p,int q){
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
    //find d
    float d;
    int k=1;
    while(1){
        d=(k*phi+1)/e;
        if((k*phi+1)==(d*e))
            break;
        else{k++;}

    }
    return d;
    }


/*______________________________________________________________*/
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
    //get SARAFI information
    char Sarafi_id[50];
    scanf("%s",Sarafi_id);
    float *Sarifi_acc;
    Sarifi_acc=malloc(n*sizeof(float));
    for(i=0;i<n;i++){
        scanf("%f\n",&Sarifi_acc[i]);
    }
    //get account number
    int costumer_num;
    scanf("%d",&costumer_num);
    float **costumers_acc = (float **)malloc(costumer_num * sizeof(float *));
    for (int r = 0; r < costumer_num; r++) {
        costumers_acc[r] = (float *)malloc(n * sizeof(float));
    }
    //float costumers_acc[costumer_num][n];
    char costumers_name[costumer_num][50];
    for(i=0;i<costumer_num;i++){
        scanf("%s",costumers_name[i]);
        float *account_num;
        account_num=malloc(n*sizeof(float));
        for(int j=0;j<n;j++){
            scanf("%f",&account_num[j]);
            costumers_acc[i][j]=account_num[j];}
        }

   //get instructions
    int m;
    scanf("%d\n",&m);
    int *code;
    char words[10][50];
    char instruct[128];   // decoded command (at most ~50 chars) + terminator
    double decode;
    int len;
    //find d first for decoding
    float d;
    d=find_d(101,19);
    //a list of errors to print later
    char errors[m][50];
    int error_index=0;
    //get codes of instructions
    for(i=0;i<m;i++){
        scanf("%d",&len);
        code=malloc(len*sizeof(int));
        for(int j=0;j<len;j++){
                scanf("%d",&code[j]);
            }
        //DECODE instruction
        int j;
        for(j=0;j<len;j++){
            decode=mod(code[j],d,1919);
            instruct[j]=(char)(int)decode;
            }
        instruct[j]='\0';
        free(code);



        //Split instruction into words

        int ctr,k;
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
        int prior_error=error_index;
        int flag=1;
        //find out if command is valid
        error_index=ValidCommand(words,len,errors,error_index);
        if(prior_error<error_index)flag=0;
        //find out if user is valid
        error_index=ValidName(costumer_num,costumers_name,words,errors,error_index);
        if(prior_error<error_index)flag=0;
        //find out which instruction and do
        if(flag){
            if(!(strcmp(words[0],"DEPOSIT")))
            {
                error_index=Deposit(n,costumer_num,costumers_acc,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,errors,error_index);

            }
        else if(!(strcmp(words[0],"WITHDRAW")))
            {
                error_index=Withdrawl(n,costumer_num,costumers_acc,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,errors,error_index);
            }
        else if(!(strcmp(words[0],"EXCHANGE")))
            {
                error_index=Exchange(n,costumer_num,costumers_acc,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,errors,error_index);
            }
        else if(!(strcmp(words[0],"TRANSFER")))
            {   error_index=Transfer(n,costumer_num,costumers_acc,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,errors,error_index);
            }
        }

}
//PRINT EVERYTHING
for(i=0;i<error_index;i++){
    printf("%s\n",errors[i]);
}
for(i=0;i<n;i++){
    printf("%.2f ",Sarifi_acc[i]);
}
printf("\n");
for(i=0;i<costumer_num;i++){
    for(int j=0;j<n;j++){
        printf("%.2f ",costumers_acc[i][j]);
    }
    printf("\n");
}


return 0;}
