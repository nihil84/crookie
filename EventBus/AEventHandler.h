#ifndef DYNAMIC_AEVENTHANDLER_HPP
#define DYNAMIC_AEVENTHANDLER_HPP

#include "Core/IEventHandler.h"
#include "Core/EventBus.h"

#include <cassert>


namespace crookie {

  
/**
 * @brief Event handler base class
 * @tparam EventType    Type of the Event handled by this class.
 */
template <class EventType>
class AEventHandler : public IEventHandler
{
public:
  
  typedef EventType HandledEvent;
  

  AEventHandler() {}
  
  //! @brief Subscribes this handler to given bus.
  //! @param [in] bus   Bus on which subscribe for \a EventType events.
  explicit AEventHandler(EventBus& bus)
      : IEventHandler(bus, HandledEvent::TYPE)
  { }

  //! @brief Unsubscribe from EventBus.
  ~AEventHandler() noexcept { unsubscribe(); }

  //! @brief Return the type of handled events
  virtual int type() const { return HandledEvent::TYPE; }

  //! @brief Handler routine.
  virtual void onEvent(const HandledEvent& event) = 0;
  
protected:
  
  EventBus* subscribe(EventBus& bus)
  { return IEventHandler::subscribe(bus, HandledEvent::TYPE); }

  bool unsubscribe()
  { return IEventHandler::unsubscribe(HandledEvent::TYPE); }
};

  
} // end of namespace 

#endif // DYNAMIC_AEVENTHANDLER_HPP
