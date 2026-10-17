// Phase 1 - Problem 4: balances, users/keys and a transaction report persisted in files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>   // fmod, sqrt (link with -lm)
//READ FROM FILE
float readArz(int line_num,int arz_index,int n,char filename[n][50],char words[10][50]){
            FILE *fp;
            fp=fopen(filename[arz_index],"r");
            int keep_reading=1,current_line=0;
            char id[50];
            char arzstr[20];
            do{
                fscanf(fp,"%s %s\n",id,arzstr);
                if(current_line==line_num){keep_reading=0;}
                current_line++;

            }while(keep_reading);
            fclose(fp);
            float user_arz=atof(arzstr);
            return user_arz;
}
//WRITE new ARZ to file
void writeArz(int line_num,int arz_index,int words_index,float user_arz,int n,char filename[n][50],char words[10][50]){
    FILE *file,*temp;
    char temp_filename[50];
    char replace[50];
    //words_index is user id index in command
    strcpy(replace,words[words_index]);

    strcpy(temp_filename,"temp_____");
    strcat(temp_filename,filename[arz_index]);
    file = fopen(filename[arz_index],"r");
    temp = fopen(temp_filename,"w");
    int keep_reading=1,current_line=0;
    char buffer[50];
    do{
        fgets(buffer,50,file);
        if(feof(file)) keep_reading=0;
        else if(current_line==line_num)
                fprintf(temp,"%s %.2f\n",words[words_index],user_arz);
        else fprintf(temp,"%s",buffer);
        current_line++;

    }while(keep_reading);

    fclose(file);
    fclose(temp);
    remove(filename[arz_index]);
    rename(temp_filename,filename[arz_index]);
    return;

}
//FIND USER ID IN REPORT FILE AND PRINT LINES
void FindWord(char word[50] , char report[50]){
            FILE *fp=fopen(report,"r");
            char line[100],*user,temp[100];
            while (fgets(line,100,fp)!= NULL){
                strcpy(temp,line);
                user=strtok(temp," : ");
                if(user!=NULL && strcmp(user,word)==0){printf("%s",line);}
            }
            fclose(fp);
            return;
}


//DEPOSIT
int Deposit(int n,int co_num ,char filename[n][50],char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],int error_index){
        //c is index of currency and the file we need to change
        int c;
        for(int i=0;i<n;i++){
            if(!(strcmp(words[2],arz[i]))){c=i;break;}
            }
        //co_index is the line that should be changed
        if(sarafi[c]>=atof(words[3])){
            int co_index;
            for(int i=0;i<co_num;i++){
                if(!(strcmp(words[1],name[i]))){co_index=i;break;}
                }
            //read from file
            float user_arz;
            user_arz=readArz(co_index,c,n,filename,words);
            user_arz+=atof(words[3]);
            sarafi[c]-=atof(words[3]);
            //change ARZ in file
            writeArz(co_index,c,1,user_arz,n,filename,words);
            }
        else{
            error_index++;
            }
        return error_index;
        }

//WITHDRAWL
int Withdrawl(int n,int co_num ,char filename[n][50],char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],int error_index){
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
        //read from file
        float user_arz;
        user_arz=readArz(co_index,c,n,filename,words);
        if(user_arz<val){
            error_index++;
            }
        else{
            user_arz-=val;
            //change ARZ in file
            writeArz(co_index,c,1,user_arz,n,filename,words);
            sarafi[c]+=val;
        }
    return error_index;
}
//EXCHANGE
int Exchange(int n,int co_num ,char filename[n][50],char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],int error_index){
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
        //read from file
        float user_arz1,user_arz2;
        user_arz1=readArz(co_index,c1,n,filename,words);
        user_arz2=readArz(co_index,c2,n,filename,words);

        if(val1>user_arz1){
            error_index++;
            return error_index;
            }
        else{
            //check error for sarafi

            if(val2>sarafi[c2]){
                error_index++;
                return error_index;
                }
            //do the exchange
            else{
            user_arz1-=val1;
            sarafi[c1]+=val1;
            sarafi[c2]-=val2;
            user_arz2+=val2;
            //write in file the new ARZ
            writeArz(co_index,c1,1,user_arz1,n,filename,words);
            writeArz(co_index,c2,1,user_arz2,n,filename,words);
            }
            }
        return error_index;

}
//TRANSFER

int Transfer(int n,int co_num ,char filename[n][50],char name[co_num][50],char arz[n][20],float value[n],char words[10][50],float *sarafi,char s_id[50],int error_index){

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
        //read data from file
        float user_arz_co1,user_arz_co2;
        user_arz_co1=readArz(co1,c,n,filename,words);
        user_arz_co2=readArz(co2,c,n,filename,words);

        if((val+karmozd)>user_arz_co1){
            error_index++;
            return error_index;
            }
        else{
            //do the exchange
            user_arz_co1-=(val+karmozd);
            sarafi[c]+=karmozd;
            user_arz_co2+=val;
            //write new data in file
            writeArz(co1,c,1,user_arz_co1,n,filename,words);
            writeArz(co2,c,2,user_arz_co2,n,filename,words);
            }
        return error_index;
}

//CHECK IF USER IS VALID
int ValidName(int co_num,char name[co_num][50],char words[10][50],int error_index){
    int valid=0,valid2=0;
    if((!(strcmp(words[0],"DEPOSIT"))) || (!(strcmp(words[0],"WITHDRAW"))) || (!(strcmp(words[0],"EXCHANGE")))){
        for(int i=0;i<co_num;i++){
            if(!(strcmp(words[1],name[i])))
                {valid=1;break;}
    }
    if(valid==0){
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
        error_index++;
        return error_index;
        }
    if(valid2==0){
        error_index++;
        return error_index;
        }
    }
    return error_index;
    }

//CHECK IF COMMAND IS VALID
int ValidCommand(char words[10][50],int len,int error_index){
    int valid_com=0;
    if(!(strcmp(words[0],"TRANSFER"))) valid_com=1;
    if(!(strcmp(words[0],"WITHDRAW"))) valid_com=1;
    if(!(strcmp(words[0],"EXCHANGE"))) valid_com=1;
    if(!(strcmp(words[0],"DEPOSIT"))) valid_com=1;

    if(valid_com==0){
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
float find_e(int p,int q){
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
    return e;
}


float find_d(int p,int q,float e){
        int n=p*q;
        //phi function
        int phi=(p-1)*(q-1);
    //find d
    float d;
    int k=1;
    while(1){
        d=(k*phi+1)/e;
        if(((int)d)==d)
            break;
        else{k++;}
        //printf("omg");

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

    //CREATE FILE FOR EACH ARZ
    FILE *fptr[n];
    char filename[n][50];
    for(i=0;i<n;i++){
        strcpy(filename[i],arz[i]);
        fptr[i]=fopen(strcat(filename[i],".txt"),"w");}
    //ADD ARZ of user to file
    for(i=0;i<n;i++){
            for(int j=0;j<costumer_num;j++){
                fprintf(fptr[i],"%s %.2f\n",costumers_name[j],costumers_acc[j][i]);
            }
            fclose(fptr[i]);
}

    //CREATE FILE FOR USERS AND KEYS
    /*ufp=user file pointer*/
    FILE *ufp;
    ufp=fopen("USERS.txt","w");
    //create a list of prime numbers
    int prime_arr[costumer_num+1];
    int primecount=0,prime_num=2;
    i=3;
    while(primecount<costumer_num+1){
            int flag=1;
            for(int j=2;j*j<=i;j++){
                if(i%j==0){
                    flag=0;
                    break;}
            }
            if(flag){
                prime_arr[primecount]=i;
                primecount++;
            }
            i++;
        }
    float private_key,public_key,n_forkeys;
    for(i=0;i<costumer_num;i++){
        fprintf(ufp,"%s ",costumers_name[i]);
        //find e and d
        public_key=find_e(prime_arr[i],prime_arr[i+1]);
        private_key=find_d(prime_arr[i],prime_arr[i+1],public_key);
        n_forkeys=prime_arr[i]*prime_arr[i+1];
        //write keys in file
         fprintf(ufp,"public_key(%g,%g) ",n_forkeys,public_key);
         fprintf(ufp,"private_key(%g,%g)\n",n_forkeys,private_key);

    }
 fclose(ufp);


   //get instructions
    int m;
    scanf("%d\n",&m);
    int *code;
    char words[10][50];
    char instruct[50];
    double decode;
    int len=0;
    //open file to restore reports
    FILE *rfp;
    rfp=fopen("report.txt","w");

    //get each instruction and do
    for(i=0;i<m;i++){
        fgets(instruct, 50 , stdin);
        instruct[strcspn(instruct,"\n")]='\0';

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
        int error_index=0;
        int flag=1;
        //find out if command is valid
        error_index=ValidCommand(words,len,error_index);
        if(0<error_index)flag=0;
        //find out if user is valid
        error_index=ValidName(costumer_num,costumers_name,words,error_index);
        if(0<error_index)flag=0;
        int transfer_flag=0;
        //find out which instruction and do
        if(flag){
            if(!(strcmp(words[0],"DEPOSIT")))
            {
                error_index=Deposit(n,costumer_num,filename,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,error_index);
            }
        else if(!(strcmp(words[0],"WITHDRAW")))
            {
                error_index=Withdrawl(n,costumer_num,filename,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,error_index);
            }
        else if(!(strcmp(words[0],"EXCHANGE")))
            {
                error_index=Exchange(n,costumer_num,filename,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,error_index);
            }
        else if(!(strcmp(words[0],"TRANSFER")))
            {   error_index=Transfer(n,costumer_num,filename,costumers_name,arz,value,words,Sarifi_acc,Sarafi_id,error_index);
                transfer_flag=1;
            }
        }

        //write in report
        if(strcmp(words[0],"REPORT")!=0){
            char result[20];
            if(error_index==0) strcpy(result," Successful!");
            else{strcpy(result," Failed!");}
            fprintf(rfp,"%s : ",words[1]);
            for(j=0;j<strlen(instruct);j++)
                fprintf(rfp,"%c",instruct[j]);
            fprintf(rfp,"%s\n",result);
            if(transfer_flag){
                fprintf(rfp,"%s : ",words[2]);
                for(j=0;j<strlen(instruct);j++)
                    fprintf(rfp,"%c",instruct[j]);
                fprintf(rfp,"%s\n",result);
                }
        }else{
                fflush(rfp);
                char co_id[50];
                int r;
                for(r=0;r<strlen(words[1]);r++){
                    co_id[r]=words[1][r];
                }
                co_id[r]='\0';
                FindWord(co_id,"report.txt");
                }

}

fclose(rfp);
return 0;}
