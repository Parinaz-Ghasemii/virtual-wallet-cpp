// Phase 2 - Problem 1 ("Frajibi"): object-oriented wallet (Wallet / Account / Sarafi / Currency / Balance)
#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>
using namespace std;

// RSA parameters shared by the whole system: p=101, q=19
const int RSA_P = 101;
const int RSA_Q = 19;
const int RSA_N = RSA_P * RSA_Q;   // 1919

class Currency{
public:
    string name;
    float value;
    void operator=(Currency c2){
        name=c2.name;
        value=c2.value;
    }
};
class Balance{
public:
    Currency c;
    float amount;
};

class Account{
private:
    int num_of_balance = 0;
    Balance balances[100];
public:
    friend class Wallet;
    string ID;
    void set_account(string str,int n,Currency currency_arr[100]){
        //split balances from string and add to strWords
        string strWords[n];
        int counter = 0;
        for(int i=0;i<str.length();i++){
            if(str[i] == ' '){
                counter++;
            }else{

            strWords[counter] += str[i]; }
            }
        //Add to balances array
        num_of_balance=0;
        for(int i=0;i<n;i++){
                balances[i].amount=stof(strWords[i]);
                balances[i].c=currency_arr[i];
                num_of_balance++;

        }
    }

    void print(){
        for(int i=0;i<num_of_balance;i++){
            cout<<fixed<<setprecision(2)<<balances[i].amount<<' ';}
        cout<<"\n";}

};

class Sarafi:public Account {};

class Wallet{
public:
    Currency currencys[100];
    int num_of_currency = 0;

    Account accounts[100];
    int num_of_accounts = 0;

    string errors[100];
    string err;
    int error_index = 0;   // was uninitialized -> crashed on some systems

    friend class Account;
    friend class Sarafi;


    void addCurrency(float f,string n){
        currencys[num_of_currency].value=f;
        currencys[num_of_currency].name=n;
        num_of_currency++;
    }
    void addAccount(Account acc){
    accounts[num_of_accounts]=acc;
    num_of_accounts++;
    }
    void Deposit(string instruct_words[4],Sarafi& sarafi_acc){
        int c;
        int co_index;
        for(int i=0;i<num_of_currency;i++){
            if(instruct_words[2]==currencys[i].name){c=i;break;}
            }

        if(sarafi_acc.balances[c].amount>=stof(instruct_words[3])){
            for(int i=0;i<num_of_accounts;i++){
                if(instruct_words[1]==accounts[i].ID){co_index=i;break;}
                }
            accounts[co_index].balances[c].amount+=stof(instruct_words[3]);
            sarafi_acc.balances[c].amount-=stof(instruct_words[3]);
            }
        else{
            err="Insufficient Funds: ";
            err+=sarafi_acc.ID;
            errors[error_index]=err;
            error_index++;
        }
    return ;
    }

    void Withdraw(string instruct_words[4],Sarafi& sarafi_acc){
        int c;
        for(int i=0;i<num_of_currency;i++){
            if(instruct_words[2]==currencys[i].name){c=i;break;}
            }
        int co_index;
        for(int i=0;i<num_of_accounts;i++){
            if(instruct_words[1]==accounts[i].ID){co_index=i;break;}
            }
        float val=stof(instruct_words[3]);
        if(accounts[co_index].balances[c].amount<val){
            err="Insufficient Funds: ";
            err+=accounts[co_index].ID;
            errors[error_index]=err;
            error_index++;
        }
        else{
            accounts[co_index].balances[c].amount-=val;
            sarafi_acc.balances[c].amount+=val;
        }
    return;
    }


     void Exchange(string instruct_words[5],Sarafi& sarafi_acc){
        int co_index;
        for(int i=0;i<num_of_accounts;i++){
            if(instruct_words[1]==accounts[i].ID){co_index=i;break;}
            }
        //c is index of currency
        int c1,c2,j;
        for(int i=0;i<num_of_currency;i++){
            if(instruct_words[2]==currencys[i].name){c1=i;}
            if(instruct_words[3]==currencys[i].name){c2=i;}
            if("jibCoin"==currencys[i].name){j=i;}
            }
        float val1=stof(instruct_words[4]);
        float val2=(val1*currencys[c1].value)/currencys[c2].value;
        //karmozd and subtract

        if(val1<=1000){
                val1+=val1/100;
            }
        else{val1+=currencys[j].value*(10/currencys[c1].value);}

        if(val1>accounts[co_index].balances[c1].amount){
            err="Insufficient Funds: ";
            err+=accounts[co_index].ID;
            errors[error_index]=err;
            error_index++;
            }
        else{
            //check error for sarafi

            if(val2>sarafi_acc.balances[c2].amount){
            err="Insufficient Funds: ";
            err+=sarafi_acc.ID;
            errors[error_index]=err;
            error_index++;
                }
            //do the exchange
            else{
            accounts[co_index].balances[c1].amount-=val1;
            sarafi_acc.balances[c1].amount+=val1;
            sarafi_acc.balances[c2].amount-=val2;
            accounts[co_index].balances[c2].amount+=val2;}
            }
        return;
        }

    void Transfer(string instruct_words[5],Sarafi& sarafi_acc){
        //find costumer
        int co1,co2;
        for(int i=0;i<num_of_accounts;i++){
            if(instruct_words[1]==accounts[i].ID){co1=i;}
            if(instruct_words[2]==accounts[i].ID){co2=i;}
            }
        //c is index of currency
        int c=0,j=0;
        for(int i=0;i<num_of_currency;i++){
            if(instruct_words[3]==currencys[i].name){c=i;}
            if("jibCoin"==currencys[i].name){j=i;}
            }
        float val=stof(instruct_words[4]);
        //karmozd and subtract
        float karmozd=0;
        if(val<=100){karmozd=val/10;}
        else{karmozd=currencys[j].value*(10/currencys[c].value);}

        if((val+karmozd)>accounts[co1].balances[c].amount){
            err="Insufficient Funds: ";
            err+=accounts[co1].ID;
            errors[error_index]=err;
            error_index++;
            }
        else{
            //do the exchange
            accounts[co1].balances[c].amount-=(val+karmozd);
            sarafi_acc.balances[c].amount+=karmozd;
            accounts[co2].balances[c].amount+=val;
            }
        return;

    }

    bool ValidCommand(string instruct_Words[4]){
        bool invalid=true;
        if(instruct_Words[0]=="WITHDRAW")invalid=false;
        else if(instruct_Words[0]=="DEPOSIT")invalid=false;
        else if(instruct_Words[0]=="EXCHANGE")invalid=false;
        else if(instruct_Words[0]=="TRANSFER")invalid=false;

        if(invalid){
            errors[error_index]="Invalid Command";
            error_index++;
        }
        return (!invalid);
    }

    bool ValidID(string instruct_Words[5]){
    int valid=0,valid2=0;
    if((instruct_Words[0]== "DEPOSIT") || (instruct_Words[0]=="WITHDRAW") || (instruct_Words[0]=="EXCHANGE")){
        for(int i=0;i<num_of_accounts;i++){
            if(instruct_Words[1]==accounts[i].ID)
                {valid=1;break;}
    }
    if(valid==0){
        err="Invalid User: ";
        err+=instruct_Words[1];
        errors[error_index]=err;
        error_index++;
        }
    return valid;
    }
    if(instruct_Words[0]=="TRANSFER"){
        for(int i=0;i<num_of_accounts;i++){
        if(instruct_Words[1]==accounts[i].ID)
            {valid=1;}
        if(instruct_Words[2]==accounts[i].ID)
            {valid2=1;}
    }
    if(valid==0){
        err="Invalid User: ";
        err+=instruct_Words[1];
        errors[error_index]=err;
        error_index++;
        return false;
        }
    if(valid2==0){
        err="Invalid User: ";
        err+=instruct_Words[2];
        errors[error_index]=err;
        error_index++;
        return false;
        }
    return true;
    }
    return false;
    }

    void printErrors(){
        for(int i=0;i<error_index;i++){
            cout<<errors[i]<<endl;
        }
    }


    void printAccounts(){
        for(int i=0;i<num_of_accounts;i++){
            for(int j=0;j<num_of_currency;j++){
            cout<<fixed<<setprecision(2)<<accounts[i].balances[j].amount<<' ';
        }
        cout<<"\n";
        }
    }
};


//functions
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

string Decipher(string str,int n){
    float d;
    d=find_d(RSA_P,RSA_Q);
    int len=str.length();
    int codes[n];
    string strWords[n];
    int counter = 0;
    for(int i=0;i<str.length();i++){
        if(str[i] == ' '){
            counter++;
        }else{

        strWords[counter] += str[i]; }
        }
    for(int i=0;i<n;i++){
        codes[i]=stoi(strWords[i]);
    }
    string decoded="";
    int decode;
    for(int i=0;i<n;i++){
        decode=mod(codes[i],d,RSA_N);
        decoded+=(char)(int)decode;
            }
    return decoded;
    }

int main(){
    int n;
    cin>>n;
    //get currency and value
    Wallet bank_wallet;
    bank_wallet.num_of_currency=0;
    int i;
    float f;
    string str;
    for(i=0;i<n;i++){
        cin>>str>>f;
        bank_wallet.addCurrency(f,str);
    }
    //Get Sarafi account
    Sarafi sarafi_acc;
    cin>>sarafi_acc.ID;
    getline(cin>>ws,str);
    sarafi_acc.set_account(str,n,bank_wallet.currencys);
    //Get all accounts and save in wallet
    bank_wallet.num_of_accounts=0;
    int account_num;
    Account a;
    cin>>account_num;
    for(int i=0;i<account_num;i++){
            cin>>a.ID;
            getline(cin>>ws,str);
            a.set_account(str,n,bank_wallet.currencys);
            bank_wallet.addAccount(a);
            }
    //Get instructions
    int instruct_num;
    cin>>instruct_num;
    int code_num;
    string decode;
    for(int i=0;i<instruct_num;i++){
        cin>>code_num;
        getline(cin>>ws,str);
        decode=Decipher(str,code_num);
        //Get words of instruct
        string instruct_Words[10];   // was sized by the number of currencies -> overflow when n < 5
        int counter = 0;
        for(int j=0;j<decode.length();j++){
            if(decode[j] == ' '){
                counter++;
            }else{

            instruct_Words[counter] += decode[j]; }
            }
         //Check command and ID
         bool valid_command=bank_wallet.ValidCommand(instruct_Words);
         bool valid_ID=bank_wallet.ValidID(instruct_Words);
        //Find which instruction and do
        if(valid_command && valid_ID){
            if(instruct_Words[0]=="DEPOSIT"){
                bank_wallet.Deposit(instruct_Words,sarafi_acc);
            }else if(instruct_Words[0]=="WITHDRAW"){
                bank_wallet.Withdraw(instruct_Words,sarafi_acc);

            }else if(instruct_Words[0]=="EXCHANGE"){
                bank_wallet.Exchange(instruct_Words,sarafi_acc);
            }else if(instruct_Words[0]=="TRANSFER"){
                bank_wallet.Transfer(instruct_Words,sarafi_acc);
            }
        }
    }
        bank_wallet.printErrors();
        sarafi_acc.print();
        bank_wallet.printAccounts();

    return 0;
}

