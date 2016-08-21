#ifndef DYNAMIC_EVENTBUS_H
#define DYNAMIC_EVENTBUS_H

#include "../Core/IEvent.h"
#include "../Core/BucketException.h"
#include "../Core/MonitoredCollection.h"

#include <memory>
#include <mutex>
#include <list>
#include <map>


namespace crookie {

  // forward declarations

  template < class EventClass > class AEventHandler;

  // code

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
  public: /*types*/

    //! @brief Convenient typename for smart pointers to Event instances.
    typedef std::shared_ptr<IEvent> Event;

    //! @brief Event handlers base interface type
    typedef IEventHandler IBusHandler;

    //! @brief Type of subscribers list
    typedef MonitoredCollection< IBusHandler* > List;


    //! @brief Association to a subscribers list.
    //! An instance of this class is returned by add(), you should keep a copy
    //! of that instance to simplify the management of the subscription
    class Account
    {
        std::shared_ptr< List > m_list;
        IBusHandler* m_handler;

    public:

        explicit Account(std::shared_ptr<List> list, IBusHandler* handler)
            : m_list(list), m_handler(handler)
        { }

        //! @brief Remove its handler from the subscriber list.
        bool unsubscribe()
        { m_list->remove(m_handler); return true; }
    };

  public: /*interface*/

    //! @brief Construct a new bus with no handlers registered
    EventBus()
        : m_firingDepth(0)
    { }

    //! Clear this bus subscriber list, locking the operation.
    ~EventBus();

    //! @brief Return current dispatching depth.
    //! Dispatching depth represents how deep we are in the EventBus::fire
    //! recursion, which happen whenever an event handler fire another
    //! event on the same bus.
    int getCurrentDepth() const { return m_firingDepth; }

    //! @brief Subscribe given handler for events of given type.
    //! If subscriber list already contains given @a handler it does nothing
    //! but can be used to obtain a new copy of the subscription Account.
    //! @param [in] handler  event handler instance of the subscriber
    //! @tparam EventClass Type of the Event to subscribe for.
    //! @return Association to subscriber list, keep a copy of the object to
    //! obtain a faster unsubscribe.
    template < class EventClass >
    Account subscribe(AEventHandler< EventClass >& handler);

    //! @brief Remove given @a handler from subscribers list (if present).
    //! @param [in] handler     pointer to handler to remove (if NULL does
    //!                         nothing.)
    //! @return True if unsubscribed successfully, false otherwise.
    bool unsubscribe(IBusHandler* handler);

    //! @brief Sends given event to all subscribed handlers.
    void fire(const Event& event);

  protected: /*types and data*/
    
    typedef std::map< int, std::shared_ptr< List > > Registry;
      

    std::recursive_mutex m_mutex;    //!< mutex semaphore

    Registry m_handlers;             //!< subscribers map

    int m_firingDepth;               //!< current firing depth
  };

  // there will be a copy of this function for EACH Event type in your project
  // but code memory is not a problem, is it?
  template < class EventClass >
  EventBus::Account EventBus::subscribe(AEventHandler< EventClass >& handler)
  {
      std::unique_lock<std::recursive_mutex> lock(m_mutex);
      Registry::iterator it = m_handlers.find(EventClass::TYPE);
    
      std::shared_ptr<List> subscribers;
      if (it == m_handlers.end()) // null pointer on insertion
      {
          subscribers.reset(new List());
          m_handlers.insert(std::make_pair(EventClass::TYPE, subscribers));
      }
      else
        subscribers = it->second;

//      if (!subscribers->contains(&handler))
          subscribers->add(&handler);

      return Account(subscribers, &handler);
  }


}

#endif // DYNAMIC_EVENTBUS_H
