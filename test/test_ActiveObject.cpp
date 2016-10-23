#define BOOST_TEST_MODULE "ActiveObject test"
#include <boost/test/unit_test.hpp>

#include <EventBus/Core/EventBus.h>
#include <EventBus/ActiveObject.h>
#include <EventBus/EventBase.h>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
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
  AnEvent() {}
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
    
  }
  
  bool receivedSimple() const { return m_simple; }
  
private:
  
  bool m_simple = false;
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
  
  BOOST_CHECK( !handler.receivedSimple() );
  
  handler.start();
  
  std::this_thread::sleep_for(std::chrono::seconds(1));
  
  while (!handler.receivedSimple())
    ;
  
  handler.quit();
  
  BOOST_CHECK( handler.receivedSimple() );
}

BOOST_AUTO_TEST_SUITE_END()
  
