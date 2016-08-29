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

#include <type_traits>
#include <cassert>


namespace crookie {
  
/**
 * @brief Event base class
 *
 * Any other detail of MyEvent class (constructor parameters, public methods,
 * data members, etc...) is up to you.
 * 
 * @tparm EventClass Type of derived class from the instance of the template.
 */
template <class EventClass>
class EventBase : public IEvent
{
public:
  
  typedef EventClass DerivedEvent;
  
  typedef AEventHandler<EventClass> SpecificHandler;
  
  static const int TYPE;
  
  
  //! Constructor is here mainly for type checks.
  EventBase()
  {
    using SpecializedType = typename std::remove_pointer<decltype(this)>::type;
  
    // The following assert is here to check that the declaration of the Event
    // class is correct: class YourEvent : public EventBase<YourEvent>
    // If the assert fails you provided the wrong template argument to EventBase
    // Unfortunately the static_assert will be deceived by the corner but common
    // case of putting another class which is actually another derivation of
    // EventBase. That is the reason behind the assert in the dispatch method.
    static_assert(std::is_base_of<SpecializedType, DerivedEvent>::value,
                  "EventBase template specialization error: "
                  "EventBase<EventClass> must be the base class of EventClass");
  }

  virtual int type() const override { return TYPE; }
  
  virtual void dispatch(IEventDispatcher& handler) const override
  {
    // The following assert is here to check that the declaration of the Event
    // class is correct: class YourEvent : public EventBase<YourEvent>
    // If the assert fails you provided the wrong template argument to EventBase
    assert(dynamic_cast<const DerivedEvent*>(this) != nullptr);
    
    SpecificHandler& specific = dynamic_cast<SpecificHandler&>(handler);
    specific.onEvent(static_cast<const EventClass&>(*this));
  }
};

template <class EventClass>
const int EventBase<EventClass>::TYPE = IEvent::getTypeId();

  
} // end of namespace

#endif /* EVENTBASE_H */
