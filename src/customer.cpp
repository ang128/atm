#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <locale>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <string.h>

#include "customer.hpp"
#include "command.hpp"
#include "transaction.hpp"
#include "machine.hpp"

/*
 *Customer Constructor
 * - initializing customer's account number, pin, and balance
 */
Customer::Customer(string& account, string& pin_number, double amount)
{
  act = account;
  pin = pin_number;
  balance = amount;
  overdrawnFee = 0;
}

/*
 *Customer Destructor
 * - cleanup history list
 */
Customer::~Customer()
{
  for (list<TxHistory *>::const_iterator it=txHistory.begin(); it!=txHistory.end(); ++it) {
    delete (*it);
  }
}


/*
 * get_account
 * Responsibility:
 *  - returns the accuont number
 * Parameters:
 *  - none
 * Returns
 *  - account string
 */
string& Customer::get_account()
{
  return act;
}


/*
 * get_account
 * Responsibility:
 *  - returns the pin
 * Parameters:
 *  - none
 * Returns
 *  - pin string
 */
string& Customer::get_pin()
{
  return pin;
}

/*
 * get_balance
 * Responsibility:
 *  - returns balance in the customer account
 * Parameters:
 *  - none
 * Returns
 *  - balance value
 */
double Customer::get_balance()
{
  return balance;
}

/*
 * set_balance
 * Responsibility:
 *  - set balance in the customer account
 * Parameters:
 *  - amount to be set in the balance
 * Returns
 *  - none
 */
void Customer::set_balance(double amount)
{
  balance = amount;;
}

/*
 * set_overdrawnFee
 * Responsibility:
 *  - set overdrawn fee for the account
 * Parameters:
 *  - fee amount
 * Returns
 *  - none 
 */
void Customer::set_overdrawnFee(double fee)
{
  overdrawnFee = fee;
}

/*
 * get_overdrawnFee
 * Responsibility:
 *  - returns the overdrawnFee in the account
 * Parameters:
 *  - none
 * Returns
 *  - overdrawnFee value
 */
double Customer::get_overdrawnFee()
{
  return overdrawnFee;
}

/*
 * get_historySize
 * Responsibility:
 *  - returns the length of the history list
 * Parameters:
 *  - none
 * Returns
 *  - list size
 */
unsigned int Customer::get_historySize()
{
  return (txHistory.size());
}

/*
 * update_history
 * Responsibility:
 *  - update history list with a new transaction
 * Parameters:
 *  - the final amount of the transaction
 * Returns
 *  - none
 */
void Customer::update_history(double amount)
{
  string txdate;
  string txtime;

  //get date and time
  get_TimeAndDate(txdate, txtime);

  //get a new history block and set info
  TxHistory *_txhistory = new TxHistory(txdate, txtime, amount, get_balance());
  if (_txhistory == NULL) {
    cout << "Failed to get history object" << endl;
    return;
  }
 
  //push to the front of the list
  txHistory.push_front(_txhistory);
}

/*
 * get_TimeAndDate
 * Responsibility:
 *  - get current time and date
 * Parameters:
 *  - place holder for date
 *  - place holder for time
 * Returns
 *  - none
 */
void Customer::get_TimeAndDate(string& txdate, string& txtime)
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[DATE_MAX_STR_SIZE];
    tstruct = *localtime(&now);

    //convert to date string
    strftime(buf, DATE_MAX_STR_SIZE, "%Y-%m-%d ", &tstruct);
    txdate.assign(&buf[0]);

    //convert to time string
    memset(&buf[0], 0, sizeof(buf));
    strftime(buf, DATE_MAX_STR_SIZE, "%X ", &tstruct);
    txtime.assign(&buf[0]);
}

/*
 * show_historyInfo
 * Responsibility:
 *  - show all the history associated with the customer
 * Parameters:
 *  - none
 * Returns
 *  - none
 */
void Customer::show_historyInfo()
{
  //output the heading
  cout << setw(15) << left << "<date>" << setw(15) << left << "<time>" << setw(20) << left << "<amount>" << setw(30) << left << "<balance after transaction>" << endl;

  //iterate thru the history list
  //display the info
  for (list<TxHistory *>::iterator it=txHistory.begin(); it!=txHistory.end(); ++it){
    cout << setw(15) << left << (*it)->getDate().c_str() << setw(15) << left << (*it)->getTime().c_str() << setw(20) << left << (*it)->getAmount() << setw(30) << left << (*it)->getBalance() << endl;
  }

}


//History Class
/*
 * History constructor
 * Responsibility:
 *  - initialize the block with the data associate with a transaction
 * Parameters:
 *  - date of the transaction
 *  - time of the transaction
 *  - amount of the transaction
 *  - balance after the transaction
 * Returns
 *  - none
 */
TxHistory::TxHistory(string& txdate, string& txtime, double txamount, double txbalance)
{
  date = txdate;
  time = txtime;
  amount = txamount;
  balance = txbalance;
}

/*History destructor
 */
TxHistory::~TxHistory()
{
}
