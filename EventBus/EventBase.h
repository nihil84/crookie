//
//  EventBase.h
//  crookie
//
//  Created by Paolo Bernini on 22/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//

#ifndef EVENTBASE_H
#define EVENTBASE_H

#include "Core/IEvent.h"
#include "AEventHandler.h"

namespace crookie {
  
/**
 * @brief Event base class
 *
 * Any other detail of MyEvent class (constructor parameters, public methods,
 * data members, etc...) is up to you.
 */
template <class EventClass>
class EventBase : public IEvent
{
public:
  
  typedef AEventHandler<EventClass> SpecificHandler;
  
  static const int TYPE;
  
  virtual int type() const { return TYPE; }
  
  virtual void dispatch(IEventHandler& handler) const
  {
    SpecificHandler& specific = static_cast<SpecificHandler&>(handler);
    specific.onEvent(static_cast<const EventClass&>(*this));
  }
};


template <class EventClass>
const int EventBase<EventClass>::TYPE = IEvent::getTypeId();

} // end of namespace

#endif /* EVENTBASE_H */
