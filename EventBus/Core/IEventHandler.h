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

  IEventHandler(int type) : m_type(type) { }
  
  explicit IEventHandler(EventBus& bus, int type);

  
  //! @brief Return the type of handled events
  virtual int type() const { return m_type; }

  //! Called by the owner EventBus upon deletion
  virtual void dismiss() 
  { 
    m_owner = nullptr; 
  }
  
  bool subscribed() const { return m_owner != nullptr; }
  
protected:   
  
  //! @brief Subscribe this handler on given bus.
  //! If it was subscribed on another bus, the old subscription is removed.
  //! @param [in] bus   A reference to the bus where to perform the subscription
  //! @return A reference to the bus the handler was subscribed before or
  //! nullptr if none.
  EventBus* subscribe(EventBus& bus);
  
  //! @brief Early unsubscribe from EventBus.
  //! @return True if this handler was subscribed for the specified event, false
  //! otherwise.
  bool unsubscribe();
  
  //! @brief Allow derived classes full access to the current EventBus instance.
  //! @return A Reference to current EventBus instance or nullptr if not
  //! subscribed yet/anymore.
  EventBus* owner() const { return m_owner; }
  
private:

  //! Reference to the bus where it has subscribed for HandledEvents
  const int m_type;
  EventBus* m_owner = nullptr;
};


} // end of namespace

#endif
