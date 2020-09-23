#include "TestCommand.h"

extern vector<Customer *> customerVec;

// method to test find customer
void CommandTestCase::testFindCustomer() {  
  // create a customer object
  Customer customer1("12345678", "1234", 100.00);
  //push into the customer vector
  customerVec.push_back(&customer1);

  Command cmd();
  string account("12345678");
  Customer *cust = cmd.find_customer(account);
  assert(cust);

}

// method to test amount validation
void CommandTestCase::testAmountValidation() {
  // create a command
  Command cmd();
  //create amount
  string amount("100.00");
  double amountD=0;

  bool result = cmd.validate_amount(amount, amountD);
  assert(result);

  amount = "100dollar";
  result = cmd.validate_amount(amount, amountD);
  assert(!result);

  amount = "$100";
  result = cmd.validate_amount(amount, amountD);
  assert(result);

}

// method to create a suite of tests - 
Test *CommandTestCase::suite () {
  TestSuite *testSuite = new TestSuite ("CommandTestCase");
  
  // add the tests
  testSuite->addTest (new TestCaller  
      ("testFindCustomer", &CommandTestCase::testFindCustomer));
  testSuite->addTest (new TestCaller  
      ("testAmountValidation", &CommandTestCase::testAmountValidation));
  return testSuite;
}

// the main method 
int main (int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: tester name_of_class_being_test" << std::endl;
    exit(1);
  }

  TestRunner runner;
  runner.addTest(argv[1], CommandTestCase::suite());
  runner.run(argc, argv);
  
  return 0;
}
