#include <iostream>
#include <string>

#ifndef CUSTOMER_H
#define CUSTOMER_H

using namespace std;

struct Record {
  string actnumber;
  string pin;
  double balance;
};


class TxHistory
{
public:
  TxHistory(string& date, string& time, double amount, double balance);
  ~TxHistory();

  string& getDate() {return date;}
  string& getTime() {return time;}
  double getAmount() {return amount;}
  double getBalance() {return balance;}

private:
  string date;    //date of the transaction
  string time;    //time of the transaction
  double amount;  //final amount of the transaction
  double balance; //balance after the transaction
};

class Customer 
{
public:
  Customer(string& act, string& pin, double balance);
  ~Customer();

  double get_balance();
  void set_balance(double amount);
  void set_overdrawnFee(double fee);
  double get_overdrawnFee();
  void update_history(double amount);
  unsigned int get_historySize();
  void show_historyInfo();
  void get_TimeAndDate(string& date, string&time);
  string& get_account();
  string& get_pin();

private: 
  string act;
  string pin;
  double balance;
  double overdrawnFee;
  list<TxHistory *> txHistory;
};

#define DATE_MAX_STR_SIZE 32

#endif
