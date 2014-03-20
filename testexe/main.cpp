//
//  main.cpp
//  testexe
//
//  Created by Paolo Bernini on 15/03/14.
//  Copyright (c) 2014 Paolo Bernini. All rights reserved.
//

#include <iostream>

#include "LockPolicy.h"
#include "LockedRef.h"

using namespace crookie;
using namespace std;


struct TestData
{
  int a, b;
  float c;
};

typedef LockedRef<TestData, SimpleLock> DataHandle;

DataHandle getHandle(TestData& data, SimpleLock::Mutex& mutex)
{
  return DataHandle(&data, mutex);
}


int main(int argc, const char * argv[])
{

  TestData data;
  SimpleLock::Mutex mutex;
  
  {
    LockedRef<TestData, SimpleLock> handle(&data, mutex);
    cout << "Handle created" << endl;
  }
  
  cout << "Handle destroyed" << endl;
    
  DataHandle handle2 = getHandle(data, mutex);
  
  cout << "Handle factory (with move constructor) used" << endl;
  
  LockedRef<TestData, SimpleLock> handle3(&data, mutex); // << deadlock
  
  cout << "Unreachable code here" << endl;
    
  return 0;
}

