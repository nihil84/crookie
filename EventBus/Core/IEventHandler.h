#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include "IEvent.h"
#include <memory>

namespace crookie {
  
typedef std::shared_ptr<IEvent> Event;


  
//! @brief EventBus handler base interface
class IEventHandler
{
public:

  IEventHandler() { }

  virtual ~IEventHandler() noexcept { }

  //! @brief Handle given event.
  //! This base implementation simply call dispatch on the event.
  //! @see crookie::ActiveObject
  virtual void handle(const Event& event)
  {
      event->dispatch(*this);
  }
  
  //! Called by the owner EventBus upon deletion
  virtual void dismiss() = 0;
};

  

  
}

#endif
