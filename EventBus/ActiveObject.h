#ifndef DYNAMIC_ACTIVEOBJECT_H
#define DYNAMIC_ACTIVEOBJECT_H

#include "AEventHandler.h"

#include <list>
#include <mutex>
#include <thread>


namespace crookie {
  
/**
 * @brief This class is a basic implementation of the active object design 
 * pattern that easily matches most use cases of the pattern.
 *
 * From wikipedia: The active object design pattern decouples method execution 
 * from method invocation for objects that each reside in their own thread of 
 * control. The goal is to introduce concurrency, by using asynchronous method 
 * invocation and a scheduler for handling requests.
 *
 * This class implements a scheduler running in its own thread of execution,
 * while the asynchronous methods are the event handlers implemented in the
 * derived class and distiguished each others thanks to the type of the event 
 * argument.
 */
template <class... EventTypes>
class ActiveObject : public AEventHandler<EventTypes>...
{
public:
  
  //! Convenient alias for derived classes
  typedef ActiveObject<EventTypes...> ObjectBase;
  
  //! Type of the queue of events (internal use only)
  typedef std::list< Event > EventsQueue;
  
  //! Constructor
  //! @param [in] bus   The bus on which subscribe for handled events
  explicit ActiveObject(EventBus& bus)
    : AEventHandler<EventTypes>(bus)...
    , m_done(false)
  { }

  //! Desctructor
  ~ActiveObject() noexcept
  {
    this->quit();
    this->join();
  }
  
  //! @brief Handler function, needed for EventBus integration: do not override.
  virtual void handle(const Event& event) override
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push_back(event);
  }
  
  //! @brief Starts the event dispatching.
  virtual void start()
  {
    if (m_thread.joinable() && m_thread.get_id() != std::this_thread::get_id())
      return;
    
    m_done = false;
    m_thread = std::thread(&ActiveObject::run, this);
  }
  
  //! @brief Stops events dispatching.
  void quit()
  {
    m_done = true;
  }
  
  //! @brief Waits for dispatching thread termination.
  void join()
  {
    if (m_thread.joinable())
      m_thread.join();
  }

protected:
  
  //! @brief Return true when the quit() has been called.
  //! @details Useful for derived classes to know if thread termination has
  //! been requested
  bool done() { return m_done; }
  
  //! @brief Thread main loop, dispatching events.
  virtual void run()
  {
    while (!done())
    {
      this->dispatch();
    }
  }
  
  //! @brief Extracts events from the queue and dispatch them, one at time.
  void dispatch()
  {
    EventsQueue events;
    {
      std::unique_lock<std::mutex> lock(m_mutex);
      events.swap(m_queue);
    }

    while (events.size() > 0) try
    {
      Event event = events.front();
      events.pop_front();
      
      // any exception thrown by the handler will break the message
      // dispatching but you can eventually resume it later.
      event->dispatch(*this);
    }
    catch (...)
    {
      if (events.size() > 0)
      {
        // restoring unprocessed events in the main queue in the correct order
        std::unique_lock<std::mutex> lock(m_mutex);
        while (events.size() > 0)
        {
          m_queue.push_front(events.back());
          events.pop_back();
        }
      }
      
      // forwarding the exception to the outer layer: derived class can
      // reimplement the exec() and handle the exception there, then resume the
      // event processing by calling dispatch again.
      throw;
    }
  }
  
private:

  std::mutex m_mutex;       //!< mutex for queue access
  EventsQueue m_queue;      //!< queue of events
  std::thread m_thread;     //!< thread object
  std::atomic<bool> m_done; //!< flag for thread termination
  
  ActiveObject(const ActiveObject&) = delete;
  ActiveObject& operator =(const ActiveObject&) = delete;
};
  
  
} // end of namespace

#endif // DYNAMIC_ACTIVEOBJECT_H
