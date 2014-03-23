#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include "../Dynamic/EventBus.hpp"


namespace GEB {
  
namespace Core {

  //! @brief EventBus handler base interface
  class IEventHandler
  {
  public:

    typedef GEB::EventBus::Event Event;

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

  
}} // end of namespaces Core and GEB

#endif // IHANDLER_H
