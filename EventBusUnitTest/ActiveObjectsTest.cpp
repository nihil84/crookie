//
//  ActiveObjectsTest.cpp
//  crookie
//
//  Created by Paolo Bernini on 23/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//

#include "ActiveObjectsTest.h"

#include <ActiveObject.h>
#include <EventBus.h>
#include <EventBase.h>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cassert>

using namespace std;
using namespace crookie;


class SimpleEvent : public EventBase<SimpleEvent>
{
public:
  SimpleEvent() {}
};


class ActiveHandler : public ActiveObject<SimpleEvent>
{
public:
  
  ActiveHandler(EventBus& bus) : ActiveObject<SimpleEvent>(bus) {}
  
  void runOnce()
  {
    dispatch();
  }
  
  void onEvent(const SimpleEvent& evt)
  {
    m_simple = true;
  }
  
  bool receivedSimple() const { return m_simple; }
  
  void stop()
  {
    m_done = true;
    m_thread.join();
  }
  
private:
  
  std::thread m_thread;
  bool m_simple = false;
  bool m_done = false;
  
  void loop()
  {
    while (!m_done)
      dispatch();
  }
};



//------------------------------------------------------------------------------
ActiveObjectsTest::ActiveObjectsTest()
: m_bus(new EventBus())
{ }

//------------------------------------------------------------------------------
bool ActiveObjectsTest::testBasicFunctionality()
{
  ActiveHandler handler(*m_bus);
  
  m_bus->dispatch<SimpleEvent>();
  
  bool firstOk = !handler.receivedSimple();
  
  handler.runOnce();
  
  return firstOk && handler.receivedSimple();
}

//------------------------------------------------------------------------------
bool ActiveObjectsTest::testConcurrentDelivery()
{
  ActiveHandler handler(*m_bus);
  
  m_bus->dispatch<SimpleEvent>();
  
  bool firstOk = !handler.receivedSimple();
  
  handler.run();
  
  std::this_thread::sleep_for(std::chrono::seconds(1));
  
  while (!handler.receivedSimple())
    ;
  
  handler.stop();
  
  return (firstOk && handler.receivedSimple());
}
