#ifndef TestCommand_h
#define TestCommand_h

#include <iostream>
#include <string>


#include "TestCase.h"
#include "TestSuite.h"
#include "TestCaller.h"
#include "TestRunner.h"

#include "command.hpp"
#include "customer.hpp"
#include "machine.hpp"
#include "transaction.hpp"

class CommandTestCase : public TestCase { 
public:
  // constructor 
 CommandTestCase() : TestCase() {}

  // method to test find customer
  void testFindCustomer();

  // method to test amount validation
  void testAmountValidation();

  // method to create a suite of tests
  static Test *suite ();
};
#endif
