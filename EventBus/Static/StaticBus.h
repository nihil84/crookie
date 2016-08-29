//  StaticBus.h
//
//  Created by Paolo Bernini on 02/01/13.
//  Copyright (c) 2013 IDS. All rights reserved.
//


#ifndef STATIC_EVENTBUS_H
#define STATIC_EVENTBUS_H

#include "../Core/EventBus.h"


namespace crookie {
  
namespace sbus {
  

// StaticBus EventHandlers forward declarations
  
template < unsigned buscode, class EventType >
  class AEventHandler;
  
template < unsigned int buscode, class... EventTypes >
  class ActiveObject;
  
// code

template < unsigned code >
class StaticBus : public EventBus
{
public:

  template <class EventType>
  using Handler = AEventHandler< code, EventType >;
  
  template <class... EventTypes>
  using ActiveObject = crookie::sbus::ActiveObject< code, EventTypes...>;
  
  static const unsigned CODE = code;
  
  
  static StaticBus< code >& instance();

  //! @brief Sends given event to all subscribed handlers.
  static void dispatch(const Event& event)
  { StaticBus< CODE >::instance().EventBus::dispatch(event); }
  
  template <typename EventClass, typename ...Args>
  static void dispatch(Args&&... params)
  {
    dispatch(Event(new EventClass(std::forward<Args>(params)...)));
  }


private:

  StaticBus() { }
  
  StaticBus(const StaticBus&) = delete;
  StaticBus operator =(const StaticBus&) = delete;
};


//- method definitions ---------------------------------------------------------

template < unsigned int code >
StaticBus< code >& StaticBus< code >::instance()
{
  static StaticBus<code> _bus;
  return _bus;
}

}} // end of namespace

#endif
