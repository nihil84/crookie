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
  

  AEventHandler()
    : IEventHandler(HandledEvent::TYPE)
  { }
  
  //! @brief Subscribes this handler to given bus.
  //! @param [in] bus   Bus on which subscribe for \a EventType events.
  explicit AEventHandler(EventBus& bus)
      : IEventHandler(bus, HandledEvent::TYPE)
  { }

  //! @brief Unsubscribe from EventBus.
  ~AEventHandler() noexcept { unsubscribe(); }

  //! @brief Handler routine.
  virtual void onEvent(const HandledEvent& event) = 0;
};

  
} // end of namespace 

#endif // DYNAMIC_AEVENTHANDLER_HPP
