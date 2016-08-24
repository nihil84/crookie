#ifndef DYNAMIC_AEVENTHANDLER_HPP
#define DYNAMIC_AEVENTHANDLER_HPP

#include "Core/IEventHandler.h"
#include "EventBus.h"


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
      : m_owner(&bus)
  { bus.subscribe(HandledEvent::TYPE, this); }

  //! @brief Unsubscribe from EventBus.
  ~AEventHandler() noexcept { unsubscribe(); }

  //! @brief Return the type of handled events
  virtual int type() const { return HandledEvent::TYPE; }

  virtual void dismiss() { m_owner = nullptr; }

  //! @brief Handler routine.
  virtual void onEvent(const HandledEvent& event) = 0;

protected:
  
  //! Reference to the bus where it has subscribed for HandledEvents
  EventBus* m_owner = nullptr;
  
  //! @brief Subscribe this handler on given bus.
  //! If it was subscribed on another bus, the old subscription is removed.
  //! @param [in] bus   A reference to the bus where to perform the subscription
  //! @return A reference to the bus the handler was subscribed before or
  //! nullptr if none.
  EventBus* subscribe(EventBus& bus)
  {
    EventBus* oldbus = m_owner;
    if (&bus == oldbus)
      return oldbus; // nothing to change
    
    if (oldbus != nullptr)
      oldbus->unsubscribe(HandledEvent::TYPE, this);
    bus.subscribe(HandledEvent::TYPE, this);
    return oldbus;
  }
  
  //! @brief Early unsubscribe from EventBus.
  bool unsubscribe()
  {
    return (m_owner != nullptr
            && m_owner->unsubscribe(HandledEvent::TYPE, this));
  }
};

  
} // end of namespace 

#endif // DYNAMIC_AEVENTHANDLER_HPP
