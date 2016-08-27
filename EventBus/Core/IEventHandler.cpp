//
//  IEventHandler.cpp
//  crookie
//
//  Created by Paolo Bernini on 27/08/16.
//
//

#include "IEventHandler.h"
#include "EventBus.h"
#include <cassert>

using namespace crookie;


//------------------------------------------------------------------------------
IEventHandler::IEventHandler(EventBus& bus, int type)
  : m_owner(&bus)
{
  bus.subscribe(type, this);
}

//- protected ------------------------------------------------------------------
EventBus* IEventHandler::subscribe(EventBus& bus, int type)
{
  EventBus* oldbus = m_owner;
  if (&bus == oldbus)
    return oldbus; // nothing to change
  
  if (oldbus != nullptr)
  {
    bool found = oldbus->unsubscribe(type, this);
    assert(found);
  }
  
  m_owner = &bus;
  m_owner->subscribe(type, this);
  return oldbus;
}

//- protected ------------------------------------------------------------------
bool IEventHandler::unsubscribe(int type)
{
  return (m_owner != nullptr && m_owner->unsubscribe(type, this));
}
