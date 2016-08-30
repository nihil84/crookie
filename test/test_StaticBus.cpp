#define BOOST_TEST_MODULE "StaticBus test"
#include <boost/test/unit_test.hpp>

#include <EventBus/Static/StaticBus.h>
#include <EventBus/Static/AEventHandler.h>
#include <EventBus/Static/ActiveObject.h>
#include <EventBus/EventBase.h>

#include <thread>

using namespace crookie;
using namespace std;



enum Id { Main = 0 };

// template instance
typedef sbus::StaticBus< Main > MainBus;



class TestEvent : public EventBase<TestEvent> { };
class AnOtherEmptyEvent : public EventBase<AnOtherEmptyEvent> { };
class EventForNobody : public EventBase<EventForNobody> { };


class TestHandler : public MainBus::Handler<TestEvent>
                  , public MainBus::Handler<AnOtherEmptyEvent>
{
public:
  
  int m_test = 0;
  int m_otherEvt = 0;
  
  void onEvent(const TestEvent& evt) { ++m_test; }
  
  void onEvent(const AnOtherEmptyEvent& evt) { ++m_otherEvt; }
};

class ActiveHandler : public MainBus::ActiveObject<TestEvent, AnOtherEmptyEvent>
{
public:
  
  int m_test = 0;
  int m_otherEvt = 0;
  
  void onEvent(const TestEvent& evt) { ++m_test; }
  
  void onEvent(const AnOtherEmptyEvent& evt) { ++m_otherEvt; }
  
  void run() { m_thread = std::thread(&ActiveHandler::loop, this); }
  
  void runOnce() { dispatch(); }
  
  void stop() { m_done = true; m_thread.join(); }
  
  
private:
  
  std::thread m_thread;
  bool m_done = false;
  
  void loop()
  {
    while (!m_done)
      dispatch();
  }
};


//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( basic_functional_test )
{
  TestHandler handler;
  
  MainBus::instance().dispatch<EventForNobody>();
  MainBus::instance().dispatch<TestEvent>();
  
  BOOST_CHECK_EQUAL( handler.m_test, 1 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 0 );

  MainBus::instance().dispatch<AnOtherEmptyEvent>();
  
  BOOST_CHECK_EQUAL( handler.m_test, 1 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 1 );
  
  MainBus::instance().dispatch<TestEvent>();

  BOOST_CHECK_EQUAL( handler.m_test, 2 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 1 );
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( static_dispatching_members )
{
  TestHandler handler;
  
  MainBus::dispatch<EventForNobody>();
  MainBus::dispatch<TestEvent>();
  
  BOOST_CHECK_EQUAL( handler.m_test, 1 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 0 );
  
  MainBus::dispatch<AnOtherEmptyEvent>();
  
  BOOST_CHECK_EQUAL( handler.m_test, 1 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 1 );
  
  MainBus::dispatch<TestEvent>();
  
  BOOST_CHECK_EQUAL( handler.m_test, 2 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 1 );
}

//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( active_object_on_static_bus_threadless )
{
  ActiveHandler handler;
  
  MainBus::instance().dispatch<EventForNobody>();
  MainBus::instance().dispatch<TestEvent>();
  handler.runOnce();
  
  BOOST_CHECK_EQUAL( handler.m_test, 1 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 0 );
  
  MainBus::dispatch<TestEvent>();
  MainBus::dispatch<AnOtherEmptyEvent>();
  MainBus::dispatch<EventForNobody>();
  handler.runOnce();
  
  BOOST_CHECK_EQUAL( handler.m_test, 2 );
  BOOST_CHECK_EQUAL( handler.m_otherEvt, 1 );
}