

#ifndef MACHINE_H
#define MACHINE_H

#define MACHINE_INITIAL_BALANCE 10000
#define WITHDRAWAL_MULTIPLIER 20
#define OVERDRAWN_FEE 5

class Machine
{
public:
  Machine(double balance);
  ~Machine();

  double get_balance();
  void set_balance(double amount);

private:
  double _balance;

};

enum Commands {
  Authorize,
  Withdraw,
  Deposit,
  Balance,
  History,
  Logout,
  End,
  Invalid
};

#endif
