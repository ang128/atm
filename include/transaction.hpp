#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <signal.h>

class Transaction
{
public:
  static Transaction& getInstance()
  {
    static Transaction instance;
    return instance;
  }

  bool setupTimer();
  bool startTimer(int timeout);
  bool stopTimer();
  bool deleteTimer();  
  void setAccount( string& account);
  string& getAccount() { return account; }
  void setAuthorized(bool authorize);
  bool getAuthorized();
  timer_t getTimerId();
  void resetInstance();
  
private:
  Transaction();
  ~Transaction();

  //  static Transaction *instance;
  string account;
  timer_t timerid;
  bool authorized;  

};

void timeoutHandler( sigval_t This );
void timeoutHandler2(int sig, siginfo_t *si, void *uc);
#endif
