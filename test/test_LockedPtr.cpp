#define BOOST_TEST_MODULE "LockedPtr test"
#include "boost-test/unit_test.hpp"

#include <MultiThreading/SimpleLock.h>
#include <MultiThreading/LockedPtr.h>

using namespace crookie;
using namespace std;


struct TestData
{
  int a, b;
  float c;
};

typedef LockedPtr<TestData, LockPolicies::SimpleLock> DataHandle;

DataHandle getHandle(TestData& data, LockPolicies::SimpleLock::Mutex& mutex)
{
  return DataHandle(&data, mutex);
}


BOOST_AUTO_TEST_CASE( basic_functional_test )
{
  TestData data;
  LockPolicies::SimpleLock::Mutex mutex;
  
  {
    LockedPtr<TestData, LockPolicies::SimpleLock> handle(&data, mutex);
  }

  BOOST_CHECK( true ); 
  
  // TODO write a good test for the deadlock below
//  DataHandle handle2 = getHandle(data, mutex);

//  LockedPtr<TestData, LockPolicies::SimpleLock> handle3(&data, mutex); // << deadlock
}

