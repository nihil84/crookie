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
  
private:
  
  bool m_simple = false;
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
