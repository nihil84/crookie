#define BOOST_TEST_MODULE "EventBus test"
#include <boost/test/unit_test.hpp>

#include <EventBus/Core/EventBus.h>
#include <EventBus/AEventHandler.h>
#include <EventBus/EventBase.h>
#include <EventBus/HandlerFunctor.h>

#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace crookie;

struct SuiteFixture 
{
  SuiteFixture() { g_bus = new EventBus(); }
  ~SuiteFixture() { delete g_bus; }
  
  EventBus* g_bus;
};

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
public:
  int m_a;
  double m_b;
  
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
  
  EventBus* subscribe(EventBus& bus)
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

bool aGoodOldGlobal = false;
void aStaticFunctionHandler(const AnotherEvent& evt)
{
  aGoodOldGlobal = true;
}

class VerySimpleClass
{
public:
  
  HandlerFunctor<AnotherEvent> handler;
  bool good = false;

  void eventHandler(const AnotherEvent& evt)
  {
    good = true;
  }
  
  explicit VerySimpleClass(EventBus& bus)
    : handler(bus, this, &VerySimpleClass::eventHandler)
  { }
};


BOOST_FIXTURE_TEST_SUITE( event_bus, SuiteFixture )

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( basic_functional_test )
{
  TestEventHandler handler(*g_bus);
  
  g_bus->dispatch(Event(new TestEvent("## testBasicFunctionality ##")));
  
  BOOST_REQUIRE(handler.gotit());
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( check_correctness_on_reversed_destruction_sequence )
{
  EventBus* tempBus = new EventBus();
  TestEventHandler handler(*tempBus);
  
  BOOST_REQUIRE(handler.AEventHandler<TestEvent>::subscribed());
  
  tempBus->dispatch(Event(new TestEvent("## testBusDeletionCorrectness ##")));
  BOOST_REQUIRE(handler.gotit());
  
  delete tempBus;
  
  BOOST_REQUIRE(!handler.AEventHandler<TestEvent>::subscribed());
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_multiple_delivery )
{
  TestEventHandler handler1(*g_bus);
  TestEventHandler handler2(*g_bus);
  TestEventHandler handler3(*g_bus);
  
  // using variadic template dispatch function (this is functionally equivalent
  // to the dispatch function above
  g_bus->dispatch<TestEvent>("## testMultipleDelivery ##");
  
  BOOST_CHECK(handler1.gotit() && handler2.gotit() && handler3.gotit());
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_two_events_in_a_row )
{
  TestEventHandler handler(*g_bus);

  g_bus->dispatch(Event(new TestEvent("## testTwoEventsInARow ##")));
  BOOST_CHECK(handler.gotit() && !handler.another());

  g_bus->dispatch<AnotherEvent>(1, 5.5);
  BOOST_CHECK(handler.gotit() && handler.another());
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_bus_subscriptions )
{
  EventBus secondBus;
  
  TestEventHandler handler(*g_bus);
  
  g_bus->dispatch<TestEvent>("## testBusSubscriptions - tier 1 ##");
  secondBus.dispatch<AnotherEvent>(2, 11.);
  
  BOOST_CHECK(handler.gotit() && !handler.another());
  
  handler.subscribe(secondBus);
  handler.reset();
  
  g_bus->dispatch<TestEvent>("## testBusSubscriptions - tier 2 ##");
  secondBus.dispatch<AnotherEvent>(3, 16.5);
  
  BOOST_CHECK(!handler.gotit() && handler.another());  
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_handler_functors )
{
  TestEventHandler handler(*g_bus);
  VerySimpleClass object(*g_bus);
  HandlerFunctor<AnotherEvent> functor;
  
  functor = HandlerFunctor<AnotherEvent>(*g_bus, aStaticFunctionHandler);
  
  g_bus->dispatch<AnotherEvent>(4, 22.8);
  
  BOOST_CHECK(handler.another() && object.good && aGoodOldGlobal);
}


BOOST_AUTO_TEST_SUITE_END()


