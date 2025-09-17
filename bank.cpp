#include<bits/stdc++.h>
using namespace std; 
class Transaction {
   private:
   double amount;
   string type;
   string date;
   public:
   Transaction(double a, string t, string d) : 
   amount(a), type(t), date(d) {}
   void display () {
    cout << "Amount: " << amount << endl;
    cout << "Type: " << type << endl;
    cout << "date: " << date << endl;
   }
   double getamount() {
    return amount;
   }
   string gettype() {
    return type;
   }
   string getdate(){
    return date;
   }
   friend ostream& operator << (ostream& os,const Transaction& t ){
    os << "Amount: " << t.amount <<" "<< "Type: " << t.type <<" " << "Date: " << t.date << endl;
    return os;
   }
   bool operator == (const Transaction& other) const{
      return amount == other.amount && type == other.type && date == other.date;
   }
};

class Account {
   protected:
   int accountnumber;
   double balance;
   string ownername;
   vector<Transaction> history; 
   public:
   Account(int ac, double ba , string ow) :
   accountnumber(ac), balance(ba), ownername(ow) {}
   bool deposit (double money){
    if(money <= 0) return false;
     balance += money;
     Transaction t(money, "deposit", "today");
     history.push_back(t);
     cout << "Transaction done: " << t;
     return true;
   }
   virtual bool withdraw( double money) {
    if(money > balance) {
        cout << "Not enough balance to withdraw." << endl;
        return false;
    }
     balance -= money;
      Transaction t(money, "withdraw", "today");
     history.push_back(t);
      cout << "Transaction done: " << t;
     return true;
   }
   bool tranfer(Account* ac, double money ) {
      if(money <= 0) return false;
      if(money > balance) {
         cout << "Not enough balance to withdraw." << endl;
        return false;
      }
      balance -= money;
      Transaction t1(money,"Tranfer-out", "today");
      history.push_back(t1);
      cout << "Transaction done: " << t1;
      ac->balance += money;
      Transaction t2(money,"Tranfer-in", "today");
      ac->history.push_back(t2);
      cout << "Transaction done: " << t2;
      return true;
   }
   double getbalance() {
    return balance;
   }
   void displayinfo() {
    cout << "accountNumber: " << accountnumber << endl;
    cout << "Balance: " << balance << endl;
    cout << "ownerName: " << ownername << endl;
   }
   friend ostream& operator << (ostream& os, Account& ac) {
    os << "accountNumber: " << ac.accountnumber << endl 
     << "Balance: " << ac.balance << endl
    << "ownerName: " << ac.ownername << endl;
    return os;
   }
   
   Account operator += (Transaction& t) {
     if(t.gettype() == "deposit"){
        balance += t.getamount();
        history.push_back(t);
        cout << "Transaction done: " << t;
     }
     else if(t.gettype() == "withdraw") {
        if(balance < t.getamount()) {
            cout << "Not enough balance to withdraw" << endl;
        }
        else balance -= t.getamount();
        history.push_back(t);
        cout << "Transaction done: " << t;
     }
     return *this;
   }
};

class SavingAccount: public Account {
   private:
   double interestRate;
   public:
   SavingAccount(int acc, double ba, string ow, double in) :
   Account(acc,ba,ow) , interestRate(in) {}
   void addinteresrate() {
    double inter = balance*(interestRate/100);
    balance += inter;
    Transaction t(inter,"interest","today");
    history.push_back(t);
    cout << "Transaction done: " << t;
   }
   bool withdraw (double money ) override {
       if(money > balance) {
        cout << "Insufficient funds in Savings Account!" << endl;
       return false;
       }
       double fee = money * 0.01;
       balance -= (money + fee);
       Transaction t1(money,"withdraw","today");
       Transaction t2(fee,"fee","today");
       history.push_back(t1);
       history.push_back(t2);
       cout << "Transaction done: " << t1;
       cout << "Transaction done: " << t2;
        
       return true; 
   }
   

};

class Customer {
   private:
   string name;
   int id;
   vector<Account*> account;
   public:
    Customer(string n, int i):
    name(n), id(i) {}
    void openaccount(Account* ac) {
       account.push_back(ac);
    }
    double totalallbalance() {
        double total = 0.0;
        for(auto &a: account) {
            total += a->getbalance();
        }
          return total;
    }
    
    void displayinfo1() {
        cout << "Customer: " << name << endl;
        cout << " | ID: " << id << endl;
        cout << "Accounts:" << endl;
        for (auto acc : account) {
            cout << *acc << endl;
        }
        cout << "Total Balance: " << totalallbalance() << endl;
    }
};

int main() {
    Customer c1("Alice", 101);

    Account* acc1 = new Account(1001, 500, "Alice");
    SavingAccount* acc2 = new SavingAccount(1002, 1000, "Alice", 5.0);

    c1.openaccount(acc1);
    c1.openaccount(acc2);

    cout << "\n--- Test deposit ---\n";
    acc1->deposit(200); 
    acc1->displayinfo();

    cout << "\n--- Test withdraw (success) ---\n";
    acc1->withdraw(100); 
    acc1->displayinfo();

    cout << "\n--- Test withdraw (fail) ---\n";
    if (!acc1->withdraw(10000)) {
        cout << "Withdraw failed: not enough balance." << endl;
    }
    acc1->displayinfo();

    cout << "\n--- Test operator += ---\n";
    Transaction t1(300, "deposit", "today");   
    Transaction t2(200, "withdraw", "today");   
    *acc1 += t1;
    *acc1 += t2;
    acc1->displayinfo();

    cout << "\n--- Test SavingAccount interest ---\n";
    acc2->addinteresrate(); 
    acc2->displayinfo();

    cout << "\n--- Test SavingAccount withdraw ---\n";
    acc2->withdraw(200); 
    acc2->displayinfo();

    
    cout << "\n--- Test transfer ---\n";
    acc1->tranfer(acc2, 300);   
    acc1->displayinfo();
    acc2->displayinfo();


    cout << "\n--- Test Transaction comparison ---\n";
    Transaction t3(300, "deposit", "today");
    if (t1 == t3) cout << "two accounts are similar" << endl;
    else cout << "two accounts are different" << endl;

    cout << "\n--- Customer info ---\n";
    c1.displayinfo1();
   

    return 0;
}
