//
//  BasicFunctionalTest.cpp
//  crookie
//
//  Created by Paolo Bernini on 21/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//


#include "BasicFunctionalTest.h"

#include <AEventHandler.h>
#include <EventBus.h>
#include <EventBase.h>

#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace crookie;

class TestEvent : public EventBase<TestEvent>
{
  std::string m_msg;

public:

  TestEvent(const std::string& msg)
      : m_msg(msg)
  { }
  
  const std::string& message() const { return m_msg; }
};

class AnotherEvent : public EventBase<AnotherEvent>
{
  int m_a;
  double m_b;
public:
  
  AnotherEvent(int a, double b)
    : m_a(a), m_b(b)
  {}
};


class TestEventHandler : public AEventHandler<TestEvent>
                       , public AEventHandler<AnotherEvent>
{
public:
  
  TestEventHandler(crookie::EventBus& bus)
    : AEventHandler<TestEvent>(bus)
    , AEventHandler<AnotherEvent>(bus)
  { }
  
  void onEvent(const TestEvent& evt) override
  {
    m_gotit = true;
//    cout << "TestEvent: " << evt.message() << endl;
  }
  
  void onEvent(const AnotherEvent& evt) override
  {
    m_another = true;
  }
  
  bool gotit() const { return m_gotit; }
  
  bool another() const { return m_another; }
  
  void reset()
  {
    m_gotit = false;
    m_another = false;
  }
  
  virtual EventBus* subscribe(EventBus& bus) override
  {
    EventBus* oldbus1 = AEventHandler<TestEvent>::subscribe(bus);
    EventBus* oldbus2 = AEventHandler<AnotherEvent>::subscribe(bus);
    assert(oldbus1 == oldbus2);
    return oldbus1;
  }
  
private:
    
  bool m_gotit = false;
  bool m_another = false;
};



//------------------------------------------------------------------------------
BasicFunctionalTest::BasicFunctionalTest()
  : m_bus(new EventBus())
{ }

//------------------------------------------------------------------------------
bool BasicFunctionalTest::testBasicFunctionality()
{
  TestEventHandler handler(*m_bus);
  
  m_bus->dispatch(EventBus::Event(new TestEvent("## testBasicFunctionality ##")));
  
  return handler.gotit();
}

//------------------------------------------------------------------------------
bool BasicFunctionalTest::testMultipleDelivery()
{
  TestEventHandler handler1(*m_bus);
  TestEventHandler handler2(*m_bus);
  TestEventHandler handler3(*m_bus);
  
  // using variadic template dispatch function (this is functionally equivalent
  // to the dispatch function above
  m_bus->dispatch<TestEvent>("## testMultipleDelivery ##");
  
  return (handler1.gotit() && handler2.gotit() && handler3.gotit());
}

//------------------------------------------------------------------------------
bool BasicFunctionalTest::testTwoEventsInARow()
{
  TestEventHandler handler(*m_bus);

  m_bus->dispatch(EventBus::Event(new TestEvent("## testTwoEventsInARow ##")));
  bool firstok = handler.gotit() && !handler.another();

  m_bus->dispatch<AnotherEvent>(1, 5.5);
  return (firstok && handler.gotit() && handler.another());
}

bool BasicFunctionalTest::testBusSubscriptions()
{
  EventBus secondBus;
  
  TestEventHandler handler(*m_bus);
  
  m_bus->dispatch<TestEvent>("## testBusSubscriptions - tier 1 ##");
  secondBus.dispatch<AnotherEvent>(2, 11.);
  
  bool firstOk = handler.gotit() && !handler.another();
  
  handler.subscribe(secondBus);
  handler.reset();
  
  m_bus->dispatch<TestEvent>("## testBusSubscriptions - tier 2 ##");
  secondBus.dispatch<AnotherEvent>(3, 16.5);
  
  bool secondOk = !handler.gotit() && handler.another();
  
  return (firstOk && secondOk);
}


