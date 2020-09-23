#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <vector>

#include "customer.hpp"

class Command
{
public:
  Command();
  ~Command();

  virtual void process_command(vector<string>& cmd);
  Customer *find_customer(string& account);
  string get_account();
  bool validate_amount(string& amount, double& amountD);

private:


};

class AuthorizeCmd : public Command
{
public:
  AuthorizeCmd();
  ~AuthorizeCmd();

  void process_command(vector<string>& cmd);

private: 

};


class WithdrawCmd : public Command
{
public:
  WithdrawCmd();
  ~WithdrawCmd();

  void process_command(vector<string>& cmd);

private:

};

class DepositCmd : public Command
{
public:
  DepositCmd();
  ~DepositCmd();

  void process_command(vector<string>& cmd);

private:

};


class BalanceCmd : public Command
{
public:
  BalanceCmd();
  ~BalanceCmd();

  void process_command(vector<string>& cmd);

private:

};

class HistoryCmd : public Command
{
public:
  HistoryCmd();
  ~HistoryCmd();

  void process_command(vector<string>& cmd);

private:

};


class LogoutCmd : public Command
{
public:
  LogoutCmd();
  ~LogoutCmd();

  void process_command(vector<string>& cmd);

private:

};


class EndCmd : public Command
{
public:
  EndCmd();
  ~EndCmd();

  void process_command(vector<string>& cmd);

private:

};

#define AUTHORIZECMD_NUM_PARAMS 3
#define WINTHDRAWCMD_NUM_PARAMS 2
#define DEPOSITCMD_NUM_PARAMS 2
#define BALANCECMD_NUM_PARAMS 1
#define HISTORYCMD_NUM_PARAMS 1
#define LOGOUTCMD_NUM_PARAMS 1
#define ENDCMD_NUM_PARAMS 1

#define COMMAND_TIMEOUT_SECONDS 120
#endif
