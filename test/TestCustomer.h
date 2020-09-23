#ifndef TestCustomer_h
#define TestCustomer_h

#include <iostream>
#include <string>


#include "TestCase.h"
#include "TestSuite.h"
#include "TestCaller.h"
#include "TestRunner.h"

#include "customer.hpp"

class CustomerTestCase : public TestCase { 
public:
  // constructor 
 CustomerTestCase(unsigned long int act, unsigned int pin, double balance ) : TestCase(act) {}

  // method to test the constructor
  void testConstructor();

  // method to test transaction history
  void testUpdateHistory();

  // method to create a suite of tests
  static Test *suite ();
};
#endif
