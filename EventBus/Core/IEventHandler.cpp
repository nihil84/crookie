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
  : m_type(type), m_owner(&bus)
{
  bus.subscribe(type, this);
}

//- protected ------------------------------------------------------------------
EventBus* IEventHandler::subscribe(EventBus& bus)
{
  EventBus* oldbus = m_owner;
  if (&bus == oldbus)
    return oldbus; // nothing to change
  
  if (oldbus != nullptr)
  {
    bool found = oldbus->unsubscribe(m_type, this);
    assert(found);
  }
  
  m_owner = &bus;
  m_owner->subscribe(m_type, this);
  return oldbus;
}

//- protected ------------------------------------------------------------------
bool IEventHandler::unsubscribe()
{
  return (m_owner != nullptr && m_owner->unsubscribe(m_type, this));
}
