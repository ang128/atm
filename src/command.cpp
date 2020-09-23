
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <locale>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "customer.hpp"
#include "command.hpp"
#include "transaction.hpp"
#include "machine.hpp"


extern Record custRecord[];
extern Transaction *_transaction;
extern vector<Customer *> customerVec;
extern Machine *atmMachine;;

//Base Command Class
Command::Command()
{
}

Command::~Command()
{
}

void Command::process_command(vector<string>& cmd)
{
}


/*
 * find_customer
 * Responsibility:
 *  - find customer by account number
 * Parameters:
 *  - account 
 * Returns
 *  - pointer to the customer record
 *  - 0 if no record found
 */
Customer *Command::find_customer(string& account)
{
  vector<Customer *>::iterator it;

  for (it = customerVec.begin(); it != customerVec.end(); ++it) {
    if (strcmp((*it)->get_account().c_str(), account.c_str()) == 0) {
      //found the customer record
      return *it;
    }
  }
  cout << "DEBUG: no customer record found" << endl;
  return 0;
}

/*
 * get_account
 * Responsibility:
 *  - get account number from the authorized transaction block
 * Parameters:
 *  - none
 * Returns
 *  - String: account numnber
 */
string Command::get_account() 
{

  if (_transaction && _transaction->getAuthorized()) {
    //get account from the authorized transaction
    return (_transaction->getAccount());
  }
  else {
    return ("");
  }
}

/*
 * validate amount
 * Responsibility:
 *  - validate the amount is a valid numeric string. "$" and "." are allowed
 * Parameters:
 *  - amount string
 *  - place holder for the amount in double
 * Returns
 *  - true - a valid string
 *  - false - string with other non-numeric characters
 */
bool Command::validate_amount(string& amount, double& amountD) 
{
  size_t pos;

  amountD=0;

  //"$" must be the first character if it presents
  pos = amount.find_first_of("$");
  if (pos != 0 && pos != string::npos) {
    cout << "Unable to process the amount" << endl;
    return false;
  }
  
  //skip over the dollar sign if it presents
  if (pos == 0) {
    amount.erase(0,1);
  }

  //check for other non-numeric characters
  for (int i=0; i<amount.size(); i++) {
    if (!isdigit(amount[i]) && amount[i] != '.') {
        cout << "DEBUG: Unable to process the amount" << endl;
        return false;
    }
  }
  //convert it to float
  amountD = atof(amount.c_str());

  return true;
}

//Authorize command class
AuthorizeCmd::AuthorizeCmd()
{
}

AuthorizeCmd::~AuthorizeCmd()
{
}

/*
 * AuthorizeCmd constructor
 * Responsibility:
 *  - validate the account and pin
 *  - instantiate a singleton transaction block for subsequent transactions
 * Parameters:
 *  - tokenized cmd with account number and pin number
 * Return:
 *  -authorized flas is set to true if account and pin match up with the record
 *  -authorized flag is set to false if otherwise
 */
void AuthorizeCmd::process_command(vector<string>& cmd)
{
  char *account;
  char *pin;

  //verify the number of cmd parameters 
  if (cmd.size() != AUTHORIZECMD_NUM_PARAMS) {
    cout << "Authorization required" << endl;
    return;  
  }
  
  //verify the account and pin
  account = (char *)cmd.at(1).c_str();
  pin = (char *)cmd.at(2).c_str();
  for (int i=0; i<customerVec.size(); i++) {
    if (strcmp(account, customerVec.at(i)->get_account().c_str()) == 0) {
      //found a match on the account number
      //now verify the pin
      if (strcmp(pin, customerVec.at(i)->get_pin().c_str()) == 0) {
        //instantiate a transaction block
        _transaction = &Transaction::getInstance();
        _transaction->setAccount(customerVec.at(i)->get_account());
        _transaction->setAuthorized(true);
	cout << account << " successfuly authorized" << endl;
        return;
      }
    }
  }
  cout << "Authorization failed" <<endl;
}

//Withdraw command class
WithdrawCmd::WithdrawCmd()
{
}

WithdrawCmd::~WithdrawCmd()
{
}

/*
 * WithdrawCmd::process_command
 * Responsibility:
 *  - validate the amount string is valid, i.e no other funny characters
 *  - validate the amount is in multiple of $20
 *  - check the balance of the account has sufficient amount
 *  - check the atm machine has sufficient funding
 *  - carry out the transaction 
 *  - allocate a history block if no error
 * Parameters:
 *  - tokenized cmd with command and amount
 * Return:
 *  - display message
 */
void WithdrawCmd::process_command(vector<string>& cmd)
{
  string amount;
  Customer *customer;
  double amountD;

  //get the amount of withdraw
  amount = cmd.at(1);
  //validate the amount string 
  if (!validate_amount(amount, amountD)) {
    //invalid string, simply return
    return;
  }

  //get the customer record
  string account = get_account();
  customer = Command::find_customer(account);
  if (!customer) {
    //just return since nothing can be done here
    return;
  }

  //check if amount is valid
  //customer could be entered a zero amount or error in converting from string to flot
  //treat them as zero
  if (amountD == 0) {
    cout << "Amount dispensed: $0" << endl;
    cout << "Current balance: " << customer->get_balance() << endl;
    customer->update_history(amountD);
    return;
  }

  //check the account if it's been overdrawn
  if (customer->get_balance() < 0) {
    cout << "Your account is overdrawn! You may not make withdrawals at this time" << endl;
    return;
  }

  //check if the amount is multiple of $20
  if ((long long)amountD % WITHDRAWAL_MULTIPLIER) {
    cout << "Withdrawl amount must be a mupltiple of $20" << endl;
    return;
  }
  
  //chaeck the balance in the machine
  if (atmMachine) {
    if (atmMachine->get_balance() == 0) {
      cout << "Unable to process your withdrawal at this time." << endl;
      return;
    } else if (atmMachine->get_balance() < amountD) {
      //machine has less money than the withdraw amount
      //update withdraw amount to be the balance in the machine
      amountD = atmMachine->get_balance();
      cout << "Unable to dispense dispense full amount request at this time." << endl;
    }
  }

  //check the balance of the account 
  if (customer->get_balance() < amountD) {
    //overdraw, needs to add fee 
    customer->set_overdrawnFee(OVERDRAWN_FEE);
  }
    
  //everuthing is good to go
  //update account and machine with the amount
  customer->set_balance(customer->get_balance() - (amountD + customer->get_overdrawnFee()));
  atmMachine->set_balance(atmMachine->get_balance() - amountD);
  cout << "Amount dispensed: $" << amountD << endl;
  if (customer->get_overdrawnFee()) {
    cout << "You have been charged an overdraft fee of $5. Current balance:" << customer->get_balance() << endl; 
  }
  else {
    cout << "Current balance: " << customer->get_balance() << endl;
  }

  //update history block for the account
  customer->update_history(amountD);
}


//Deposit command class
DepositCmd::DepositCmd()
{
}

DepositCmd::~DepositCmd()
{
}


/*
 * DepositCmd::process_command
 * Responsibility:
 *  - validate the amount string is valid, i.e no other funny characters
 *  - carry out the transaction 
 *  - allocate a history block if no error
 * Parameters:
 *  - tokenized cmd with command and amount
 * Return:
 *  - display message
 */
void DepositCmd::process_command(vector<string>& cmd)
{
  string amount;
  double amountD;
  Customer *customer;

  //get the amount of withdraw
  amount = cmd.at(1);
  //validate the amount string 
  if (!validate_amount(amount, amountD)) {
    //invalid string, simply return
    return;
  }

  //get the customer record
  string account = get_account();
  customer = find_customer(account);
  if (!customer) {
    //just return since nothing can be done here
    return;
  }

  //update customer record
  customer->set_balance(customer->get_balance() + amountD);
  cout << "Current balance: " << customer->get_balance() << endl;

  //update history list
  customer->update_history(amountD);
}


//Balance command class
BalanceCmd::BalanceCmd()
{
}

BalanceCmd::~BalanceCmd()
{
}

/*
 * BalanceCmd::process_command
 * Responsibility:
 *  - carry out the transaction 
 * Parameters:
 *  - tokenized cmd with command and amount
 * Return:
 *  - display balance message
 */
void BalanceCmd::process_command(vector<string>& cmd)
{
  Customer *customer;

  //get the customer record
  string account = get_account();
  customer = find_customer(account);
  if (!customer) {
    //just return since nothing can be done here
    return;
  }

  //update customer record
  cout << "Current balance: " << customer->get_balance() << endl;

}


//History command class
HistoryCmd::HistoryCmd()
{
}

HistoryCmd::~HistoryCmd()
{
}

/*
 * HistoryCmd::process_command
 * Responsibility:
 *  - Find the customer
 *  - looks into the history list
 * Parameters:
 *  - tokenized cmd with command and amount
 * Return:
 *  - display history info
 */
void HistoryCmd::process_command(vector<string>& cmd)
{
  Customer *customer;

  //get the customer record
  string account = get_account();
  customer = find_customer(account);
  if (!customer) {
    //just return since nothing can be done here
    return;
  }

  //check the history list
  if (customer->get_historySize() == 0) {
    cout << "No history found" << endl;
    return;
  }

  customer->show_historyInfo();
}


//Logout command class
LogoutCmd::LogoutCmd()
{
}

LogoutCmd::~LogoutCmd()
{
}

/*
 * LogoutCmd::process_command
 * Responsibility:
 *  - Find the customer
 *  - clean up transaction block
 *  - clean up timer
 * Parameters:
 *  - tokenized cmd with command and amount
 * Return:
 *  - display history info
 */
void LogoutCmd::process_command(vector<string>& cmd)
{
  Customer *customer;
  string act;

  //get the account before it is free
  act = get_account();

  //if no valid active account
  if (act.empty()) {
    cout << "No account is currently authorized" << endl;
    return;
  }

  //get the customer record
  customer = find_customer(act);
  if (!customer) {
    //just return since nothing can be done here
    return;
  }

  if (_transaction) {
    //reset transaction resources
    _transaction->resetInstance();
  }  
  
  //message to user
  cout << "Account " << act << " logged out" << endl;

}



//End command class
EndCmd::EndCmd()
{
}

EndCmd::~EndCmd()
{
}

void EndCmd::process_command(vector<string>& cmd)
{

}
