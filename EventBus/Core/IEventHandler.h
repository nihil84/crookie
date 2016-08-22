#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include "../Dynamic/EventBus.h"


namespace crookie {
  
  
//! @brief EventBus handler base interface
class IEventHandler
{
public:

  typedef EventBus::Event Event;

  IEventHandler(EventBus& bus)
    : m_owner(&bus)
  { }

  virtual ~IEventHandler() { }

  virtual int type() const = 0;

  //! @brief Handle given event.
  //! This base implementation simply call dispatch on the event.
  //! @see DBus::ActiveObject
  virtual void handle(const Event& event)
  {
      event->dispatch(*this);
  }
  
  //! Called by the owner EventBus upon deletion
  virtual void dismiss() { m_owner = nullptr; }
  
protected:
  
  EventBus* m_owner;
};

  
}

#endif
