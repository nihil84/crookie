//
//  BasicFunctionalTest.cpp
//  crookie
//
//  Created by Paolo Bernini on 21/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//


#include "BasicFunctionalTest.h"

#include <Dynamic/AEventHandler.h>
#include <DynamicBus.h>
#include <EventBase.h>

#include <iostream>
#include <string>

using namespace std;

class TestEvent : public crookie::EventBase<TestEvent>
{
public:
    
    TestEvent(const std::string& msg)
        : m_msg(msg)
    { }
    
    const std::string& message() const { return m_msg; }
    
private:
    
    std::string m_msg;
};


class TestEventHandler : public crookie::AEventHandler<TestEvent>
{
public:
    
    TestEventHandler(crookie::EventBus& bus)
        : crookie::AEventHandler<TestEvent>(bus)
        , m_gotit(false)
    { }
    
    void onEvent(const TestEvent& evt)
    {
        m_gotit = true;
        cout << "TestEvent: " << evt.message() << endl;
    }
    
    bool gotit() const { return m_gotit; }
    
private:
    
    bool m_gotit;
};



//------------------------------------------------------------------------------
BasicFunctionalTest::BasicFunctionalTest()
    : m_bus(new crookie::EventBus())
{ }

//------------------------------------------------------------------------------
bool BasicFunctionalTest::testBasicFunctionality()
{
    TestEventHandler handler(*m_bus);
    
    m_bus->dispatch(crookie::EventBus::Event(new TestEvent("## try this ##")));
    
    return handler.gotit();
}



