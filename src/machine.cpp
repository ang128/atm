
#include <iostream>
#include <string>
#include <list>

#include "customer.hpp"
#include "command.hpp"
#include "transaction.hpp"
#include "machine.hpp"

using namespace std;

/*
 * machine constructor
 * Responsibility:
 *  - initializes the balance on the machine
 * Parameters:
 *  - balance amount
 * Return:
 *  - none
 */
Machine::Machine(double balance) 
{
  _balance = balance;
}

/*
 * machine destructor
 * Responsibility:
 *  - none
 * Parameters:
 *  - none
 * Return:
 *  - none
 */
Machine::~Machine()
{}


/*
 * get_balance
 * Responsibility:
 *  - to retrieve the balance on the machine
 * Parameters:
 *  - none
 * Return:
 *  - balance amount
 */
double Machine::get_balance()
{
  return _balance;
}


/*
 * set_balance
 * Responsibility:
 *  - to set the balance amount on the machine
 * Parameters:
 *  - amount
 * Return:
 *  - none
 */
void  Machine::set_balance(double amount)
{
  _balance = amount;
}
