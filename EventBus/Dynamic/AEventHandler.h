#ifndef DYNAMIC_AEVENTHANDLER_HPP
#define DYNAMIC_AEVENTHANDLER_HPP

#include "../Core/IEventHandler.h"
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

  //! @brief Subscribes this handler to given bus.
  //! @param [in] bus   Bus on which subscribe for \a EventType events.
  explicit AEventHandler(EventBus& bus)
      : IEventHandler(bus)
  { bus.subscribe(HandledEvent::TYPE, this); }

  //! @brief Unsubscribe from EventBus.
  ~AEventHandler() { unsubscribe(); }

  //! @brief Return the type of handled events
  virtual int type() const { return HandledEvent::TYPE; }

  //! @brief Handler routine.
  virtual void onEvent(const HandledEvent& event) = 0;

protected:

  //! @brief Early unsubscribe from EventBus.
  //! @warning NO re-subscribe is allowed.
  bool unsubscribe()
  {
    return (m_owner != nullptr && m_owner->unsubscribe(this));
  }
};

  
} // end of namespace 

#endif // DYNAMIC_AEVENTHANDLER_HPP
