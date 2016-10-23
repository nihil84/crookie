#ifndef DYNAMIC_ACTIVEOBJECT_H
#define DYNAMIC_ACTIVEOBJECT_H

#include "AEventHandler.h"

#include <list>
#include <mutex>
#include <thread>


namespace crookie {

  
/**
 * @brief
 * 
 */
template <class... EventTypes>
class ActiveObject : public AEventHandler<EventTypes>...
{
public:
  
  typedef std::list< Event > EventsQueue;

  ActiveObject() { }
  
  ActiveObject(const ActiveObject&) = delete;
  
  explicit ActiveObject(EventBus& bus)
    : AEventHandler<EventTypes>(bus)...
    , m_done(false)
  { }

  ~ActiveObject()
  {
    this->quit();
    this->join();
  }
  
  virtual void handle(const Event& event) override
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push_back(event);
  }
  
  virtual void start()
  {
    if (m_thread.joinable() && m_thread.get_id() != std::this_thread::get_id())
      return;
    
    m_thread = std::thread(&ActiveObject::run, this);
  }
  
  void quit()
  {
    m_done = true;
  }
  
  void join()
  {
    if (m_thread.joinable())
      m_thread.join();
  }

protected:
  
  bool done() { return m_done; }
  
  virtual void run()
  {
    while (!done())
    {
      this->dispatch();
    }
  }
  
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

  std::mutex m_mutex;
  EventsQueue m_queue;
  std::thread m_thread;
  std::atomic<bool> m_done;
};
  
  
} // end of namespace

#endif // DYNAMIC_ACTIVEOBJECT_H
