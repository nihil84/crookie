#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include "IEvent.h"
#include <memory>

namespace crookie {

class EventBus;
  
typedef std::shared_ptr<IEvent> Event;


class IEventDispatcher
{
public:
  
  virtual ~IEventDispatcher() noexcept { }
  
  //! @brief Handle given event.
  //! This base implementation simply call dispatch on the event.
  //! @see crookie::ActiveObject
  virtual void handle(const Event& event)
  {
    event->dispatch(*this);
  }
};
  
  
//! @brief EventBus handler base interface
class IEventHandler : public virtual IEventDispatcher
{
public:

  IEventHandler() { }
  
  explicit IEventHandler(EventBus& bus)
    : m_owner(&bus)
  { }

  //! Called by the owner EventBus upon deletion
  virtual void dismiss() 
  { 
    m_owner = nullptr; 
  }
  
protected:   

  //! Reference to the bus where it has subscribed for HandledEvents
  EventBus* m_owner = nullptr;
};

  

  
}

#endif
