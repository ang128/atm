#include "TestCustomer.h"

// method to test the constructor
void CustomerTestCase::testConstructor() {  
  // create a customer object
  Customer customer1("12345678", "1234", 100.00);

  // check that the object is constructed correctly 
  std::string customer_act = customer1.get_account();
  assert(customer_act == "12345678");
  std::string customer_pin = customer.get_pin();
  assert(customer_pin == "1234");
  double customer_balance = customer.get_balance();
  assert(customer_balance == 100);
}

// method to test transaction history being updated
void CustomerTestCase::testUpdateHistory() {
  // create a customer
  Customer customer1("12345678", "1234", 100.00);

  // verify that there is no transaction history
  sz = customer.gethistorySize();
  assertEquals(0, sz);

  //change the balance
  customer.set_balance(80);
  //update history
  customer.update_history(-20);
  // verify that there is one transaction history
  unsigned int sz = customer.gethistorySize();
  assertEquals(1, sz);

  //do another transaction
  //change the balance
  customer.set_balance(130);
  //update history
  customer.update_history(50);
  // verify that there is two transaction history
  sz = customer.gethistorySize();
  assertEquals(2, sz);
}

// method to create a suite of tests 
Test *CustomerTestCase::suite () {
  TestSuite *testSuite = new TestSuite ("CustomerTestCase");
  
  // add the tests
  testSuite->addTest (new TestCaller  
      ("testConstructor", &CustomerTestCase::testConstructor));
  testSuite->addTest (new TestCaller  
      ("testUpdateHistory", &CustomerTestCase::testUpdateHistory));
  return testSuite;
}

// the main method 
int main (int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: tester name_of_class_being_test" << std::endl;
    exit(1);
  }

  TestRunner runner;
  runner.addTest(argv[1], CustomerTestCase::suite());
  runner.run(argc, argv);
  
  return 0;
}
