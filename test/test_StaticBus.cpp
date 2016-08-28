#define BOOST_TEST_MODULE "StaticBus test"
#include <boost/test/unit_test.hpp>

#include <EventBus/Static/StaticBus.h>
#include <EventBus/Static/AEventHandler.h>
#include <EventBus/EventBase.h>

using namespace crookie;
using namespace std;



enum Id { Main = 0 };

// template instance
typedef sbus::StaticBus< Main > MainBus;



class TestEvent : public EventBase<TestEvent> { };
class AnOtherEmptyEvent : public EventBase<AnOtherEmptyEvent> { };


class TestHandler : public MainBus::Handler<TestEvent>
                  , public MainBus::Handler<AnOtherEmptyEvent>
{
  int m_test = 0;
  int m_otherEvt = 0;
  
public:
  
  void onEvent(const TestEvent& evt)
  {
    ++m_test;
  }
  
  void onEvent(const AnOtherEmptyEvent& evt)
  {
    ++m_otherEvt;
  }
  
  int test() const { return m_test; }
  
  int secondTest() const { return m_otherEvt; }
};


//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( basic_functional_test )
{
  TestHandler handler;
  
  MainBus::instance().dispatch<TestEvent>();
  
  BOOST_CHECK( handler.test() == 1 && handler.secondTest() == 0 );

  MainBus::instance().dispatch<AnOtherEmptyEvent>();
  
  BOOST_CHECK( handler.test() == 1 && handler.secondTest() == 1 );
  
  MainBus::instance().dispatch<TestEvent>();

  BOOST_CHECK( handler.test() == 2 && handler.secondTest() == 1 );
}

