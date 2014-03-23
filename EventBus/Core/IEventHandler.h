#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include "../Dynamic/EventBus.h"


namespace crookie {

  //! @brief EventBus handler base interface
  class IEventHandler
  {
  public:

    typedef EventBus::Event Event;

    IEventHandler() { }

    virtual ~IEventHandler() { }

    virtual int type() const = 0;

    //! @brief Handle given event.
    //! This base implementation simply call dispatch on the event.
    //! @see DBus::ActiveObject
    virtual void handle(const Event& event)
    {
        event->dispatch(*this);
    }
  };

  
}

#endif
