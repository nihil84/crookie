#ifndef DYNAMIC_EVENTBUS_H
#define DYNAMIC_EVENTBUS_H

#include "Core/IEvent.h"
#include "Core/BucketException.h"
#include "Core/MonitoredCollection.h"

#include <memory>
#include <mutex>
#include <list>
#include <unordered_map>


namespace crookie {

/**
 * @brief Message bus business core.
 *
 * You should have not to bother with any of its methods except for fire(),
 * which allow you to send events to all the handlers of your event subscribed
 * to your bus instance.
 *
 * @warning EventBus instance HAS TO BE fully initialized BEFORE any Handler,
 * thus you cannot instance it as a member of Handler classes.
 */
class EventBus
{
public:

  //! @brief Convenient typename for smart pointers to Event instances.
  typedef std::shared_ptr<IEvent> Event;

  //! @brief Type of subscribers list
  typedef MonitoredCollection< IEventHandler* > List;


  //! @brief Construct a new bus with no handlers registered
  EventBus() { }
  
  //! Clear this bus subscriber list, locking the operation.
  ~EventBus();

  //! @brief Return current dispatching depth.
  //! Dispatching depth represents how deep we are in the EventBus::fire
  //! recursion, which happen whenever an event handler fire another
  //! event on the same bus.
  int getCurrentDepth() const { return m_firingDepth; }

  //! @brief Subscribe given handler for events of given type.
  //! Multiple subscription of the same handler are allowed and it will result
  //! in delivering the same message as many times as the subscriptions.
  //! @param [in] type      event type (integer returned by Event::type());
  //! @param [in] handler   event handler instance of the subscriber
  void subscribe(int type, IEventHandler* handler);
  
  //! @brief Remove given @p handler from subscribers list (if present).
  //! @param [in] type      event type (integer returned by Event::type());
  //! @param [in] handler     pointer to handler to remove (if nullptr does
  //!                         nothing.)
  //! @return True if unsubscribed successfully, false otherwise.
  bool unsubscribe(int type, IEventHandler* handler);

  //! @brief Sends given event to all subscribed handlers.
  void dispatch(const Event& event);
  
  template <typename EventClass, typename ...Args>
  void dispatch(Args&&... params)
  {
    dispatch(Event(new EventClass(std::forward<Args>(params)...)));
  }

protected: /*types and data*/
  
  typedef std::unordered_map< int, std::shared_ptr< List > > Registry;
    

  std::recursive_mutex m_mutex;    //!< mutex semaphore

  Registry m_handlers;             //!< subscribers map

  int m_firingDepth = 0;           //!< current firing depth
  
private:
  
  EventBus(const EventBus&) = delete;
  EventBus& operator =(const EventBus&) = delete;
};


} // end of namespace

#endif // DYNAMIC_EVENTBUS_H
