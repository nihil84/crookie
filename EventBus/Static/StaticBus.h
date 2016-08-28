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
  

// StaticBus EventHandler forward declaration
template < unsigned buscode, class EventType >
  class AEventHandler;
  
// code

template < unsigned code >
class StaticBus : public EventBus
{
public /*types*/:

  template <class EventType>
  using Handler = AEventHandler< code, EventType >;
  
  static const unsigned CODE = code;


public /*interface*/:

  static StaticBus< code >& instance();
  

private /*types and data*/:

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
