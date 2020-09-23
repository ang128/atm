#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <locale>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "customer.hpp"
#include "command.hpp"
#include "transaction.hpp"
#include "machine.hpp"

using namespace std;

/*
 * Transaction constructor
 * Responsibility:
 *  - set authorized to false
 * Parameters:
 *  - none
 * Return:
 *  - none
 */
Transaction::Transaction()
{
    authorized = false;
}

/*
 * Transaction destructor
 * Responsibility:
 *  - none
 * Parameters:
 *  - none
 * Return:
 *  - none
 */
Transaction::~Transaction()
{}

/*
 * setAccount
 * Responsibility:
 *  - sets the account of the transaction
 * Parameters:
 *  - account
 * Return:
 *  - none
 */
void Transaction::setAccount(string& act)
{
  account = act;
}

/*
 * setAuthorized
 * Responsibility:
 *  - sets the "authorized" flag accordingly
 * Parameters:
 *  - flag value
 * Return:
 *  - none
 */
void Transaction::setAuthorized(bool authorize)
{
  authorized = authorize;
}

/*
 * getAuthorized
 * Responsibility:
 *  - returns the "authorized" flag
 * Parameters:
 *  - none
 * Return:
 *  - authorized flag
 */
bool Transaction::getAuthorized()
{
  return authorized;
}

/*
 * getTimerId
 * Responsibility:
 *  - returns timer id
 * Parameters:
 *  - none
 * Return:
 *  - timerid
 */
timer_t Transaction::getTimerId()
{
  return timerid;
}


/*
 * resetInstance
 * Responsibility:
 *  - reset all the information of the transaction 
 * Parameters:
 *  - none
 * Return:
 *  - none
 */
void Transaction::resetInstance()
{
  stopTimer();
  deleteTimer();
  setAuthorized(false);
  account = "";  
}

/*
 * setupTimer
 * Responsibility:
 *  - sets up transaction timer 
 * Parameters:
 *  - none
 * Return:
 *  - true - timer setup successful
 *  - false - error in setting up the timer
 */
bool Transaction::setupTimer()
{
    sigevent sev;
    timer_t tmp_id;


    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_notify_function = timeoutHandler;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = this;

  if (timer_create(CLOCK_REALTIME, &sev, &tmp_id) == -1){
    cout << "ERROR: Faled to create timer" << endl;
    return false;
  }
  
  //set transaction timerid
  timerid = tmp_id;

  return true;

}

/*
 * startTimer
 * Responsibility:
 *  - kick off the timer 
 * Parameters:
 *  - timeout value in seconds
 * Return:
 *  - true - start timer successfully
 *  - false - error in start timer 
 */
bool Transaction::startTimer(int timeout_value)
{
  struct itimerspec its;

  if (!timerid) {
    cout << "ERROR: Invalid timer id in startTimer" << endl;
    return false;
  }

  its.it_value.tv_sec =  timeout_value;
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0; //this is a one shot timer
  its.it_interval.tv_nsec = 0;

  if(timer_settime(timerid, 0, &its, NULL) == -1){
    cout << "Error: start timer setttime failed" << endl;
    return false;
  }

  return true;
}

/*
 * stopTimer
 * Responsibility:
 *  - stop an active running timer
 * Parameters:
 *  - none
 * Return:
 *  - true - stop timer successfully
 *  - false - error in stopping timer
 */
bool Transaction::stopTimer()
{
  struct itimerspec its;

  if (!timerid) {
    cout << "ERROR: Invalid timer id in stopTimer" << endl;
    return false;
  }

  //setting the value to zero to stop the timer
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0; 
  its.it_interval.tv_nsec = 0;

  if(timer_settime(timerid, 0, &its, NULL) == -1){
    cout << "Error: stop timer setttime failed" << endl;
    return false;
  }

  return true;
}

/*
 * deleteTimer
 * Responsibility:
 *  - to delete an existing timer
 * Parameters:
 *  - none
 * Return:
 *  - true - delete timer successfully
 *  - false - error in deleting timer
 */
bool Transaction::deleteTimer()
{
  if (!timerid) {
    cout << "ERROR: Invalid timer id in deleteTimer" << endl;
    return false;
  }

  timer_delete(timerid);
  timerid = 0;

  return true;
}

/*
 * timeoutHandler
 * Responsibility:
 *  - timer timeout handling function
 *  - delete the timer
 *  - reset info in the transaction
 * Parameters:
 *  - sig - the signal event number
 *  - si - signal info which points to suer supplied info
 *  - uc - user context 
 * Return:
 *  - none
 */

void timeoutHandler( sigval_t This )
{
  Transaction *txaction = (Transaction *) This.sival_ptr;

  if (txaction) {
    //delete timer
    txaction->deleteTimer();

    //logout the current account
    string emptyStr = "";
    txaction->setAccount(emptyStr);
    txaction->setAuthorized(false);
  }

}

