// Phase 2 - Problem 2 ("Jib App"): console wallet with signup/login, roles and file persistence
#include <iostream>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

//FUNCTIONS
//HASH
string Hash_func(string str){
    string hashed;
    int len=str.length();
    if(len>100) len=100;
    int temp[100];
    for(int i=0;i<len;i++){
        temp[i]=(((3*(int)str[i])+17)%26)+65;
    }
    int sum=0;
    for(int j=0;j<16;j++){   // always exactly 16 characters
        sum=0;
        for(int i=0;i<len;i++){
            if(i%16==j){
                sum+=temp[i];
            }
        }
        hashed+=(char)(sum%26+65);
    }
    return hashed;

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

    }
    return d;
    }

//CLASSES

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
    string hashed_password;
    float e = 0, n = 0;
public:
    friend class Wallet;
    string ID;
    //Functions for accounts we already have
    void set_read_account(float f,Currency currency_list[100],string id){
        balances[num_of_balance].amount=f;
        balances[num_of_balance].c=currency_list[num_of_balance];
        ID=id;
        num_of_balance++;
    }
    void setUserInfo(string str,float a,float b){
        hashed_password=str;
        e=a;
        n=b;
        }

    //Functions for new account

    void set_new_account(string str,int n,Currency currency_arr[100],string id){
        ID=id;
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

    void createKey(string ID,string hashed_password,Account accounts[100],int num_of_accounts){
        //create a list of prime numbers
        int prime_arr[num_of_accounts+2];
        int primecount=0;
        int i=101;
        while(primecount<num_of_accounts+2){
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
        bool flag=false;
        int prime_index=0,cnt;
        while(!flag){
            public_key=find_e(prime_arr[prime_index],prime_arr[prime_index+1]);
            private_key=find_d(prime_arr[prime_index],prime_arr[prime_index+1],public_key);
            n_forkeys=prime_arr[prime_index]*prime_arr[prime_index+1];
            cnt=0;
            for(int i=0;i<num_of_accounts;i++){
                if(public_key!=accounts[i].e || n_forkeys!=accounts[i].n){
                    cnt++;
                }
            }
            if(cnt== (num_of_accounts)) flag=true;
            else{prime_index++;}
        }
        cout<<"Your Private key is: "<<private_key<<endl;
        ofstream user_file;
        user_file.open("users.txt",std::ios::out|std::ios::app);
        user_file<<ID<<' '<<hashed_password<<' '<<public_key<<' '<<n_forkeys<<"\n";
        user_file.close();

    ;}
    void Fill_CurrencyFiles(){
        ofstream currency_file;
        string file_name;
        for(int i=0;i<num_of_balance;i++){
            file_name=balances[i].c.name+".txt";
            currency_file.open(file_name,std::ios::out|std::ios::app);
            currency_file<<ID<<' '<<balances[i].amount<<endl;
            currency_file.close();
            }
    }
    //function to Update account after operation
    void updateAccount(string currency,float new_amount,int line_number){
        string file_name=currency+".txt";
        fstream read_file;
        read_file.open(file_name);
        vector<string> lines;
        string line;
        while(getline(read_file,line)){
            lines.push_back(line);
        }
        read_file.close();

        ofstream write_file;
        write_file.open(file_name);
        for(int i=0;i<lines.size();i++){
            if(i!=line_number)
                write_file<<lines[i]<<endl;
            else
                {write_file<<ID<<' '<<new_amount<<endl;}
        }
        write_file.close();
    }


    void setSarafi(Account a){
        for(int i=0;i<a.num_of_balance;i++){
            balances[i].amount=a.balances[i].amount;
            balances[i].c=a.balances[i].c;
            ID=a.ID;
            num_of_balance++;}
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
    int error_index = 0;

    friend class Account;
    friend class Sarafi;



    string Login(Sarafi s){
        string username;
        string password;
        cout<<"Username: ";
        cin>>username;
        cout<<"Password: ";
        cin>>password;
        bool valid_user=false;
        for(int i=0;i<num_of_accounts;i++){
                if(username==accounts[i].ID && Hash_func(password)==accounts[i].hashed_password){
                    valid_user=true;}
            }
        if(username==s.ID && Hash_func(password)==s.hashed_password)
            {valid_user=true;}
        if(valid_user)
            {return username;}
        else
            {return "NULL";}
    }

    string Sign_up(){
        string new_username,new_password;
        cout<<"Username: ";
        cin>>new_username;
        cout<<"Password: ";
        cin>>new_password;
        Account new_acc;
        string account_num;
        cout<<"Enter account number:"<<endl;
        getline(cin>>ws,account_num);
        //Create e and n for user
        new_acc.createKey(new_username,Hash_func(new_password),accounts,num_of_accounts);
        new_acc.set_new_account(account_num,num_of_currency,currencys,new_username);
        new_acc.Fill_CurrencyFiles();
        addAccount(new_acc);
        return new_username;

        }

    void addReport(string user,string query,int sucess_id){
        ofstream log_file;
        log_file.open("log.txt",std::ios::out|std::ios::app);
        log_file<<user<<' '<<query<<' '<<sucess_id<<"\n";
        log_file.close();
    }


    void addCurrency(float f,string n){
        currencys[num_of_currency].value=f;
        currencys[num_of_currency].name=n;
        num_of_currency++;
    }
    void addAccount(Account acc){
    accounts[num_of_accounts]=acc;
    num_of_accounts++;
    }
    int Deposit(string instruct_words[4],Sarafi& sarafi_acc){
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
            //Update files
            accounts[co_index].updateAccount(currencys[c].name,accounts[co_index].balances[c].amount,co_index+1);
            sarafi_acc.updateAccount(currencys[c].name,sarafi_acc.balances[c].amount,0);
            }
        else{
            err="Insufficient Funds: ";
            err+=sarafi_acc.ID;
            errors[error_index]=err;
            error_index++;
            return 0;
        }
    return 1;
    }

    int Withdraw(string instruct_words[4],Sarafi& sarafi_acc){
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
            return 0;
        }
        else{
            accounts[co_index].balances[c].amount-=val;
            sarafi_acc.balances[c].amount+=val;
            //Update files
            accounts[co_index].updateAccount(currencys[c].name,accounts[co_index].balances[c].amount,co_index+1);
            sarafi_acc.updateAccount(currencys[c].name,sarafi_acc.balances[c].amount,0);
        }
    return 1;
    }


     int Exchange(string instruct_words[5],Sarafi& sarafi_acc){
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
            return 0;
            }
        else{
            //check error for sarafi

            if(val2>sarafi_acc.balances[c2].amount){
            err="Insufficient Funds: ";
            err+=sarafi_acc.ID;
            errors[error_index]=err;
            error_index++;
            return 0;
                }
            //do the exchange
            else{
            accounts[co_index].balances[c1].amount-=val1;
            sarafi_acc.balances[c1].amount+=val1;
            sarafi_acc.balances[c2].amount-=val2;
            accounts[co_index].balances[c2].amount+=val2;
            //Update files
            accounts[co_index].updateAccount(currencys[c1].name,accounts[co_index].balances[c1].amount,co_index+1);
            sarafi_acc.updateAccount(currencys[c1].name,sarafi_acc.balances[c1].amount,0);
            accounts[co_index].updateAccount(currencys[c2].name,accounts[co_index].balances[c2].amount,co_index+1);
            sarafi_acc.updateAccount(currencys[c2].name,sarafi_acc.balances[c2].amount,0);
            }
            }
        return 1;
        }

    int Transfer(string instruct_words[5],Sarafi& sarafi_acc){
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
            return 0;
            }
        else{
            //do the exchange
            accounts[co1].balances[c].amount-=(val+karmozd);
            sarafi_acc.balances[c].amount+=karmozd;
            accounts[co2].balances[c].amount+=val;
            //Update files
            accounts[co1].updateAccount(currencys[c].name,accounts[co1].balances[c].amount,co1+1);
            sarafi_acc.updateAccount(currencys[c].name,sarafi_acc.balances[c].amount,0);
            accounts[co2].updateAccount(currencys[c].name,accounts[co2].balances[c].amount,co2+1);
            }
        return 1;

    }

    bool ValidCommand(string instruct_Words[4]){
        bool invalid=true;
        if(instruct_Words[0]=="WITHDRAW")invalid=false;
        else if(instruct_Words[0]=="DEPOSIT")invalid=false;
        else if(instruct_Words[0]=="EXCHANGE")invalid=false;
        else if(instruct_Words[0]=="TRANSFER")invalid=false;
        else if(instruct_Words[0]=="REPORT")invalid=false;

        if(invalid){
            errors[error_index]="Invalid Command";
            error_index++;
        }
        return (!invalid);
    }

    bool ValidID(string instruct_Words[5],string user){
        // DEPOSIT is issued by the exchange account on behalf of a customer,
        // so the target only has to be an existing account.
        if(instruct_Words[0]=="DEPOSIT"){
            for(int i=0;i<num_of_accounts;i++){
                if(accounts[i].ID==instruct_Words[1]) return true;
            }
            return false;
        }
        // every other command must be about the logged-in user's own account
        return user==instruct_Words[1];
        }
    bool commandAccess(string instruct_Words[5],string user,Sarafi s){
        bool valid=true;
        if(instruct_Words[0]=="DEPOSIT" && s.ID!=user){
            valid=false;
        }
        if(s.ID==user && instruct_Words[0]!="DEPOSIT"){
            valid=false;
        }
        return valid;
    }

    void printReport(string username){
        fstream log_file;
        log_file.open("log.txt",ios::in);
        string name,line;
        while(getline(log_file,line)){
                name="";
                for(int i=0;i<line.length();i++){
                    if(line[i]==' ') break;
                    else name+=line[i];
                }
                if(name==username)
                    cout<<line<<endl;
        }
        log_file.close();
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
void getInfo(Wallet& my_wallet,Sarafi& my_sarafi){
    fstream currency_file;
    currency_file.open("currencies.txt",ios::in);
    string n;
    float f;
    int cnt;
    my_wallet.num_of_currency=0;
    while(currency_file>>n>>f){
            my_wallet.addCurrency(f,n);
    }
    currency_file.close();
    Account read_acc[100];
    for(int i=0;i<my_wallet.num_of_currency;i++){
            string file_name=my_wallet.currencys[i].name+".txt";
            currency_file.open(file_name,ios::in);
            cnt=0;
            while(currency_file>>n>>f){
                read_acc[cnt].set_read_account(f,my_wallet.currencys,n);
                cnt++;
            }
            currency_file.close();
    }
    my_sarafi.setSarafi(read_acc[0]);
    my_wallet.num_of_accounts=0;
    for(int i=1;i<cnt;i++){
        my_wallet.addAccount(read_acc[i]);
    }
    //Get info from users file
    fstream user_file;
    user_file.open("users.txt",ios::in);
    string hash_p;
    float e,nn;
    cnt=0;
    while(user_file>>n>>hash_p>>e>>nn){
        if(cnt==0){my_sarafi.setUserInfo(hash_p,e,nn);}
        else{
        my_wallet.accounts[cnt-1].setUserInfo(hash_p,e,nn);
        }
        cnt++;
    }

}


int main(){
    Wallet bank_wallet;
    Sarafi sarafi_acc;
    getInfo(bank_wallet,sarafi_acc);
    //Console
    cout<<"Hi.Welcome to our Bank!"<<endl;
    cout<<"Login or Sign_up ? "<<endl;
    string command;
    cin>>command;
    string user_ID;
    if(command=="Login"){
        user_ID=bank_wallet.Login(sarafi_acc);
        if(user_ID=="NULL"){
            cout<<"!!Invalid account!!"<<endl;
            cout<<"Please Sign_up :"<<endl;
            user_ID=bank_wallet.Sign_up();
        }
        cout<<"Enter operations: "<<endl;
        cout<<"To end the program enter Exit"<<endl;
        getline(cin>>ws,command);
        }
    else if(command=="Sign_up"){
        user_ID=bank_wallet.Sign_up();
        cout<<"Enter operations: "<<endl;
        cout<<"To end the program enter Exit"<<endl;
        getline(cin,command);
    }
    if(command!="Exit"){
        int success_rate=1;
        //Get words of instruct
        string instruct_Words[50];
        int counter = 0;
        for(int j=0;j<command.length();j++){
            if(command[j] == ' '){
                counter++;
            }else{

            instruct_Words[counter] += command[j]; }
            }
         //Check command and ID
         bool valid_command=bank_wallet.ValidCommand(instruct_Words);
         bool command_access=bank_wallet.commandAccess(instruct_Words,user_ID,sarafi_acc);
         bool valid_ID=bank_wallet.ValidID(instruct_Words,user_ID);
        //Find which instruction and do
        if(valid_command && valid_ID && command_access){
            if(instruct_Words[0]=="DEPOSIT"){
                success_rate=bank_wallet.Deposit(instruct_Words,sarafi_acc);
            }else if(instruct_Words[0]=="WITHDRAW"){
                success_rate=bank_wallet.Withdraw(instruct_Words,sarafi_acc);

            }else if(instruct_Words[0]=="EXCHANGE"){
                success_rate=bank_wallet.Exchange(instruct_Words,sarafi_acc);
            }else if(instruct_Words[0]=="TRANSFER"){
                success_rate=bank_wallet.Transfer(instruct_Words,sarafi_acc);
            }else if(instruct_Words[0]=="REPORT"){
                bank_wallet.printReport(user_ID);
            }
        }else{success_rate=0;}
        //Add command and success rate to log.txt
        bank_wallet.addReport(user_ID,command,success_rate);
        if(instruct_Words[0]=="TRANSFER"){
                bank_wallet.addReport(instruct_Words[2],command,success_rate);
        }
        getline(cin,command);
    }
}
