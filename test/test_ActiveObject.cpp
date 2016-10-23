#define BOOST_TEST_MODULE "ActiveObject test"
#include <boost/test/unit_test.hpp>

#include <EventBus/Core/EventBus.h>
#include <EventBus/ActiveObject.h>
#include <EventBus/EventBase.h>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <cassert>

using namespace std;
using namespace crookie;


struct SuiteFixture 
{
  SuiteFixture() { m_bus = new EventBus(); }
  ~SuiteFixture() { delete m_bus; }
  
  EventBus* m_bus;
};



class SimpleEvent : public EventBase<SimpleEvent>
{
public:
  SimpleEvent() {}
};

class AnEvent : public EventBase<AnEvent>
{
public:
  AnEvent(int n) : n(n) {}
  int n;
};


class ActiveHandler : public ActiveObject<SimpleEvent, AnEvent>
{
public:
  
  ActiveHandler(EventBus& bus)
    : ActiveObject<SimpleEvent, AnEvent>(bus)
  { }
  
  void runOnce()
  {
    dispatch();
  }
  
  void onEvent(const SimpleEvent& evt)
  {
    m_simple = true;
  }
  
  void onEvent(const AnEvent& evt)
  {
    if (evt.n != m_ordered)
    {
      throw std::logic_error("an event has been receieved out of order");
    }
    
    m_ordered++;
    
    if (evt.n == 1)
    {
      throw std::runtime_error("let's start with an error");
    }
  }
  
  bool receivedSimple() const { return m_simple; }
  
  int count() const { return m_ordered; }
  
private:
  
  bool m_simple = false;
  int m_ordered = 0;
  
  virtual void run()
  {
    for (;;)
    {
      try { ObjectBase::run(); break; }
      catch (std::runtime_error& ex) { } // intercept runtime_error only
    }
  }
};



BOOST_FIXTURE_TEST_SUITE( active_object, SuiteFixture )

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( basic_functional_test )
{
  ActiveHandler handler(*m_bus);
  
  m_bus->dispatch<SimpleEvent>();
  
  BOOST_CHECK( !handler.receivedSimple() );
  
  handler.runOnce();
  
  BOOST_CHECK( handler.receivedSimple() );
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_concurrent_delivery )
{
  ActiveHandler handler(*m_bus);
  
  m_bus->dispatch<SimpleEvent>();
  
  // checks handling does not happen if thread is off
  BOOST_CHECK( !handler.receivedSimple() );
  
  handler.start();
  
  std::this_thread::sleep_for(std::chrono::seconds(1));
  
  while (!handler.receivedSimple())
    ;
  
  handler.quit();
  handler.join();
  
  BOOST_CHECK( handler.receivedSimple() );
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_activeobject_resume_after_exception )
{
  ActiveHandler handler(*m_bus);
  
  try
  {
    m_bus->dispatch<SimpleEvent>();
    m_bus->dispatch<AnEvent>(0);
    m_bus->dispatch<AnEvent>(1);
    
    // dispatching will be interrupted here by the runtime_error exception
    // but it should be resumed in the same order, otherwise an unhandled
    // logic_error exception will be caught from the catch below, failing the
    // test
    
    m_bus->dispatch<AnEvent>(2);
    m_bus->dispatch<AnEvent>(3);
    m_bus->dispatch<AnEvent>(4);
    
    BOOST_CHECK(!handler.receivedSimple());
    BOOST_CHECK(handler.count() == 0);
    
    handler.start();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    while (handler.count() < 5)
      ;
    
    handler.quit();
    handler.join();

    BOOST_CHECK(handler.receivedSimple());
    BOOST_CHECK(handler.count() == 5);
  }
  catch (std::exception& ex)
  {
    // exception shall not be propagated here
    BOOST_TEST_MESSAGE(std::string("unexpected exception: ") + ex.what());
    
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
  
