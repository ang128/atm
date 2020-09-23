
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <locale>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "customer.hpp"
#include "command.hpp"
#include "transaction.hpp"
#include "machine.hpp"

using namespace std;


//only 4 customer Records are given
Record custRecord[] = { {"2859459814", "7386", 10.24},
                        {"1434597300", "4557", 90000.55},
                        {"7089382418", "0075", 0.00},
                        {"2001377812", "5950", 60.00}
                       };

string validCmd[7] = {"authorize",
                      "withdraw",
                      "deposit",
                      "balance",
                      "history",
                      "logout",
                      "end"
                     };

Transaction *_transaction = NULL;
vector<Customer *> customerVec;
Machine *atmMachine=NULL;

/*
 * process_cmd
 * Responsibility:
 *  - tokenize the command string 
 *  - convert the command to lower case
 * Parameters:
 *  - command input string
 *  - tokenized command string on return
 * Return:
 *  -none
 */
void process_cmd(string& cmdIn, vector<string>& token)
{
  char *pstr = NULL;

  //empty command
  if (cmdIn.empty()) {
    return;
  }

  //tokenize the cmd based on the whitespace in between 
  pstr = strtok((char *)cmdIn.c_str(), " ");
  while (pstr != NULL) {
    token.push_back(string(pstr));
    pstr = strtok(NULL, " ");
  }

  //convert into the lowercase command
  transform(token.at(0).begin(), token.at(0).end(), token.at(0).begin(), ::tolower);
  
  return;
}

/*
 * cleanup_resources
 * Responsibility:
 *  - clean up customer
 *  - clean up history
 *  - clean up atm Machine
 *  - clean up transaction block
 *  - clean up timer
 * Parameters:
 *  - none
 * Return:
 *  - none
 */
void cleanup_resources()
{
  vector<Customer *>::iterator it;

  //iterate thru customers and cleanup
  for (it = customerVec.begin(); it != customerVec.end(); ++it) {
    //found the customer record
    delete (*it);
  } 
 
  if (atmMachine) {
    delete atmMachine;
  }

  if (_transaction) {
    //remove all related resources
    _transaction->resetInstance();
  }  

}


/*
 * mapCommand
 * Responsibility:
 * - map command string to a number
 * Parameters:
 *  - command string
 * Return:
 *  - command number
 */
Commands mapCommand(string& cmd)
{
  if (cmd == "authorize") return Authorize;
  if (cmd == "withdraw") return Withdraw;
  if (cmd == "deposit") return Deposit;
  if (cmd == "balance") return Balance;
  if (cmd == "history") return History;
  if (cmd == "logout") return Logout;
  if (cmd == "end") return End;
  
  //invalid command
  return Invalid;
}


/*
 * main
 * Responsibility:
 * - main entry point to the program
 * Parameters:
 *  - none
 * Return:
 *  - none
 */
int main() 
{
  string cmdIn;
  bool bExit=false;
  
  //create machine and set the initial balance in the machine
  atmMachine = new Machine(MACHINE_INITIAL_BALANCE);
  if (!atmMachine) {
    cout << "Failed to create atm machine" << endl;
    return (0);
  }

  /*create customer account that's authorized to carry out transactions on the
   *machine
   */
  Customer *tmpCustomer;
  int sz = sizeof(custRecord)/sizeof(custRecord[0]);
  for (int i=0; i<sz; i++) {
    tmpCustomer = new Customer(custRecord[i].actnumber, 
                               custRecord[i].pin,
                               custRecord[i].balance);
    if (tmpCustomer) {
      customerVec.push_back(tmpCustomer);
    }
    else {
      //display error in adding customer, but continue to the next one
      cout << "Failed to add account: " << custRecord[i].actnumber << endl;
    }
  }

  vector<string> cmdtoken;
  //look for input
  while (1) {
    int res = 0;

    //clean the buffer
    cmdIn.clear();
    cmdtoken.clear();

    //just to display a prompt for user
    cout << "\nPlease enter command > ";
    getline (cin, cmdIn);

    //got something from input, stop the running timer
    if (_transaction && _transaction->getAuthorized()) {
      _transaction->stopTimer();
    }

    //process the command string
    process_cmd(cmdIn, cmdtoken);

    //empty command, just continue
    if (cmdtoken.empty()) {
      continue;
    }
    
    //carry out the command
    switch (mapCommand(cmdtoken.at(0))) {
    case Authorize:
      {
        /*
         *check if there is an outstanding authorize cmd by checking
         *transaction block because the block is instantiated in authorize
         *and it is a singleton
         */
        if (_transaction && _transaction->getAuthorized()) {
          /* i.e. an outstanding authorize exists, reject the current one and
           *logout the outstanding one. let user starts over again.
           * Cannot have more than one person to login.
           */
          cout << "Authorization failed" << endl;
          _transaction->resetInstance();
          break;
        }
        AuthorizeCmd *cmdp = new AuthorizeCmd();
        if (cmdp) {
          cmdp->process_command(cmdtoken);
          delete cmdp;
        }
      }
      break;

    case Withdraw:
      //if transaction block is NULL or not authorized, do not process the cmd
      if (_transaction == NULL || !_transaction->getAuthorized()) {
        cout << "Authorization required" << endl;
      }
      else {
        WithdrawCmd *cmdp = new WithdrawCmd();
        if (cmdp) {
          cmdp->process_command(cmdtoken);
          delete cmdp;	
        }
      }
      break;

    case Deposit:
      //if transaction block is NULL or not authorized, do not process the cmd
      if (_transaction == NULL || !_transaction->getAuthorized()) {
        cout << "Authorization required" << endl;
      }
      else {      
        DepositCmd *cmdp = new DepositCmd();
        if (cmdp) {
          cmdp->process_command(cmdtoken);
          delete cmdp;	  
        }
      }
      break;

    case Balance:
      //if transaction block is NULL or not authorized, do not process the cmd
      if (_transaction == NULL || !_transaction->getAuthorized()) {
        cout << "Authorization required" << endl;
      }
      else {            
        BalanceCmd *cmdp = new BalanceCmd();
        if (cmdp) {
          cmdp->process_command(cmdtoken);
          delete cmdp;
        }
      }
      break;

    case History:
      //if transaction block is NULL or not authorized, do not process the cmd
      if (_transaction == NULL || !_transaction->getAuthorized()) {
        cout << "Authorization required" << endl;
      }
      else {            
        HistoryCmd *cmdp = new HistoryCmd();
        if (cmdp) {
          cmdp->process_command(cmdtoken);
          delete cmdp;
        }
      }
      break;

    case Logout:
      //if transaction is valid and authorized,log it out. otherwise ignore it
      if (_transaction || _transaction->getAuthorized()) {
        LogoutCmd *cmdp = new LogoutCmd();
        if (cmdp) {
          cmdp->process_command(cmdtoken);
          delete cmdp;
        }
      }
      break;

    case End:
      bExit = true;
      break;

    default:
      //invalid command
      cout << "Unsupported command entered. Please try again." << endl;
      break;
    }
 
    //find the transaction 
    if (_transaction) {
      if (_transaction->getTimerId() == 0) {
        //setup timer for the next command  
        if (!_transaction->setupTimer()) {
          //if timer cannot be establised something drastically wrong with the system
          //exit out and rerun the app
          cout << "DEBUG: Failed to establish timer. Exiting" << endl;
          bExit = true;
        }
      }
      //either timer setup is good or timer already exists, now start timer
      if (!bExit) {
        _transaction->startTimer(COMMAND_TIMEOUT_SECONDS);
      }
    }
    
    if (bExit) {
      //TODO cleanup everything
      cleanup_resources();      
      break;
    }
  } //while

  return(0);
}
