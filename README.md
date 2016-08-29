# crookie #

C++11 simple yet powerful building blocks

This is more a private collection of code snippets and little toolsets than a library, but you could find some useful classes for your project.

## Dependencies ##

The library has no dependencies except for the C++11 standard library. If you want to build also the test suite you will need `boost-test` headers and `libboost_unit_test_framework.a` compiled for your target.

The project is totally multi-platform and it make use of CMake (version 3.1 or above).

## Currently implemented modules ##

### MultiThreading ###

The MultiThreading package aims to provide some building blocks for highly concurrent applications. 

The `LockedPtr` template class wants to be a safe and convenient way to return a locked reference to any sort of data from a shared data container.

### EventBus ###

`EventBus` is the actual core of the library and it allows to dispatch any type of events or messages to the handlers subscribed to the bus, whitout the need of senders and receivers to know each other.

It makes extensive use of templates to allow a productive use of the bus with very few lines of code, as in the following example:


```
#!c++
#include <EventBus/Static/AEventHandler.h>
#include <EventBus/EventBase.h>

enum Id { Main = 0 };

// template instance
typedef sbus::StaticBus< Main > MainBus;

class YourEvent : public EventBase<YourEvent> 
{ /* event data and operations */ };

class Handler : public MainBus::Handler<YourEvent>
{
public:
  
  void onEvent(const TestEvent& evt) { /* event handling */ }

};

int main(/*...*/)
{
  Handler handler;



}

```