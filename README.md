# crookie #

C++11 simple yet powerful building blocks

This is more a private collection of code snippets and little toolsets than a library, but you could find some useful classes for your project.

The library is still in a highly experimental phase of its life: I apology for all the lacks in documentation and I cannot guarantee that the public interface of any of the classes of the library nor even the name of the classes and their header files will not change in the future.

See `LICENCE` file for licencing informations.

## Dependencies ##

The library has no dependencies except for the C++11 standard library. If you want to build also the test suite you will need `boost-test` headers and `libboost_unit_test_framework.a` compiled for your target.

The project is totally multi-platform and it make use of CMake (version 3.1 or above).

## Currently implemented modules ##

### MultiThreading ###

The MultiThreading package aims to provide some building blocks for highly concurrent applications. 

The `LockedPtr` template class wants to be a safe and convenient way to return a locked reference to any sort of data from a shared data container.

### EventBus ###

`EventBus` is the real core of the library and it allows to dispatch any type of events or messages to the handlers subscribed to the bus, whitout the need of senders and receivers to know each other.

It makes extensive use of templates to allow a productive use of the bus with very few lines of code, as in the following fully functional example:


```
#!c++
#include <EventBus/Static/AEventHandler.h>
#include <EventBus/EventBase.h>
#include <cassert>

using namespace crookie;

enum Id { Main = 0 };

// template instance
typedef sbus::StaticBus< Main > MainBus;

class YourEvent : public EventBase<YourEvent> 
{ /* event data and operations */ };

class Handler : public MainBus::Handler<YourEvent>
{
public:
  bool on_event = false;
  void onEvent(const TestEvent& evt)
  { 
    on_event = true;
    /* event handling */ 
  }
};

int main(/*...*/)
{
  Handler handler;

  MainBus::dispatch<YourEvent>(/*parameters to the constructor, if any*/);

  assert(handler.on_event == true);
}

```

Furthermore you can derive from `crookie::ActiveObject` or `crookie::sbus::ActiveObject` to get all your events enqueued for later handling and/or for thread decoupling as in the example below, where the class `ActiveHandler` will handle only in its private thread of execution all `TestEvent` instances dispatched on the `MainBus` (as defined in the snippet above):


```
#!c++
#include <EventBus/StaticBus/ActiveObject.h>
#include <thread>

class ActiveHandler : public MainBus::ActiveObject<TestEvent>
{
public: 
  void onEvent(const TestEvent& evt) { ++m_test; }
  
  void run() { m_thread = std::thread(&ActiveHandler::loop, this); }  
  
private:
  std::thread m_thread;
  
  void loop()
  {
    for(;;)
      dispatch();
  }
};

```

## Roadmap ##

Next goal is to write a component that will integrate into the `EventBus` system and will allow serialization of events on file and on the network, providing logging and a seamless event sharing infrastructure among processes on the same machine and on distributed machines.